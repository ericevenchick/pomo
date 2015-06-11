#pragma once

typedef enum _colors {
  WHITE = 0,
  GREEN = 1,
  RED = 2
} ui_colors_t;

void ui_init();
void set_click_provider(ClickConfigProvider provider);
void set_clock_text(const char* str);
void set_status_text(const char* str);
void set_timer_text(const char* str);
void set_background_color(ui_colors_t color);
void ui_deinit();