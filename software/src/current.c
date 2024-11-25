/* servo-v2-bricklet
 * Copyright (C) 2019 Olaf Lüke <olaf@tinkerforge.com>
 *
 * current.c: Driver for Servo current measurements
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

#include "current.h"

#include "bricklib2/hal/system_timer/system_timer.h"
#include "bricklib2/logging/logging.h"
#include "bricklib2/bootloader/bootloader.h"
#include "bricklib2/utility/util_definitions.h"

Current current[CURRENT_NUM] = {
	{ // Channel 0
		.port          = XMC_GPIO_PORT2,
		.pin           = 5,
		.result_reg    = 11,
		.channel_num   = 7,
		.channel_alias = -1,
		.group_index   = 1,
		.group         = VADC_G1
	},
	{ // Channel 1
		.port          = XMC_GPIO_PORT2,
		.pin           = 6,
		.result_reg    = 9,
		.channel_num   = 0,
		.channel_alias = -1,
		.group_index   = 0,
		.group         = VADC_G0
	},
	{ // Channel 2
		.port          = XMC_GPIO_PORT2,
		.pin           = 7,
		.result_reg    = 10,
		.channel_num   = 0,
		.channel_alias = 1,
		.group_index   = 1,
		.group         = VADC_G1
	},
	{ // Channel 3
		.port          = XMC_GPIO_PORT2,
		.pin           = 8,
		.result_reg    = 5,
		.channel_num   = 1,
		.channel_alias = 0,
		.group_index   = 1,
		.group         = VADC_G1
	},
	{ // Channel 4
		.port          = XMC_GPIO_PORT2,
		.pin           = 9,
		.result_reg    = 6,
		.channel_num   = 4,
		.channel_alias = -1,
		.group_index   = 1,
		.group         = VADC_G1
	},
	{ // Channel 5
		.port          = XMC_GPIO_PORT2,
		.pin           = 0,
		.result_reg    = 1,
		.channel_num   = 1,
		.channel_alias = 5,
		.group_index   = 0,
		.group         = VADC_G0
	},
	{ // Channel 6
		.port          = XMC_GPIO_PORT2,
		.pin           = 1,
		.result_reg    = 2,
		.channel_num   = 6,
		.channel_alias = -1,
		.group_index   = 0,
		.group         = VADC_G0
	},
	{ // Channel 7
		.port          = XMC_GPIO_PORT2,
		.pin           = 2,
		.result_reg    = 7,
		.channel_num   = 7,
		.channel_alias = -1,
		.group_index   = 0,
		.group         = VADC_G0
	},
	{ // Channel 8
		.port          = XMC_GPIO_PORT2,
		.pin           = 3,
		.result_reg    = 12,
		.channel_num   = 5,
		.channel_alias = -1,
		.group_index   = 1,
		.group         = VADC_G1
	},
	{ // Channel 9
		.port          = XMC_GPIO_PORT2,
		.pin           = 4,
		.result_reg    = 4,
		.channel_num   = 6,
		.channel_alias = -1,
		.group_index   = 1,
		.group         = VADC_G1
	},
	{ // External Voltage Input
		.port          = XMC_GPIO_PORT2,
		.pin           = 10,
		.result_reg    = 13,
		.channel_num   = 2,
		.channel_alias = -1,
		.group_index   = 1,
		.group         = VADC_G1
	}, 
};

void current_init_adc(void) {
	for(uint8_t i = 0; i < CURRENT_NUM; i ++) {
		const XMC_GPIO_CONFIG_t input_pin_config = {
			.mode             = XMC_GPIO_MODE_INPUT_TRISTATE,
			.input_hysteresis = XMC_GPIO_INPUT_HYSTERESIS_STANDARD,
		};
		XMC_GPIO_Init(current[i].port, current[i].pin, &input_pin_config);
	}

	// This structure contains the Global related Configuration.
	const XMC_VADC_GLOBAL_CONFIG_t adc_global_config = {
		.boundary0 = (uint32_t) 0, // Lower boundary value for Normal comparison mode
		.boundary1 = (uint32_t) 0, // Upper boundary value for Normal comparison mode

		.class0 = {
			.sample_time_std_conv     = 31,                      // The Sample time is (2*tadci)
			.conversion_mode_standard = XMC_VADC_CONVMODE_12BIT, // 12bit conversion Selected

		},
		.class1 = {
			.sample_time_std_conv     = 31,                      // The Sample time is (2*tadci)
			.conversion_mode_standard = XMC_VADC_CONVMODE_12BIT, // 12bit conversion Selected

		},

		.data_reduction_control         = 0b11, // Accumulate 4 result values
		.wait_for_read_mode             = 0, // GLOBRES Register will not be overwritten until the previous value is read
		.event_gen_enable               = 0, // Result Event from GLOBRES is disabled
		.disable_sleep_mode_control     = 0  // Sleep mode is enabled
	};


	// Global iclass0 configuration
	const XMC_VADC_GLOBAL_CLASS_t adc_global_iclass_config = {
		.conversion_mode_standard = XMC_VADC_CONVMODE_12BIT,
		.sample_time_std_conv	  = 31,
	};

	// Global Result Register configuration structure
	XMC_VADC_RESULT_CONFIG_t adc_global_result_config = {
		.data_reduction_control = 0b11,  // Accumulate 4 result values
		.post_processing_mode   = XMC_VADC_DMM_REDUCTION_MODE,
		.wait_for_read_mode  	= 1, // Enabled
		.part_of_fifo       	= 0, // No FIFO
		.event_gen_enable   	= 0  // Disable Result event
	};

	// LLD Background Scan Init Structure
	const XMC_VADC_BACKGROUND_CONFIG_t adc_background_config = {
		.conv_start_mode   = XMC_VADC_STARTMODE_CIR,       // Conversion start mode selected as cancel inject repeat
		.req_src_priority  = XMC_VADC_GROUP_RS_PRIORITY_1, // Priority of the Background request source in the VADC module
		.trigger_signal    = XMC_VADC_REQ_TR_A,            // If Trigger needed then this denotes the Trigger signal
		.trigger_edge      = XMC_VADC_TRIGGER_EDGE_NONE,   // If Trigger needed then this denotes Trigger edge selected
		.gate_signal       = XMC_VADC_REQ_GT_A,			   // If Gating needed then this denotes the Gating signal
		.timer_mode        = 0,							   // Timer Mode Disabled
		.external_trigger  = 0,                            // Trigger is Disabled
		.req_src_interrupt = 0,                            // Background Request source interrupt Disabled
		.enable_auto_scan  = 1,
		.load_mode         = XMC_VADC_SCAN_LOAD_OVERWRITE
	};

	const XMC_VADC_GROUP_CONFIG_t group_init_handle = {
		.emux_config = {
			.stce_usage                  = 0, 					           // Use STCE when the setting changes
			.emux_mode                   = XMC_VADC_GROUP_EMUXMODE_SWCTRL, // Mode for Emux conversion
			.emux_coding                 = XMC_VADC_GROUP_EMUXCODE_BINARY, // Channel progression - binary format
			.starting_external_channel   = 0,                              // Channel starts at 0 for EMUX
			.connected_channel           = 0                               // Channel connected to EMUX
		},
		.class0 = {
			.sample_time_std_conv        = 31,                             // The Sample time is (2*tadci)
			.conversion_mode_standard    = XMC_VADC_CONVMODE_12BIT,        // 12bit conversion Selected
			.sampling_phase_emux_channel = 0,                              // The Sample time is (2*tadci)
			.conversion_mode_emux        = XMC_VADC_CONVMODE_12BIT         // 12bit conversion Selected
		},
		.class1 = {
			.sample_time_std_conv        = 31,                             // The Sample time is (2*tadci)
			.conversion_mode_standard    = XMC_VADC_CONVMODE_12BIT,        // 12bit conversion Selected
			.sampling_phase_emux_channel = 0,                              // The Sample time is (2*tadci)
			.conversion_mode_emux        = XMC_VADC_CONVMODE_12BIT         // 12bit conversion Selected
		},

		.boundary0                       = 0,                              // Lower boundary value for Normal comparison mode
		.boundary1	                     = 0,                              // Upper boundary value for Normal comparison mode
		.arbitration_round_length        = 0,                              // 4 arbitration slots per round selected (tarb = 4*tadcd) */
		.arbiter_mode                    = XMC_VADC_GROUP_ARBMODE_ALWAYS,  // Determines when the arbiter should run.
	};

	XMC_VADC_GLOBAL_Init(VADC, &adc_global_config);

	XMC_VADC_GROUP_Init(VADC_G0, &group_init_handle);
	XMC_VADC_GROUP_Init(VADC_G1, &group_init_handle);
	XMC_VADC_GROUP_SetPowerMode(VADC_G0, XMC_VADC_GROUP_POWERMODE_NORMAL);
	XMC_VADC_GROUP_SetPowerMode(VADC_G1, XMC_VADC_GROUP_POWERMODE_NORMAL);

	XMC_VADC_GLOBAL_SHS_EnableAcceleratedMode(SHS0, XMC_VADC_GROUP_INDEX_0);
	XMC_VADC_GLOBAL_SHS_EnableAcceleratedMode(SHS0, XMC_VADC_GROUP_INDEX_1);
	XMC_VADC_GLOBAL_SHS_SetAnalogReference(SHS0, XMC_VADC_GLOBAL_SHS_AREF_EXTERNAL_VDD_UPPER_RANGE);

	XMC_VADC_GLOBAL_StartupCalibration(VADC);

	// Initialize the Global Conversion class 0
	XMC_VADC_GLOBAL_InputClassInit(VADC, adc_global_iclass_config, XMC_VADC_GROUP_CONV_STD, 0);

	// Initialize the Background Scan hardware
	XMC_VADC_GLOBAL_BackgroundInit(VADC, &adc_background_config);

	// Initialize the global result register
	XMC_VADC_GLOBAL_ResultInit(VADC, &adc_global_result_config);

	for(uint8_t i = 0; i < CURRENT_NUM; i ++) {
		XMC_VADC_CHANNEL_CONFIG_t  channel_config = {
			.input_class                =  XMC_VADC_CHANNEL_CONV_GLOBAL_CLASS0,    // Global ICLASS 0 selected
			.lower_boundary_select 	    =  XMC_VADC_CHANNEL_BOUNDARY_GROUP_BOUND0,
			.upper_boundary_select 	    =  XMC_VADC_CHANNEL_BOUNDARY_GROUP_BOUND0,
			.event_gen_criteria         =  XMC_VADC_CHANNEL_EVGEN_NEVER,           // Channel Event disabled
			.sync_conversion  		    =  0,                                      // Sync feature disabled
			.alternate_reference        =  XMC_VADC_CHANNEL_REF_INTREF,            // Internal reference selected
			.result_reg_number          =  current[i].result_reg,                  // GxRES[10] selected
			.use_global_result          =  0, 				                       // Use Group result register
			.result_alignment           =  XMC_VADC_RESULT_ALIGN_RIGHT,            // Result alignment - Right Aligned
			.broken_wire_detect_channel =  XMC_VADC_CHANNEL_BWDCH_VAGND,           // No Broken wire mode select
			.broken_wire_detect		    =  0,    		                           // No Broken wire detection
			.bfl                        =  0,                                      // No Boundary flag
			.channel_priority           =  0,                   		           // Lowest Priority 0 selected
			.alias_channel              =  current[i].channel_alias                // Channel is Aliased
		};

		XMC_VADC_RESULT_CONFIG_t channel_result_config = {
			.data_reduction_control = 0b11,                         // Accumulate 4 result values
			.post_processing_mode   = XMC_VADC_DMM_REDUCTION_MODE,  // Use reduction mode
			.wait_for_read_mode  	= 1,                            // Enabled
			.part_of_fifo       	= 0 ,                           // No FIFO
			.event_gen_enable   	= 0                             // Disable Result event
		};


		// Initialize for configured channels
		XMC_VADC_GROUP_ChannelInit(current[i].group, current[i].channel_num, &channel_config);

		// Initialize for configured result registers
		XMC_VADC_GROUP_ResultInit(current[i].group, current[i].result_reg, &channel_result_config);

		XMC_VADC_GLOBAL_BackgroundAddChannelToSequence(VADC, current[i].group_index, current[i].channel_num);
	}

	XMC_VADC_GLOBAL_SetResultEventInterruptNode(VADC, XMC_VADC_SR_SHARED_SR0);
	XMC_VADC_GLOBAL_BackgroundTriggerConversion(VADC);
}

