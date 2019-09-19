/* servo-v2-bricklet
 * Copyright (C) 2019 Olaf Lüke <olaf@tinkerforge.com>
 *
 * pwm.h: Driver for PWM output
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

#ifndef PWM_H
#define PWM_H

#include <stdint.h>
#include <stdbool.h>

#include "xmc_ccu4.h"
#include "xmc_ccu8.h"
#include "xmc_gpio.h"

#define PWM_NUM 10

typedef struct {
    // CCU4/8
    bool is_ccu4; // true = ccu4, false = ccu8

    // Pin
    XMC_GPIO_PORT_t *port;
    uint8_t pin;
    XMC_GPIO_MODE_t mode;

    // CCU4 config
    XMC_CCU4_MODULE_t *ccu4_module;
    XMC_CCU4_SLICE_t *ccu4_slice;
    uint8_t ccu4_slice_number;
    uint32_t ccu4_transfer_mask; // XMC_CCU4_SHADOW_TRANSFER_t (shadow transfer slice and shadow transfer prescaler slice)

    // CCU8 config
    XMC_CCU8_MODULE_t *ccu8_module;
    XMC_CCU8_SLICE_t *ccu8_slice;
    XMC_CCU8_SLICE_COMPARE_CHANNEL_t ccu8_channel;
    uint8_t ccu8_slice_number;
    uint32_t ccu8_transfer_mask; // XMC_CCU8_SHADOW_TRANSFER_t (shadow transfer slice and shadow transfer prescaler slice)
} PWM;

extern PWM pwm[PWM_NUM];

void pwm_init(void);
void pwm_tick(void);

#endif