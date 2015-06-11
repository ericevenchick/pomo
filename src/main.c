#include <pebble.h>
#include "ui.h"

/* enumeration of the two Pomodoro phases: work and break */
enum phases {
    PHASE_WORK = 0, /**< Work phase */
    PHASE_BREAK = 1 /**< Break phase */
};

/* one minute in milliseconds */
#define MINUTE_MS 1000 * 60

/* stores the current phase, start in work phase */
char phase = PHASE_WORK;

/* buffers to store strings for the text layers */
char status_text[50];
char clock_text[sizeof("12:34")];
char timer_text[sizeof("123 min.")];

/* minutes left on timer, -1 if timer is not running */
int minutes = -1;

/* static function prototypes */
static void phase_start();
static void phase_end(void* data);
static void minute_tick(void* data);

/* callback that fires for every minute of a Pomodoro phase */
static void minute_tick(void* data) {
    /* a minute has elapsed */
    minutes--;

    if (minutes > 0) {
	/* there is still time left */
	/* update the timer display with the current number of minutes */
        snprintf(timer_text, sizeof(timer_text), "%d min.", minutes);
        set_timer_text(timer_text);

	/* fire this callback again after the next minute elapses */
        app_timer_register(MINUTE_MS, minute_tick, NULL);
    } else if (minutes == 0) {
	/* no time left, blank the timer display and end the phase */
        set_timer_text("");
        phase_end(NULL);
    }
}

/* called at the start of a phase */
static void phase_start() {
    /* display the current phase, set the number of minutes in the phase,
       and set the background color to correspond to the phase */
    if (phase == PHASE_WORK) {
        snprintf(status_text, sizeof(status_text), "Work...");
        minutes = 25;
        set_background_color(RED);
    } else {
        snprintf(status_text, sizeof(status_text), "Break...");
        minutes = 5;
        set_background_color(GREEN);
    }
    set_status_text(status_text);
    snprintf(timer_text, sizeof(timer_text), "%d min.", minutes);
    set_timer_text(timer_text);

    /* fire a timer callback after the first minute elapses */
    app_timer_register(MINUTE_MS, minute_tick, NULL);
}

/* called at the end of a phase */
static void phase_end(void* data) {
    if (phase == PHASE_WORK) {
        /* work time done, break time next */
        snprintf(status_text, sizeof(status_text), "Break Time!");
        phase = PHASE_BREAK;
    } else {
        /* work time done, break time next */
        snprintf(status_text, sizeof(status_text), "Work Time!");
        phase = PHASE_WORK;
    }
    set_status_text(status_text);
    set_background_color(WHITE);

    /* vibrate! */
    vibes_short_pulse();

    /* set the minutes to -1 to stop the timer */
    minutes = -1;
}


/* handler for select button press */
void select_single_click_handler(ClickRecognizerRef recognizer, void *context) {
    /* start the phase of the Pomodoro timer */
    phase_start();
}

/* configures handlers for button presses */
void config_provider(Window *window) {
    /* select button */
    window_single_click_subscribe(BUTTON_ID_SELECT, select_single_click_handler);
}

/* called when the system time changes by one minute */
static void tick_handler(struct tm *tick_time, TimeUnits units_changed) {
    /* update the clock at the top of the display */
    clock_copy_time_string(clock_text, sizeof(clock_text));
    set_clock_text(clock_text);
}

void app_init(void) {
    /* draw the UI */
    ui_init();

    /* set up button handlers */
    set_click_provider((ClickConfigProvider)config_provider);

    /* set up clock handler */
    tick_timer_service_subscribe(MINUTE_UNIT, tick_handler);
}

void app_deinit(void) {
    /* destroy the UI */
    ui_deinit();
}

int main(void) {
    app_init();
    app_event_loop();
    app_deinit();
}
