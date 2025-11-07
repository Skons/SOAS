import esphome.codegen as cg
import esphome.config_validation as cv
from esphome.const import CONF_ID, CONF_PORT
from esphome.core import CORE
import os
import binascii

DEPENDENCIES = ['wifi', 'esp32', 'web_server']
AUTO_LOAD = []

CONF_ALARM_FILE = 'alarm_file'

simple_audio_server_ns = cg.esphome_ns.namespace('web_server')
SimpleAudioServerComponent = simple_audio_server_ns.class_('SimpleAudioServerComponent', cg.Component)

CONFIG_SCHEMA = cv.Schema({
    cv.GenerateID(): cv.declare_id(SimpleAudioServerComponent),
    cv.Required(CONF_ALARM_FILE): cv.string,
}).extend(cv.COMPONENT_SCHEMA)

async def to_code(config):
    var = cg.new_Pvariable(config[CONF_ID])
    await cg.register_component(var, config)

    cg.add(var.set_alarm_file(config[CONF_ALARM_FILE]))

    alarm_filename = config[CONF_ALARM_FILE]

    # Get project directory - try multiple approaches
    if hasattr(CORE, 'config_dir') and CORE.config_dir:
        project_dir = CORE.config_dir
    elif hasattr(CORE, 'build_path'):
        project_dir = os.path.dirname(str(CORE.build_path))
    else:
        project_dir = os.getcwd()

    audio_file_path = os.path.join(project_dir, alarm_filename)

    print(f"[AUDIO SERVER] Looking for: {audio_file_path}")

    if os.path.exists(audio_file_path):
        try:
            # Read the file
            with open(audio_file_path, 'rb') as f:
                audio_data = f.read()

            file_size = len(audio_data)
            if file_size < 256000:
                raise cv.Invalid("The minimum filesize is 256KB for proper streaming. Please use a larger audio file.")
            print(f"[AUDIO SERVER] Found {alarm_filename}: {file_size} bytes")

            # Create a simple hex string (no fancy formatting)
            hex_string = binascii.hexlify(audio_data).decode('ascii')

            # Convert to C array format: 0x12,0x34,0x56...
            hex_pairs = [hex_string[i:i+2] for i in range(0, len(hex_string), 2)]
            c_array = ','.join(f'0x{pair}' for pair in hex_pairs)

            # Generate the embedded C code
            cg.add_global(cg.RawStatement(f'''
// Auto-generated embedded audio data for {alarm_filename}
const unsigned char embedded_audio_file[] PROGMEM = {{
{c_array}
}};
const unsigned int embedded_audio_file_len = {file_size};
'''))

            # Tell the component about the embedded data
            cg.add(var.set_audio_data(
                cg.RawExpression("embedded_audio_file"),
                cg.RawExpression("embedded_audio_file_len")
            ))

            print(f"[AUDIO SERVER] Successfully embedded {alarm_filename} ({file_size} bytes)")

        except Exception as e:
            raise cv.Invalid(str(e))

    else:
        raise cv.Invalid(f"File not found: {audio_file_path}")