void current_init(void) {
	for(uint8_t i = 0; i < CURRENT_NUM; i++) {
		current[i].averaging_duration = 255;
	}
	current_calibration_read();
	current_init_adc();
}


void current_tick(void) {
	static uint32_t last_time_result[CURRENT_NUM] = {0};

	for(uint8_t i = 0; i < CURRENT_NUM; i++) {
		uint32_t result = XMC_VADC_GROUP_GetDetailedResult(current[i].group, current[i].result_reg);
		if(result & (1 << 31)) {
			current[i].result_sum += result & 0xFFFF;
			current[i].result_count++;
		}
	}

	for(uint8_t i = 0; i < CURRENT_NUM; i++) {
		if(system_timer_is_time_elapsed_ms(last_time_result[i], current[i].averaging_duration)) {
			if(i == 10) {
				// Resistor divider 1k:10k
				// mV = adc_value * 3300*11 / (4095*4) = 12100 / 5461
				current[i].result = current[i].result_sum * 12100 / (current[i].result_count*5461);
			} else {
				// mA = adc_value * 3300 / (4095*4) = 550 / 273
				const int32_t result = current[i].result_sum * 550 / (current[i].result_count*273);
				current[i].result = MAX(0, result + current[i].offset);
			}
				
			current[i].result_sum = 0;
			current[i].result_count = 0;

			last_time_result[i] = system_timer_get_ms();
		}
	}
}

void current_calibration_write(void) {
	uint32_t page[EEPROM_PAGE_SIZE/sizeof(uint32_t)];

	page[CURRENT_CALIBRATION_MAGIC_POS] = CURRENT_CALIBRATION_MAGIC;
	for(uint8_t i = 0; i < 10; i++) {
		page[CURRENT_CALIBRATION_OFFSET_POS + i] = current[i].offset + INT16_MAX;
	}

	bootloader_write_eeprom_page(CURRENT_CALIBRATION_PAGE, page);
}

void current_calibration_read(void) {
	uint32_t page[EEPROM_PAGE_SIZE/sizeof(uint32_t)];
	bootloader_read_eeprom_page(CURRENT_CALIBRATION_PAGE, page);

	// The magic number is not where it is supposed to be.
	if(page[CURRENT_CALIBRATION_MAGIC_POS] != CURRENT_CALIBRATION_MAGIC) {
		for(uint8_t i = 0; i < 10; i++) {
			current[i].offset = 0;
		}
		return;
	}

	for(uint8_t i = 0; i < 10; i++) {
		current[i].offset = page[CURRENT_CALIBRATION_OFFSET_POS + i] - INT16_MAX;
	}
}
