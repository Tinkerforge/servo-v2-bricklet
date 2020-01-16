/* servo-v2-bricklet
 * Copyright (C) 2019 Olaf Lüke <olaf@tinkerforge.com>
 *
 * pwm.c: Driver for PWM output
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

#include "pwm.h"

#include "bricklib2/hal/system_timer/system_timer.h"
#include "bricklib2/utility/util_definitions.h"

#include "xmc_scu.h"

// us to compare/period value:
// CP = US*1000*1000*1000*96/(1000*1000*1000*PRESCALER)
//    = US*96/PRESCALER
#define PWM_US_TO_PERIOD(US, PRESCALER) (((US)*96)/(1 << (PRESCALER)))

PWM pwm[PWM_NUM] = {
	{ // Channel 0
		.is_ccu4            = true,
		.port               = XMC_GPIO_PORT0,
		.pin                = 1,
		.mode               = XMC_GPIO_MODE_OUTPUT_PUSH_PULL_ALT4,
		.ccu4_module        = CCU40,
		.ccu4_slice         = CCU40_CC41,
		.ccu4_slice_number  = 1,
		.ccu4_transfer_mask = XMC_CCU4_SHADOW_TRANSFER_SLICE_1 | XMC_CCU4_SHADOW_TRANSFER_PRESCALER_SLICE_1
	},
	{ // Channel 1
		.is_ccu4            = true,
		.port               = XMC_GPIO_PORT0,
		.pin                = 3,
		.mode               = XMC_GPIO_MODE_OUTPUT_PUSH_PULL_ALT4,
		.ccu4_module        = CCU40,
		.ccu4_slice         = CCU40_CC43,
		.ccu4_slice_number  = 3,
		.ccu4_transfer_mask = XMC_CCU4_SHADOW_TRANSFER_SLICE_3 | XMC_CCU4_SHADOW_TRANSFER_PRESCALER_SLICE_3
	},
	{ // Channel 2
		.is_ccu4            = true,
		.port               = XMC_GPIO_PORT0,
		.pin                = 2,
		.mode               = XMC_GPIO_MODE_OUTPUT_PUSH_PULL_ALT4,
		.ccu4_module        = CCU40,
		.ccu4_slice         = CCU40_CC42,
		.ccu4_slice_number  = 2,
		.ccu4_transfer_mask = XMC_CCU4_SHADOW_TRANSFER_SLICE_2 | XMC_CCU4_SHADOW_TRANSFER_PRESCALER_SLICE_2
	},
	{ // Channel 3
		.is_ccu4            = true,
		.port               = XMC_GPIO_PORT0,
		.pin                = 0,
		.mode               = XMC_GPIO_MODE_OUTPUT_PUSH_PULL_ALT4,
		.ccu4_module        = CCU40,
		.ccu4_slice         = CCU40_CC40,
		.ccu4_slice_number  = 0,
		.ccu4_transfer_mask = XMC_CCU4_SHADOW_TRANSFER_SLICE_0 | XMC_CCU4_SHADOW_TRANSFER_PRESCALER_SLICE_0
	},
	{ // Channel 4
		.is_ccu4            = true,
		.port               = XMC_GPIO_PORT0,
		.pin                = 6,
		.mode               = XMC_GPIO_MODE_OUTPUT_PUSH_PULL_ALT9,
		.ccu4_module        = CCU41,
		.ccu4_slice         = CCU41_CC40,
		.ccu4_slice_number  = 0,
		.ccu4_transfer_mask = XMC_CCU4_SHADOW_TRANSFER_SLICE_0 | XMC_CCU4_SHADOW_TRANSFER_PRESCALER_SLICE_0
	},
	{ // Channel 5
		.is_ccu4            = false,
		.port               = XMC_GPIO_PORT0,
		.pin                = 12,
		.mode               = XMC_GPIO_MODE_OUTPUT_PUSH_PULL_ALT7,
		.ccu8_module        = CCU80,
		.ccu8_channel       = XMC_CCU8_SLICE_COMPARE_CHANNEL_1,
		.ccu8_slice         = CCU80_CC82,
		.ccu8_slice_number  = 2,
		.ccu8_transfer_mask = XMC_CCU8_SHADOW_TRANSFER_SLICE_2 | XMC_CCU8_SHADOW_TRANSFER_PRESCALER_SLICE_2
	},
	{ // Channel 6
		.is_ccu4            = false,
		.port               = XMC_GPIO_PORT0,
		.pin                = 15,
		.mode               = XMC_GPIO_MODE_OUTPUT_PUSH_PULL_ALT5,
		.ccu8_module        = CCU80,
		.ccu8_channel       = XMC_CCU8_SLICE_COMPARE_CHANNEL_1,
		.ccu8_slice         = CCU80_CC83,
		.ccu8_slice_number  = 3,
		.ccu8_transfer_mask = XMC_CCU4_SHADOW_TRANSFER_SLICE_3 | XMC_CCU4_SHADOW_TRANSFER_PRESCALER_SLICE_3
	},
	{ // Channel 7
		.is_ccu4            = true,
		.port               = XMC_GPIO_PORT0,
		.pin                = 9,
		.mode               = XMC_GPIO_MODE_OUTPUT_PUSH_PULL_ALT9,
		.ccu4_module        = CCU41,
		.ccu4_slice         = CCU41_CC43,
		.ccu4_slice_number  = 3,
		.ccu4_transfer_mask = XMC_CCU4_SHADOW_TRANSFER_SLICE_3 | XMC_CCU4_SHADOW_TRANSFER_PRESCALER_SLICE_3
	},
	{ // Channel 8
		.is_ccu4            = true,
		.port               = XMC_GPIO_PORT0,
		.pin                = 8,
		.mode               = XMC_GPIO_MODE_OUTPUT_PUSH_PULL_ALT9,
		.ccu4_module        = CCU41,
		.ccu4_slice         = CCU41_CC42,
		.ccu4_slice_number  = 2,
		.ccu4_transfer_mask = XMC_CCU4_SHADOW_TRANSFER_SLICE_2 | XMC_CCU4_SHADOW_TRANSFER_PRESCALER_SLICE_2
	},
	{ // Channel 9
		.is_ccu4            = true,
		.port               = XMC_GPIO_PORT0,
		.pin                = 7,
		.mode               = XMC_GPIO_MODE_OUTPUT_PUSH_PULL_ALT9,
		.ccu4_module        = CCU41,
		.ccu4_slice         = CCU41_CC41,
		.ccu4_slice_number  = 1,
		.ccu4_transfer_mask = XMC_CCU4_SHADOW_TRANSFER_SLICE_1 | XMC_CCU4_SHADOW_TRANSFER_PRESCALER_SLICE_1
	}
};


// Period value is the amount of clock cycles per period
void pwm_init_channel(const PWM *conf) {
	if(conf->is_ccu4) {
		const XMC_CCU4_SLICE_COMPARE_CONFIG_t compare_config = {
			.timer_mode          = XMC_CCU4_SLICE_TIMER_COUNT_MODE_EA,
			.monoshot            = false,
			.shadow_xfer_clear   = 0,
			.dither_timer_period = 0,
			.dither_duty_cycle   = 0,
			.prescaler_mode      = XMC_CCU4_SLICE_PRESCALER_MODE_NORMAL,
			.mcm_enable          = 0,
			.prescaler_initval   = conf->prescaler,
			.float_limit         = 0,
			.dither_limit        = 0,
			.passive_level       = XMC_CCU4_SLICE_OUTPUT_PASSIVE_LEVEL_HIGH,
			.timer_concatenation = 0
		};

		XMC_CCU4_Init(conf->ccu4_module, XMC_CCU4_SLICE_MCMS_ACTION_TRANSFER_PR_CR);
		XMC_CCU4_StartPrescaler(conf->ccu4_module);
		XMC_CCU4_SLICE_CompareInit(conf->ccu4_slice, &compare_config);

		XMC_CCU4_EnableShadowTransfer(conf->ccu4_module, conf->ccu4_transfer_mask);

		XMC_CCU4_EnableClock(conf->ccu4_module, conf->ccu4_slice_number);
		XMC_CCU4_SLICE_StartTimer(conf->ccu4_slice);
	} else {
		const XMC_CCU8_SLICE_COMPARE_CONFIG_t compare_config = {
			.timer_mode          = XMC_CCU8_SLICE_TIMER_COUNT_MODE_EA,
			.monoshot            = false,
			.shadow_xfer_clear   = 0,
			.dither_timer_period = 0,
			.dither_duty_cycle   = 0,
			.prescaler_mode      = XMC_CCU8_SLICE_PRESCALER_MODE_NORMAL,
			.prescaler_initval   = conf->prescaler,
			.float_limit         = 0,
			.dither_limit        = 0,
			.passive_level_out0  = XMC_CCU8_SLICE_OUTPUT_PASSIVE_LEVEL_HIGH,
			.passive_level_out1  = XMC_CCU8_SLICE_OUTPUT_PASSIVE_LEVEL_HIGH,
			.passive_level_out2  = XMC_CCU8_SLICE_OUTPUT_PASSIVE_LEVEL_HIGH,
			.passive_level_out3  = XMC_CCU8_SLICE_OUTPUT_PASSIVE_LEVEL_HIGH,
			.timer_concatenation = 0
		};

		XMC_CCU8_Init(conf->ccu8_module, XMC_CCU8_SLICE_MCMS_ACTION_TRANSFER_PR_CR);
		XMC_CCU8_StartPrescaler(conf->ccu8_module);
		XMC_CCU8_SLICE_CompareInit(conf->ccu8_slice, &compare_config);

		XMC_CCU8_EnableShadowTransfer(conf->ccu8_module, conf->ccu8_transfer_mask);

		XMC_CCU8_EnableClock(conf->ccu8_module, conf->ccu8_slice_number);
		XMC_CCU8_SLICE_StartTimer(conf->ccu8_slice);
	}
}

void pwm_set_period_match(const PWM *conf, const uint32_t period_value, const uint32_t prescaler) {
	if(conf->is_ccu4) {
		conf->ccu4_slice->PSC = prescaler;
		XMC_CCU4_SLICE_SetTimerPeriodMatch(conf->ccu4_slice, period_value-1);
	} else {
		conf->ccu8_slice->PSC = prescaler;
		XMC_CCU8_SLICE_SetTimerPeriodMatch(conf->ccu8_slice, period_value-1);
	}
}

void pwm_set_compare_match(const PWM *conf, const uint16_t compare_value) {
	if(conf->is_ccu4) {
		XMC_CCU4_SLICE_SetTimerCompareMatch(conf->ccu4_slice, compare_value);
	} else {
		XMC_CCU8_SLICE_SetTimerCompareMatch(conf->ccu8_slice, conf->ccu8_channel, compare_value);
	}
}

void pwm_enable_shadow_transfer(const PWM *conf) {
	if(conf->is_ccu4) {
		XMC_CCU4_EnableShadowTransfer(conf->ccu4_module, conf->ccu4_transfer_mask);
	} else {
		XMC_CCU8_EnableShadowTransfer(conf->ccu8_module, conf->ccu8_transfer_mask);
	}
}

void pwm_init(void) {
	for(uint8_t i = 0; i < PWM_NUM; i++) {
		pwm[i].enabled          = true;
		pwm[i].position         = 0;     // °/100
		pwm[i].current_position = 0;     // °/100
		pwm[i].velocity         = 65535; // °/100s
		pwm[i].current_velocity = 0;     // °/100s
		pwm[i].acceleration     = 65535; // °/100s^2
		pwm[i].deceleration     = 65535; // °/100s^2
		pwm[i].pulse_width_min  = 1000;  // us
		pwm[i].pulse_width_max  = 2000;  // us
		pwm[i].degree_min       = -9000; // °/100
		pwm[i].degree_max       = 9000;  // °/100
		pwm[i].period           = 19500; // us

		pwm[i].prescaler        = XMC_CCU4_SLICE_PRESCALER_32;

		pwm[i].new_enabled      = true;
		pwm[i].new_position     = true;
		pwm[i].new_motion       = true;
		pwm[i].new_pulse_width  = true;
		pwm[i].new_degree       = true;
		pwm[i].new_period       = true;

		pwm_init_channel(&pwm[i]);
	}
}

void pwm_tick(void) {
	for(uint8_t i = 0; i < PWM_NUM; i++) {
		if(pwm[i].new_period || pwm[i].new_degree || pwm[i].new_pulse_width) {
			pwm[i].new_period      = false;
			pwm[i].new_degree      = false;
			pwm[i].new_pulse_width = false;

			pwm[i].prescaler = XMC_CCU4_SLICE_PRESCALER_64;
			for(uint8_t prescaler = 0; prescaler < 16; prescaler++) {
				if(PWM_US_TO_PERIOD(pwm[i].period, prescaler) <= UINT16_MAX) {
					pwm[i].prescaler = prescaler;
					break;
				}
			}
			pwm_set_period_match(&pwm[i], PWM_US_TO_PERIOD(pwm[i].period, pwm[i].prescaler), pwm[i].prescaler);

			// When we change the period, degree or pulse width
			// we also have to update the position
			pwm[i].new_position = true;
		}

		if(pwm[i].new_position) {
			pwm[i].new_position = false;

			pwm[i].current_position = pwm[i].position;
			uint32_t value = SCALE((int64_t)pwm[i].position,
								   (int64_t)pwm[i].degree_min,
								   (int64_t)pwm[i].degree_max,
								   (int64_t)PWM_US_TO_PERIOD(pwm[i].pulse_width_min, pwm[i].prescaler),
								   (int64_t)PWM_US_TO_PERIOD(pwm[i].pulse_width_max, pwm[i].prescaler));
			pwm_set_compare_match(&pwm[i], value);
			pwm_enable_shadow_transfer(&pwm[i]);
		}

		if(pwm[i].new_enabled) {
			pwm[i].new_enabled = false;
			if(pwm[i].enabled) {
				XMC_GPIO_CONFIG_t gpio_out_config	= {
					.mode         = pwm[i].mode,
					.output_level = XMC_GPIO_OUTPUT_LEVEL_LOW,
				};
				
				XMC_GPIO_Init(pwm[i].port, pwm[i].pin, &gpio_out_config);
			} else {
				XMC_GPIO_CONFIG_t gpio_in_config	= {
					.mode             = XMC_GPIO_MODE_INPUT_TRISTATE,
					.input_hysteresis = XMC_GPIO_INPUT_HYSTERESIS_STANDARD
				};
				
				XMC_GPIO_Init(pwm[i].port, pwm[i].pin, &gpio_in_config);
			}

		}
	}
}