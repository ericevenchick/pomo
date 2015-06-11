#include <pebble.h>
#include "ui.h"

Window *window;
TextLayer *clock_text_layer;
TextLayer *status_text_layer;
TextLayer *timer_text_layer;
ActionBarLayer *action_bar;

void ui_init() {
    /* create the main window */
    window = window_create();

    /* create the text layers */
    clock_text_layer = text_layer_create(GRect(0, 0, 144-ACTION_BAR_WIDTH, 29));
    status_text_layer = text_layer_create(GRect(0, 30, 144-ACTION_BAR_WIDTH, 39));
    timer_text_layer = text_layer_create(GRect(0, 70, 144-ACTION_BAR_WIDTH, 39));

    /* setup clock text layer */
    text_layer_set_font(clock_text_layer, fonts_get_system_font(FONT_KEY_ROBOTO_CONDENSED_21));
    text_layer_set_text_alignment(clock_text_layer, GTextAlignmentCenter);
    layer_add_child(window_get_root_layer(window), text_layer_get_layer(clock_text_layer));

    /* setup status text layer */
    text_layer_set_text(status_text_layer, "Ready!");
    text_layer_set_font(status_text_layer, fonts_get_system_font(FONT_KEY_GOTHIC_28_BOLD));
    text_layer_set_text_alignment(status_text_layer, GTextAlignmentCenter);
    layer_add_child(window_get_root_layer(window), text_layer_get_layer(status_text_layer));

    /* setup timer text layer */
    text_layer_set_font(timer_text_layer, fonts_get_system_font(FONT_KEY_GOTHIC_28_BOLD));
    text_layer_set_text_alignment(timer_text_layer, GTextAlignmentCenter);
    layer_add_child(window_get_root_layer(window), text_layer_get_layer(timer_text_layer));

    /* push the window to the display */
    window_stack_push(window, true);

    /* set up the action bar */
    /* TODO: icons */
    action_bar = action_bar_layer_create();
    /* bind the action bar to the window */
    action_bar_layer_add_to_window(action_bar, window);
}

/* function to set click handlers */
void set_click_provider(ClickConfigProvider provider) {
    action_bar_layer_set_click_config_provider(action_bar, provider);
}

/* function to set value of the clock text layer */
void set_clock_text(const char* str) {
    text_layer_set_text(clock_text_layer, str);
}

/* function to set value of the status text layer */
void set_status_text(const char* str) {
    text_layer_set_text(status_text_layer, str);
}

/* function to set value of the timer text layer */
void set_timer_text(const char* str) {
    text_layer_set_text(timer_text_layer, str);
}

/* function to set the background color */
void set_background_color(ui_colors_t color) {
#ifdef PBL_COLOR

    GColor main_color;
    GColor bar_color;
    GColor text_color;

    /* set text, action bar, and text color based on
       requested background color */
    switch (color) {
    case WHITE:
        main_color = GColorWhite;
        bar_color = GColorBlack;
        text_color = GColorBlack;
        break;
    case GREEN:
        main_color = GColorBrightGreen;
        bar_color = GColorJaegerGreen;
        text_color = GColorBlack;
        break;
    case RED:
        main_color = GColorRed;
        bar_color = GColorDarkCandyAppleRed;
        text_color = GColorBlack;
        break;
    }

    /* apply color changes */
    window_set_background_color(window, main_color);
    action_bar_layer_set_background_color(action_bar, bar_color);
    text_layer_set_background_color(clock_text_layer, main_color);
    text_layer_set_background_color(status_text_layer, main_color);
    text_layer_set_background_color(timer_text_layer, main_color);
    text_layer_set_text_color(clock_text_layer, text_color);
    text_layer_set_text_color(status_text_layer, text_color);
    text_layer_set_text_color(timer_text_layer, text_color);
#endif
}

/* destroys the UI */
void ui_deinit() {
    /* destroy text layers */
    text_layer_destroy(clock_text_layer);
    text_layer_destroy(status_text_layer);
    text_layer_destroy(timer_text_layer);

    /* destroy action bar */
    action_bar_layer_destroy(action_bar);

    /* destroy the window */
    window_destroy(window);
}
