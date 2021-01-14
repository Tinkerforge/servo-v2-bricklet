/* servo-v2-bricklet
 * Copyright (C) 2020 Olaf Lüke <olaf@tinkerforge.com>
 *
 * communication.h: TFP protocol message handling
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

#ifndef COMMUNICATION_H
#define COMMUNICATION_H

#include <stdint.h>
#include <stdbool.h>

#include "bricklib2/protocols/tfp/tfp.h"
#include "bricklib2/bootloader/bootloader.h"

// Default functions
BootloaderHandleMessageResponse handle_message(const void *data, void *response);
void communication_tick(void);
void communication_init(void);

// Constants

#define SERVO_V2_BOOTLOADER_MODE_BOOTLOADER 0
#define SERVO_V2_BOOTLOADER_MODE_FIRMWARE 1
#define SERVO_V2_BOOTLOADER_MODE_BOOTLOADER_WAIT_FOR_REBOOT 2
#define SERVO_V2_BOOTLOADER_MODE_FIRMWARE_WAIT_FOR_REBOOT 3
#define SERVO_V2_BOOTLOADER_MODE_FIRMWARE_WAIT_FOR_ERASE_AND_REBOOT 4

#define SERVO_V2_BOOTLOADER_STATUS_OK 0
#define SERVO_V2_BOOTLOADER_STATUS_INVALID_MODE 1
#define SERVO_V2_BOOTLOADER_STATUS_NO_CHANGE 2
#define SERVO_V2_BOOTLOADER_STATUS_ENTRY_FUNCTION_NOT_PRESENT 3
#define SERVO_V2_BOOTLOADER_STATUS_DEVICE_IDENTIFIER_INCORRECT 4
#define SERVO_V2_BOOTLOADER_STATUS_CRC_MISMATCH 5

#define SERVO_V2_STATUS_LED_CONFIG_OFF 0
#define SERVO_V2_STATUS_LED_CONFIG_ON 1
#define SERVO_V2_STATUS_LED_CONFIG_SHOW_HEARTBEAT 2
#define SERVO_V2_STATUS_LED_CONFIG_SHOW_STATUS 3

// Function and callback IDs and structs
#define FID_GET_STATUS 1
#define FID_SET_ENABLE 2
#define FID_GET_ENABLED 3
#define FID_SET_POSITION 4
#define FID_GET_POSITION 5
#define FID_GET_CURRENT_POSITION 6
#define FID_GET_CURRENT_VELOCITY 7
#define FID_SET_MOTION_CONFIGURATION 8
#define FID_GET_MOTION_CONFIGURATION 9
#define FID_SET_PULSE_WIDTH 10
#define FID_GET_PULSE_WIDTH 11
#define FID_SET_DEGREE 12
#define FID_GET_DEGREE 13
#define FID_SET_PERIOD 14
#define FID_GET_PERIOD 15
#define FID_GET_SERVO_CURRENT 16
#define FID_SET_SERVO_CURRENT_CONFIGURATION 17
#define FID_GET_SERVO_CURRENT_CONFIGURATION 18
#define FID_SET_INPUT_VOLTAGE_CONFIGURATION 19
#define FID_GET_INPUT_VOLTAGE_CONFIGURATION 20
#define FID_GET_OVERALL_CURRENT 21
#define FID_GET_INPUT_VOLTAGE 22
#define FID_SET_CURRENT_CALIBRATION 23
#define FID_GET_CURRENT_CALIBRATION 24
#define FID_SET_POSITION_REACHED_CALLBACK_CONFIGURATION 25
#define FID_GET_POSITION_REACHED_CALLBACK_CONFIGURATION 26

#define FID_CALLBACK_POSITION_REACHED 27

typedef struct {
	TFPMessageHeader header;
} __attribute__((__packed__)) GetStatus;

typedef struct {
	TFPMessageHeader header;
	uint8_t enabled[2];
	int16_t current_position[10];
	int16_t current_velocity[10];
	uint16_t current[10];
	uint16_t input_voltage;
} __attribute__((__packed__)) GetStatus_Response;

typedef struct {
	TFPMessageHeader header;
	uint16_t servo_channel;
	bool enable;
} __attribute__((__packed__)) SetEnable;

typedef struct {
	TFPMessageHeader header;
	uint16_t servo_channel;
} __attribute__((__packed__)) GetEnabled;

typedef struct {
	TFPMessageHeader header;
	bool enable;
} __attribute__((__packed__)) GetEnabled_Response;

typedef struct {
	TFPMessageHeader header;
	uint16_t servo_channel;
	int16_t position;
} __attribute__((__packed__)) SetPosition;

typedef struct {
	TFPMessageHeader header;
	uint16_t servo_channel;
} __attribute__((__packed__)) GetPosition;

typedef struct {
	TFPMessageHeader header;
	int16_t position;
} __attribute__((__packed__)) GetPosition_Response;

typedef struct {
	TFPMessageHeader header;
	uint16_t servo_channel;
} __attribute__((__packed__)) GetCurrentPosition;

typedef struct {
	TFPMessageHeader header;
	int16_t position;
} __attribute__((__packed__)) GetCurrentPosition_Response;

typedef struct {
	TFPMessageHeader header;
	uint16_t servo_channel;
} __attribute__((__packed__)) GetCurrentVelocity;

typedef struct {
	TFPMessageHeader header;
	uint16_t velocity;
} __attribute__((__packed__)) GetCurrentVelocity_Response;

typedef struct {
	TFPMessageHeader header;
	uint16_t servo_channel;
	uint32_t velocity;
	uint32_t acceleration;
	uint32_t deceleration;
} __attribute__((__packed__)) SetMotionConfiguration;

typedef struct {
	TFPMessageHeader header;
	uint16_t servo_channel;
} __attribute__((__packed__)) GetMotionConfiguration;

typedef struct {
	TFPMessageHeader header;
	uint32_t velocity;
	uint32_t acceleration;
	uint32_t deceleration;
} __attribute__((__packed__)) GetMotionConfiguration_Response;

typedef struct {
	TFPMessageHeader header;
	uint16_t servo_channel;
	uint32_t min;
	uint32_t max;
} __attribute__((__packed__)) SetPulseWidth;

typedef struct {
	TFPMessageHeader header;
	uint16_t servo_channel;
} __attribute__((__packed__)) GetPulseWidth;

typedef struct {
	TFPMessageHeader header;
	uint32_t min;
	uint32_t max;
} __attribute__((__packed__)) GetPulseWidth_Response;

typedef struct {
	TFPMessageHeader header;
	uint16_t servo_channel;
	int16_t min;
	int16_t max;
} __attribute__((__packed__)) SetDegree;

typedef struct {
	TFPMessageHeader header;
	uint16_t servo_channel;
} __attribute__((__packed__)) GetDegree;

typedef struct {
	TFPMessageHeader header;
	int16_t min;
	int16_t max;
} __attribute__((__packed__)) GetDegree_Response;

typedef struct {
	TFPMessageHeader header;
	uint16_t servo_channel;
	uint32_t period;
} __attribute__((__packed__)) SetPeriod;

typedef struct {
	TFPMessageHeader header;
	uint16_t servo_channel;
} __attribute__((__packed__)) GetPeriod;

typedef struct {
	TFPMessageHeader header;
	uint32_t period;
} __attribute__((__packed__)) GetPeriod_Response;

typedef struct {
	TFPMessageHeader header;
	uint16_t servo_channel;
} __attribute__((__packed__)) GetServoCurrent;

typedef struct {
	TFPMessageHeader header;
	uint16_t current;
} __attribute__((__packed__)) GetServoCurrent_Response;

typedef struct {
	TFPMessageHeader header;
	uint16_t servo_channel;
	uint8_t averaging_duration;
} __attribute__((__packed__)) SetServoCurrentConfiguration;

typedef struct {
	TFPMessageHeader header;
	uint16_t servo_channel;
} __attribute__((__packed__)) GetServoCurrentConfiguration;

typedef struct {
	TFPMessageHeader header;
	uint8_t averaging_duration;
} __attribute__((__packed__)) GetServoCurrentConfiguration_Response;

typedef struct {
	TFPMessageHeader header;
	uint8_t averaging_duration;
} __attribute__((__packed__)) SetInputVoltageConfiguration;

typedef struct {
	TFPMessageHeader header;
} __attribute__((__packed__)) GetInputVoltageConfiguration;

typedef struct {
	TFPMessageHeader header;
	uint8_t averaging_duration;
} __attribute__((__packed__)) GetInputVoltageConfiguration_Response;

typedef struct {
	TFPMessageHeader header;
} __attribute__((__packed__)) GetOverallCurrent;

typedef struct {
	TFPMessageHeader header;
	uint16_t current;
} __attribute__((__packed__)) GetOverallCurrent_Response;

typedef struct {
	TFPMessageHeader header;
} __attribute__((__packed__)) GetInputVoltage;

typedef struct {
	TFPMessageHeader header;
	uint16_t voltage;
} __attribute__((__packed__)) GetInputVoltage_Response;

typedef struct {
	TFPMessageHeader header;
	int16_t offset[10];
} __attribute__((__packed__)) SetCurrentCalibration;

typedef struct {
	TFPMessageHeader header;
} __attribute__((__packed__)) GetCurrentCalibration;

typedef struct {
	TFPMessageHeader header;
	int16_t offset[10];
} __attribute__((__packed__)) GetCurrentCalibration_Response;

typedef struct {
	TFPMessageHeader header;
	uint16_t servo_channel;
	bool enabled;
} __attribute__((__packed__)) SetPositionReachedCallbackConfiguration;

typedef struct {
	TFPMessageHeader header;
	uint16_t servo_channel;
} __attribute__((__packed__)) GetPositionReachedCallbackConfiguration;

typedef struct {
	TFPMessageHeader header;
	bool enabled;
} __attribute__((__packed__)) GetPositionReachedCallbackConfiguration_Response;

typedef struct {
	TFPMessageHeader header;
	uint16_t servo_channel;
	int16_t position;
} __attribute__((__packed__)) PositionReached_Callback;


// Function prototypes
BootloaderHandleMessageResponse get_status(const GetStatus *data, GetStatus_Response *response);
BootloaderHandleMessageResponse set_enable(const SetEnable *data);
BootloaderHandleMessageResponse get_enabled(const GetEnabled *data, GetEnabled_Response *response);
BootloaderHandleMessageResponse set_position(const SetPosition *data);
BootloaderHandleMessageResponse get_position(const GetPosition *data, GetPosition_Response *response);
BootloaderHandleMessageResponse get_current_position(const GetCurrentPosition *data, GetCurrentPosition_Response *response);
BootloaderHandleMessageResponse get_current_velocity(const GetCurrentVelocity *data, GetCurrentVelocity_Response *response);
BootloaderHandleMessageResponse set_motion_configuration(const SetMotionConfiguration *data);
BootloaderHandleMessageResponse get_motion_configuration(const GetMotionConfiguration *data, GetMotionConfiguration_Response *response);
BootloaderHandleMessageResponse set_pulse_width(const SetPulseWidth *data);
BootloaderHandleMessageResponse get_pulse_width(const GetPulseWidth *data, GetPulseWidth_Response *response);
BootloaderHandleMessageResponse set_degree(const SetDegree *data);
BootloaderHandleMessageResponse get_degree(const GetDegree *data, GetDegree_Response *response);
BootloaderHandleMessageResponse set_period(const SetPeriod *data);
BootloaderHandleMessageResponse get_period(const GetPeriod *data, GetPeriod_Response *response);
BootloaderHandleMessageResponse get_servo_current(const GetServoCurrent *data, GetServoCurrent_Response *response);
BootloaderHandleMessageResponse set_servo_current_configuration(const SetServoCurrentConfiguration *data);
BootloaderHandleMessageResponse get_servo_current_configuration(const GetServoCurrentConfiguration *data, GetServoCurrentConfiguration_Response *response);
BootloaderHandleMessageResponse set_input_voltage_configuration(const SetInputVoltageConfiguration *data);
BootloaderHandleMessageResponse get_input_voltage_configuration(const GetInputVoltageConfiguration *data, GetInputVoltageConfiguration_Response *response);
BootloaderHandleMessageResponse get_overall_current(const GetOverallCurrent *data, GetOverallCurrent_Response *response);
BootloaderHandleMessageResponse get_input_voltage(const GetInputVoltage *data, GetInputVoltage_Response *response);
BootloaderHandleMessageResponse set_current_calibration(const SetCurrentCalibration *data);
BootloaderHandleMessageResponse get_current_calibration(const GetCurrentCalibration *data, GetCurrentCalibration_Response *response);
BootloaderHandleMessageResponse set_position_reached_callback_configuration(const SetPositionReachedCallbackConfiguration *data);
BootloaderHandleMessageResponse get_position_reached_callback_configuration(const GetPositionReachedCallbackConfiguration *data, GetPositionReachedCallbackConfiguration_Response *response);

// Callbacks
bool handle_position_reached_callback(void);

#define COMMUNICATION_CALLBACK_TICK_WAIT_MS 1
#define COMMUNICATION_CALLBACK_HANDLER_NUM 1
#define COMMUNICATION_CALLBACK_LIST_INIT \
	handle_position_reached_callback, \


#endif
