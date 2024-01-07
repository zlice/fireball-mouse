/* Copyright 2020 Christopher Courtney, aka Drashna Jael're  (@drashna) <drashna@live.com>
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

#define ADC_PIN GP26

/* Debounce reduces chatter (unintended double-presses) - set 0 if debouncing is not needed */
#define DEBOUNCE 5

/* Much more so than a keyboard, speed matters for a mouse. So we'll go for as high
   a polling rate as possible. */
#define USB_POLLING_INTERVAL_MS 1
#define USB_MAX_POWER_CONSUMPTION 100

/* define if matrix has ghost (lacks anti-ghosting diodes) */
//#define MATRIX_HAS_GHOST

/* Bootmagic Lite key configuration */
// already in info.json - use 'forward' on boot to reflash
//#define BOOTMAGIC_LITE_ROW 0
//#define BOOTMAGIC_LITE_COLUMN 4

/* PMW3360 Settings */
// CS is SS
#define PMW33XX_CS_PIN GP9

// modifiable - defaults 0x02 - in mm
//#define PMW33XX_LIFTOFF_DISTANCE 0x05
//#define PMW33XX_LIFTOFF_DISTANCE 0x03
// 0x00
// #ROTATIONAL_TRANSFORM_ANGLE 0x00

/*
MISO: G4  /  6
MOSI: G3  /  5
SCLK: G2  /  4
NCS:  G9  / 12
MOT:  G1  /  3 - unused, over SPI iirc
ret:  G18 / 29
lclk: G19 / 30
rclk: G29 / 41
mclk: G28 / 40
halB: G27 / 39 - what i used
halM: G26 / 38 - only good for single direction
fw:   G17 / 28
bk:   G16 / 27
*/

#define SPI_SCK_PIN GP2
#define SPI_MOSI_PIN GP3
#define SPI_MISO_PIN GP4
