/* servo-v2-bricklet
 * Copyright (C) 2019 Olaf Lüke <olaf@tinkerforge.com>
 *
 * current.h: Driver for Servo current measurements
 *
 * This library is free software; you can redistribute it and/or
 * modify it under the terms of the GNU Lesser General Public
 * License as published by the Free Software Foundation; either
 * version 2 of the License, or (at your option) any later version.
 *
 * This library is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the GNU
 * Lesser General Public License for more details.
 *
 * You should have received a copy of the GNU Lesser General Public
 * License along with this library; if not, write to the
 * Free Software Foundation, Inc., 59 Temple Place - Suite 330,
 * Boston, MA 02111-1307, USA.
 */

#ifndef CURRENT_H
#define CURRENT_H

#include "xmc_vadc.h"
#include "xmc_gpio.h"

#include <stdint.h>
#include <stdbool.h>

#define INPUT_VOLTAGE_INDEX 10

#define CURRENT_NUM 11

#define CURRENT_CALIBRATION_PAGE           1
#define CURRENT_CALIBRATION_OFFSET_POS     0
#define CURRENT_CALIBRATION_MAGIC_POS      10

#define CURRENT_CALIBRATION_MAGIC          0x12345678

typedef struct {
    // Pin
    XMC_GPIO_PORT_t *port;
    uint8_t pin;

	// ADC config
	uint8_t result_reg;
	uint8_t channel_num;
	int8_t channel_alias;
	uint8_t group_index;
	XMC_VADC_GROUP_t *group;

	// ADC result
	uint64_t result_sum;
	uint32_t result_count;
	uint32_t result; // mA

	// Channel config
	uint8_t averaging_duration;
	int16_t offset;

	// Channel config changed by API
	bool new_averaging_duration;
} Current;

extern Current current[CURRENT_NUM];

void current_init(void);
void current_tick(void);
void current_calibration_write(void);
void current_calibration_read(void);

#endif