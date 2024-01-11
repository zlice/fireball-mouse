/* Copyright 2024 zlice
 * Copyright 2020 Christopher Courtney, aka Drashna Jael're  (@drashna) <drashna@live.com>
 * Copyright 2019 Sunjun Kim
 * Copyright 2020 Ploopy Corporation
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 2 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */

#include QMK_KEYBOARD_H

// hall sensor pins
#define HAL1 GP26 // unused - only works well for single direction
#define HAL2 GP27 // good for both directions
//#define MAG_UP 770 // up - js magnet towards hand / stick forward to usb
//#define MAG_DN 770 // down - js magnet towards usb / stick back to hand

#ifndef OPT_DEBOUNCE
#    define OPT_DEBOUNCE 25       // (ms) Time between scroll events - orig 5
#    define OPT_DEBOUNCE_FAST 10  // (ms) Time between scroll events - orig 5
#endif
#ifndef SCROLL_BUTT_DEBOUNCE
#    define SCROLL_BUTT_DEBOUNCE 100  // (ms) Time between scroll events
#endif
#ifndef OPT_THRES
#    define OPT_THRES 150  // (0-1024) Threshold for actication
#endif
#ifndef OPT_SCALE
#    define OPT_SCALE 1  // Multiplier for wheel
#endif
#ifndef PLOOPY_DPI_OPTIONS
#    define PLOOPY_DPI_OPTIONS { 12000, 12000, 12000 }
#    ifndef PLOOPY_DPI_DEFAULT
#        define PLOOPY_DPI_DEFAULT 1
#    endif
#endif
#ifndef PLOOPY_DPI_DEFAULT
#    define PLOOPY_DPI_DEFAULT 0
#endif

keyboard_config_t keyboard_config;
uint16_t          dpi_array[] = PLOOPY_DPI_OPTIONS;
#define DPI_OPTION_SIZE (sizeof(dpi_array) / sizeof(uint16_t))

// TODO: Implement libinput profiles
// https://wayland.freedesktop.org/libinput/doc/latest/pointer-acceleration.html
// Compile time accel selection
// Valid options are ACC_NONE, ACC_LINEAR, ACC_CUSTOM, ACC_QUADRATIC

// Trackball State
bool     is_scroll_clicked = false;
uint16_t lastScroll        = 0;      // Previous confirmed wheel event
bool     debug_encoder     = false;

uint16_t lastMidClick      = 0;      // Stops scrollwheel from being read if it was pressed
uint16_t cur_opt_debounce = OPT_DEBOUNCE;
uint16_t scroll_hits = 0; // tracker for 40 25ms intervals = 1sec (1000ms)
#define SCROLL_RAMP 40
// finds a high/lo resting point and sets
#define MAG_MARGIN 5 // offset max MAG_XX values before registering both scrolls
#define UP_MARGIN 10 // offset for registering up scrolls
#define DN_MARGIN 18 // offset for registering down scrolls
uint16_t MAG_UP = 888 ; // up - js magnet towards hand / stick forward to usb
uint16_t MAG_DN = 888 ; // down - js magnet towards usb / stick back to hand

// HALL SENSOR JOYSTICK JS
void process_wheel(void) {

    // proper qmk way to read is with mux
    // unused is an error in qmk
    uint16_t val1 = analogReadPin(HAL1);
    uint16_t val2 = analogReadPin(HAL2);

    // Don't scroll if the middle button is depressed.
    if (is_scroll_clicked) {
#ifndef IGNORE_SCROLL_CLICK
      return;
#endif
    }

    // If the mouse wheel was just released, do not scroll.
    if (timer_elapsed(lastMidClick) < SCROLL_BUTT_DEBOUNCE) {
      return;
    }

    // orig val is 5ms - changed to 25, with 10 after 1 sec (40 25ms hits)
    //                   u16 overflows after hour of scrolling?
    // Limit the number of scrolls per unit time.
    cur_opt_debounce = OPT_DEBOUNCE;
    if (scroll_hits > SCROLL_RAMP) {
      cur_opt_debounce = OPT_DEBOUNCE_FAST;
    }

    if (timer_elapsed(lastScroll) < cur_opt_debounce) {
      return;
    }

    // actual joystick scroll logic
    // debug + CONSOLE need enabled in in rules.mk
    // dances around 740 +-, can hit 73X which is similar to 'down'
    // values vary per sensor, PCB and positioning
    // use this to figure out what works for you
    //uprintf("mag read: %u\n", val2);
    //uprintf("MAG_UP : %u\tMAG_DOWN : %u\n", MAG_UP, MAG_DN);
    //uprintf("val1 : %u\tval2 : %u\n", val1, val2);

    lastScroll = timer_read();
    scroll_hits += 1;

    // dual sensors
    if (val1 < MAG_UP) {
      tap_code(KC_WH_U);
    } else if (val2 < MAG_DN) {
      tap_code(KC_WH_D);
    } else {
      scroll_hits = 0;
    }
    // single sensor
    // if (val2 < MAG_DN) {
    //   tap_code(KC_WH_D);
    // } else if (val2 > MAG_UP) {
    //   tap_code(KC_WH_U);
    // } else {
    //   scroll_hits = 0;
    // }
}

