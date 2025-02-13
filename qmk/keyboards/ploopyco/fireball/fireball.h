/*
 * Copyright 2024 zlice
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

#pragma once

#include "quantum.h"
#include "spi_master.h"
#include "drivers/sensors/pmw3360.h"
#include "analog.h"
#include "pointing_device.h"

void process_mouse(report_mouse_t* mouse_report);
void process_mouse_user(report_mouse_t* mouse_report, int16_t x, int16_t y);
void process_wheel(void);
void process_wheel_user(report_mouse_t* mouse_report, int16_t h, int16_t v);

#define LAYOUT(BL, BM, BR, BF, BB, BRR) \
    { {BL, BM, BR, BF, BB, BRR}, }

typedef union {
  uint32_t raw;
  struct {
    uint8_t    dpi_config;
  };
} keyboard_config_t;

extern keyboard_config_t keyboard_config;
extern uint16_t          dpi_array[];

enum ploopy_keycodes {
#ifdef VIA_ENABLE
    DPI_CONFIG = USER00,
#else
    DPI_CONFIG = SAFE_RANGE,
#endif
    DRAG_SCROLL,
#ifdef VIA_ENABLE
    PLOOPY_SAFE_RANGE = SAFE_RANGE,
#else
    PLOOPY_SAFE_RANGE,
#endif
};
