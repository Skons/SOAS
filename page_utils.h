#pragma once

#include <string>
#include <sys/time.h>

#include "esphome/components/display/display_buffer.h"

template <typename DisplayT, typename FontT>
inline void render_ha_page(DisplayT &it,
                          const std::string &title,
                          float ha_value,
                          bool enabled,
                          bool alarmed,
                          int rotate_switch_single_value,
                          bool show_bottom_left,
                          bool show_bottom_right,
                          bool show_top_left,
                          bool show_top_right,
                          FontT *font_medium40,
                          FontT *font_medium30,
                          FontT *font_medium20,
                          FontT *font_medium15) {
  struct timeval tv;
  gettimeofday(&tv, nullptr);
  int shouldBlink = 0;
  if (tv.tv_usec >= 500000) {  // every 0.5 second
    shouldBlink = 1;
  }

  it.printf(it.get_width() / 2, 0, font_medium20, display::TextAlign::TOP_CENTER, "%s", title.c_str());

  // the font will be placed too high by default
  int fontMarginBottomCorrection = 10;

  int calculated_width;
  int calculated_height;
  int calculated_x;
  int calculated_y;
  std::string displayString = "\U000F07D0";
  it.get_text_bounds(0, 0, displayString.c_str(), font_medium30, display::TextAlign::TOP_CENTER, &calculated_x, &calculated_y, &calculated_width, &calculated_height);
  int iconWidthHalf = calculated_width / 2;

  // ha icon
  it.printf(iconWidthHalf, it.get_height() / 2, font_medium20, display::TextAlign::CENTER, "\U000F07D0");

  int iconLeftRightMargin = 2;
  int iconTopMargin = 4;
  int iconBottomMargin = 3;

  // ha icon hide
  if (shouldBlink) {
    if (show_bottom_right) {
      it.printf(iconWidthHalf + iconLeftRightMargin, (it.get_height() / 2) + iconBottomMargin, font_medium15, COLOR_OFF, display::TextAlign::BOTTOM_RIGHT, "\U000F0764");
    }
    if (show_bottom_left) {
      it.printf(iconWidthHalf - iconLeftRightMargin, (it.get_height() / 2) + iconBottomMargin, font_medium15,  COLOR_OFF, display::TextAlign::BOTTOM_LEFT, "\U000F0764");
    }
    if (show_top_right) {
      it.printf(iconWidthHalf + iconLeftRightMargin, (it.get_height() / 2) - iconTopMargin, font_medium15, COLOR_OFF, display::TextAlign::TOP_RIGHT, "\U000F0764");
    }
    if (show_top_left) {
      it.printf(iconWidthHalf - iconLeftRightMargin, (it.get_height() / 2) - iconTopMargin, font_medium15, COLOR_OFF, display::TextAlign::TOP_LEFT, "\U000F0764");
    }
  }

  // ha value
  if ((shouldBlink && rotate_switch_single_value == 1) || rotate_switch_single_value != 1) {
    if (it.get_width() == 128 && it.get_height() == 128) {  // sh1107
      it.printf((it.get_width() / 2), (it.get_height() / 2), font_medium40, display::TextAlign::CENTER, "%i", (int) ha_value);
    } else {
      it.printf((it.get_width() / 2), it.get_height() + fontMarginBottomCorrection, font_medium40, display::TextAlign::BOTTOM_CENTER, "%i", (int) ha_value);
    }
  }

  // ha enabled
  if (enabled) {
    it.printf(it.get_width(), (it.get_height() / 2), font_medium20, display::TextAlign::CENTER_RIGHT, "\U000F0C52");
  } else {
    it.printf(it.get_width(), (it.get_height() / 2), font_medium20, display::TextAlign::CENTER_RIGHT, "\U000F0131");
  }

  // ha alarmed
  it.get_text_bounds(0, 0, displayString.c_str(), font_medium20, display::TextAlign::TOP_CENTER, &calculated_x, &calculated_y, &calculated_width, &calculated_height);
  if (alarmed) {
    it.printf(it.get_width(), (it.get_height() / 2) + calculated_height, font_medium20, display::TextAlign::CENTER_RIGHT, "\U000F0021");
  } else {
    it.printf(it.get_width(), (it.get_height() / 2) + calculated_height, font_medium20, display::TextAlign::CENTER_RIGHT, "\U000F0023 ");
  }
}