report_mouse_t pointing_device_task_kb(report_mouse_t mouse_report) {
    process_wheel();

    // invert / reverse / upside down sensor
    mouse_report.x = mouse_report.x * -1;
    mouse_report.y = mouse_report.y * -1;

    return pointing_device_task_user(mouse_report);
}

bool process_record_kb(uint16_t keycode, keyrecord_t* record) {
    // Update Timer to prevent accidental scrolls
    if ((record->event.key.col == 1) && (record->event.key.row == 0)) {
        lastMidClick      = timer_read();
        is_scroll_clicked = record->event.pressed;
    }

    if (!process_record_user(keycode, record)) {
        return false;
    }

    if (keycode == DPI_CONFIG && record->event.pressed) {
        keyboard_config.dpi_config = (keyboard_config.dpi_config + 1) % DPI_OPTION_SIZE;
        eeconfig_update_kb(keyboard_config.raw);
        pointing_device_set_cpi(dpi_array[keyboard_config.dpi_config]);
    }

    return true;
}

// Hardware Setup
void keyboard_pre_init_kb(void) {
    // debug_enable  = true;
    // debug_matrix  = true;
    // debug_mouse   = true;
    // debug_encoder = true;

    setPinInput(HAL1);
    setPinInput(HAL2);
    uint16_t hal = 0;
    // more reads (without touching) will find a better 'rest' point
    for (int i = 0 ; i < 12 ; i++) {
      hal = analogReadPin(HAL2);
      if (hal < MAG_DN)
        MAG_DN = hal;
      hal = analogReadPin(HAL1);
      if (hal < MAG_UP)
        MAG_UP = hal;
    }
    MAG_DN -= (DN_MARGIN + MAG_MARGIN);
    MAG_UP -= (UP_MARGIN + MAG_MARGIN);

    /* Ground all output pins connected to ground. This provides additional
     * pathways to ground. If you're messing with this, know this: driving ANY
     * of these pins high will cause a short. On the MCU. Ka-blooey.
     */
#ifdef UNUSABLE_PINS
    const pin_t unused_pins[] = UNUSABLE_PINS;

    for (uint8_t i = 0; i < ARRAY_SIZE(unused_pins); i++) {
        setPinOutput(unused_pins[i]);
        writePinLow(unused_pins[i]);
    }
#endif

    // This is the debug LED.
#if defined(DEBUG_LED_PIN)
    setPinOutput(DEBUG_LED_PIN);
    writePin(DEBUG_LED_PIN, debug_enable);
#endif

    keyboard_pre_init_user();
}

void pointing_device_init_kb(void) {
    pointing_device_set_cpi(dpi_array[keyboard_config.dpi_config]);
}

void eeconfig_init_kb(void) {
    keyboard_config.dpi_config = PLOOPY_DPI_DEFAULT;
    eeconfig_update_kb(keyboard_config.raw);
    eeconfig_init_user();
}

void matrix_init_kb(void) {
    // is safe to just read DPI setting since matrix init
    // comes before pointing device init.
    keyboard_config.raw = eeconfig_read_kb();
    if (keyboard_config.dpi_config > DPI_OPTION_SIZE) {
        eeconfig_init_kb();
    }
    matrix_init_user();
}
