/* servo-v2-bricklet
 * Copyright (C) 2019 Olaf Lüke <olaf@tinkerforge.com>
 *
 * communication.c: TFP protocol message handling
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

#include "communication.h"

#include <stdint.h>
#include <stdbool.h>

#include "bricklib2/utility/communication_callback.h"
#include "bricklib2/protocols/tfp/tfp.h"

#include "pwm.h"
#include "current.h"

#define SERVO_BIT_MODE (1 << 15)

typedef struct {
	uint8_t servo[PWM_NUM];
	uint8_t length;
} ServoChannels;


BootloaderHandleMessageResponse handle_message(const void *message, void *response) {
	switch(tfp_get_fid_from_message(message)) {
		case FID_GET_STATUS: return get_status(message, response);
		case FID_SET_ENABLE: return set_enable(message);
		case FID_GET_ENABLED: return get_enabled(message, response);
		case FID_SET_POSITION: return set_position(message);
		case FID_GET_POSITION: return get_position(message, response);
		case FID_GET_CURRENT_POSITION: return get_current_position(message, response);
		case FID_GET_CURRENT_VELOCITY: return get_current_velocity(message, response);
		case FID_SET_MOTION_CONFIGURATION: return set_motion_configuration(message);
		case FID_GET_MOTION_CONFIGURATION: return get_motion_configuration(message, response);
		case FID_SET_PULSE_WIDTH: return set_pulse_width(message);
		case FID_GET_PULSE_WIDTH: return get_pulse_width(message, response);
		case FID_SET_DEGREE: return set_degree(message);
		case FID_GET_DEGREE: return get_degree(message, response);
		case FID_SET_PERIOD: return set_period(message);
		case FID_GET_PERIOD: return get_period(message, response);
		case FID_GET_SERVO_CURRENT: return get_servo_current(message, response);
		case FID_SET_SERVO_CURRENT_CONFIGURATION: return set_servo_current_configuration(message);
		case FID_GET_SERVO_CURRENT_CONFIGURATION: return get_servo_current_configuration(message, response);
		case FID_SET_INPUT_VOLTAGE_CONFIGURATION: return set_input_voltage_configuration(message);
		case FID_GET_INPUT_VOLTAGE_CONFIGURATION: return get_input_voltage_configuration(message, response);
		case FID_GET_OVERALL_CURRENT: return get_overall_current(message, response);
		case FID_GET_INPUT_VOLTAGE: return get_input_voltage(message, response);
		case FID_CALIBRATE_SERVO_CURRENT: return calibrate_servo_current(message);
		default: return HANDLE_MESSAGE_RESPONSE_NOT_SUPPORTED;
	}
}

ServoChannels* get_servo_channels(const uint16_t channel) {
	static ServoChannels sc;
	sc.length = 0;

	if(channel & SERVO_BIT_MODE) {
		for(uint8_t i = 0; i < PWM_NUM; i++) {
			if(channel & (1 << i)) {
				sc.servo[sc.length] = i;
				sc.length++;
			}
		}
	} else if(channel < PWM_NUM) {
		sc.servo[0] = channel;
		sc.length   = 1;
	}

	return &sc;
}

BootloaderHandleMessageResponse get_status(const GetStatus *data, GetStatus_Response *response) {
	response->header.length = sizeof(GetStatus_Response);
	response->enabled[0]    = 0;
	response->enabled[1]    = 0;

	for(uint8_t i = 0; i < PWM_NUM; i++) {
		response->current[i]          = current[i].result;
		response->current_position[i] = pwm[i].current_position;
		response->current_velocity[i] = pwm[i].current_velocity;
		if(i < 8) {
			response->enabled[0]     |= (pwm[i].enabled << i);
		} else {
			response->enabled[1]     |= (pwm[i].enabled << (i - 8));
		}
	}
		
	response->input_voltage = current[INPUT_VOLTAGE_INDEX].result;

	return HANDLE_MESSAGE_RESPONSE_NEW_MESSAGE;
}

BootloaderHandleMessageResponse set_enable(const SetEnable *data) {
	ServoChannels *sc = get_servo_channels(data->servo_channel);
	if(sc->length == 0) {
		return HANDLE_MESSAGE_RESPONSE_INVALID_PARAMETER;
	}
	
	for(uint16_t i = 0; i < sc->length; i++) {
		const uint8_t channel    = sc->servo[i];
		pwm[channel].enabled     = data->enable;
		pwm[channel].new_enabled = true;
	}

	return HANDLE_MESSAGE_RESPONSE_EMPTY;
}

BootloaderHandleMessageResponse get_enabled(const GetEnabled *data, GetEnabled_Response *response) {
	if(data->servo_channel >= PWM_NUM) {
		return HANDLE_MESSAGE_RESPONSE_INVALID_PARAMETER;
	}

	response->header.length = sizeof(GetEnabled_Response);
	response->enable        = pwm[data->servo_channel].enabled;

	return HANDLE_MESSAGE_RESPONSE_NEW_MESSAGE;
}

BootloaderHandleMessageResponse set_position(const SetPosition *data) {
	ServoChannels *sc = get_servo_channels(data->servo_channel);
	if(sc->length == 0) {
		return HANDLE_MESSAGE_RESPONSE_INVALID_PARAMETER;
	}
	
	for(uint16_t i = 0; i < sc->length; i++) {
		const uint8_t channel     = sc->servo[i];
		pwm[channel].position     = data->position;
		pwm[channel].new_position = true;
	}

	return HANDLE_MESSAGE_RESPONSE_EMPTY;
}

BootloaderHandleMessageResponse get_position(const GetPosition *data, GetPosition_Response *response) {
	if(data->servo_channel >= PWM_NUM) {
		return HANDLE_MESSAGE_RESPONSE_INVALID_PARAMETER;
	}

	response->header.length = sizeof(GetPosition_Response);
	response->position      = pwm[data->servo_channel].position;

	return HANDLE_MESSAGE_RESPONSE_NEW_MESSAGE;
}

BootloaderHandleMessageResponse get_current_position(const GetCurrentPosition *data, GetCurrentPosition_Response *response) {
	if(data->servo_channel >= PWM_NUM) {
		return HANDLE_MESSAGE_RESPONSE_INVALID_PARAMETER;
	}

	response->header.length = sizeof(GetCurrentPosition_Response);
	response->position      = pwm[data->servo_channel].current_position;

	return HANDLE_MESSAGE_RESPONSE_NEW_MESSAGE;
}

BootloaderHandleMessageResponse get_current_velocity(const GetCurrentVelocity *data, GetCurrentVelocity_Response *response) {
	if(data->servo_channel >= PWM_NUM) {
		return HANDLE_MESSAGE_RESPONSE_INVALID_PARAMETER;
	}

	response->header.length = sizeof(GetCurrentVelocity_Response);
	response->velocity      = pwm[data->servo_channel].current_velocity;

	return HANDLE_MESSAGE_RESPONSE_NEW_MESSAGE;
}

BootloaderHandleMessageResponse set_motion_configuration(const SetMotionConfiguration *data) {
	ServoChannels *sc = get_servo_channels(data->servo_channel);
	if(sc->length == 0) {
		return HANDLE_MESSAGE_RESPONSE_INVALID_PARAMETER;
	}
	
	for(uint16_t i = 0; i < sc->length; i++) {
		const uint8_t channel     = sc->servo[i];
		pwm[channel].velocity     = data->velocity;
		pwm[channel].acceleration = data->acceleration;
		pwm[channel].deceleration = data->deceleration;
		pwm[channel].new_motion   = true;
	}

	return HANDLE_MESSAGE_RESPONSE_EMPTY;
}

BootloaderHandleMessageResponse get_motion_configuration(const GetMotionConfiguration *data, GetMotionConfiguration_Response *response) {
	if(data->servo_channel >= PWM_NUM) {
		return HANDLE_MESSAGE_RESPONSE_INVALID_PARAMETER;
	}

	response->header.length = sizeof(GetMotionConfiguration_Response);
	response->acceleration  = pwm[data->servo_channel].acceleration;
	response->deceleration  = pwm[data->servo_channel].deceleration;
	response->velocity      = pwm[data->servo_channel].velocity;

	return HANDLE_MESSAGE_RESPONSE_NEW_MESSAGE;
}

BootloaderHandleMessageResponse set_pulse_width(const SetPulseWidth *data) {
	ServoChannels *sc = get_servo_channels(data->servo_channel);
	if(sc->length == 0) {
		return HANDLE_MESSAGE_RESPONSE_INVALID_PARAMETER;
	}
	
	for(uint16_t i = 0; i < sc->length; i++) {
		const uint8_t channel        = sc->servo[i];
		pwm[channel].pulse_width_max = data->max;
		pwm[channel].pulse_width_min = data->min;
		pwm[channel].new_pulse_width = true;
	}

	return HANDLE_MESSAGE_RESPONSE_EMPTY;
}

BootloaderHandleMessageResponse get_pulse_width(const GetPulseWidth *data, GetPulseWidth_Response *response) {
	if(data->servo_channel >= PWM_NUM) {
		return HANDLE_MESSAGE_RESPONSE_INVALID_PARAMETER;
	}

	response->header.length = sizeof(GetPulseWidth_Response);
	response->max           = pwm[data->servo_channel].pulse_width_max;
	response->min           = pwm[data->servo_channel].pulse_width_min;

	return HANDLE_MESSAGE_RESPONSE_NEW_MESSAGE;
}

BootloaderHandleMessageResponse set_degree(const SetDegree *data) {
	ServoChannels *sc = get_servo_channels(data->servo_channel);
	if(sc->length == 0) {
		return HANDLE_MESSAGE_RESPONSE_INVALID_PARAMETER;
	}
	
	for(uint16_t i = 0; i < sc->length; i++) {
		const uint8_t channel   = sc->servo[i];
		pwm[channel].degree_max = data->max;
		pwm[channel].degree_min = data->min;
		pwm[channel].new_degree = true;
	}

	return HANDLE_MESSAGE_RESPONSE_EMPTY;
}

BootloaderHandleMessageResponse get_degree(const GetDegree *data, GetDegree_Response *response) {
	if(data->servo_channel >= PWM_NUM) {
		return HANDLE_MESSAGE_RESPONSE_INVALID_PARAMETER;
	}

	response->header.length = sizeof(GetDegree_Response);
	response->max           = pwm[data->servo_channel].degree_max;
	response->min           = pwm[data->servo_channel].degree_min;

	return HANDLE_MESSAGE_RESPONSE_NEW_MESSAGE;
}

BootloaderHandleMessageResponse set_period(const SetPeriod *data) {
	if(data->period > 1000000) {
		return HANDLE_MESSAGE_RESPONSE_INVALID_PARAMETER;
	}

	ServoChannels *sc = get_servo_channels(data->servo_channel);
	if(sc->length == 0) {
		return HANDLE_MESSAGE_RESPONSE_INVALID_PARAMETER;
	}
	
	for(uint16_t i = 0; i < sc->length; i++) {
		const uint8_t channel   = sc->servo[i];
		pwm[channel].period     = data->period;
		pwm[channel].new_period = true;
	}

	return HANDLE_MESSAGE_RESPONSE_EMPTY;
}

BootloaderHandleMessageResponse get_period(const GetPeriod *data, GetPeriod_Response *response) {
	if(data->servo_channel >= PWM_NUM) {
		return HANDLE_MESSAGE_RESPONSE_INVALID_PARAMETER;
	}

	response->header.length = sizeof(GetPeriod_Response);
	response->period        = pwm[data->servo_channel].period;

	return HANDLE_MESSAGE_RESPONSE_NEW_MESSAGE;
}

BootloaderHandleMessageResponse get_servo_current(const GetServoCurrent *data, GetServoCurrent_Response *response) {
	if(data->servo_channel >= PWM_NUM) {
		return HANDLE_MESSAGE_RESPONSE_INVALID_PARAMETER;
	}

	response->header.length = sizeof(GetServoCurrent_Response);
	response->current       = current[data->servo_channel].result;

	return HANDLE_MESSAGE_RESPONSE_NEW_MESSAGE;
}

BootloaderHandleMessageResponse set_servo_current_configuration(const SetServoCurrentConfiguration *data) {
	ServoChannels *sc = get_servo_channels(data->servo_channel);
	if(sc->length == 0) {
		return HANDLE_MESSAGE_RESPONSE_INVALID_PARAMETER;
	}

	for(uint16_t i = 0; i < sc->length; i++) {
		const uint8_t channel                   = sc->servo[i];
		current[channel].averaging_duration     = data->averaging_duration;
		current[channel].new_averaging_duration = true;
	}

	return HANDLE_MESSAGE_RESPONSE_EMPTY;
}

BootloaderHandleMessageResponse get_servo_current_configuration(const GetServoCurrentConfiguration *data, GetServoCurrentConfiguration_Response *response) {
	if(data->servo_channel >= PWM_NUM) {
		return HANDLE_MESSAGE_RESPONSE_INVALID_PARAMETER;
	}

	response->header.length      = sizeof(GetServoCurrentConfiguration_Response);
	response->averaging_duration = current[data->servo_channel].averaging_duration;

	return HANDLE_MESSAGE_RESPONSE_NEW_MESSAGE;
}

BootloaderHandleMessageResponse set_input_voltage_configuration(const SetInputVoltageConfiguration *data) {
	current[INPUT_VOLTAGE_INDEX].averaging_duration     = data->averaging_duration;
	current[INPUT_VOLTAGE_INDEX].new_averaging_duration = true;

	return HANDLE_MESSAGE_RESPONSE_EMPTY;
}

BootloaderHandleMessageResponse get_input_voltage_configuration(const GetInputVoltageConfiguration *data, GetInputVoltageConfiguration_Response *response) {
	response->header.length      = sizeof(GetInputVoltageConfiguration_Response);
	response->averaging_duration = current[INPUT_VOLTAGE_INDEX].averaging_duration;

	return HANDLE_MESSAGE_RESPONSE_NEW_MESSAGE;
}

BootloaderHandleMessageResponse get_overall_current(const GetOverallCurrent *data, GetOverallCurrent_Response *response) {
	response->header.length = sizeof(GetOverallCurrent_Response);
	response->current       = 0;
	for(uint8_t i = 0; i < PWM_NUM; i++) {
		response->current  += current[i].result;
	}

	return HANDLE_MESSAGE_RESPONSE_NEW_MESSAGE;
}

BootloaderHandleMessageResponse get_input_voltage(const GetInputVoltage *data, GetInputVoltage_Response *response) {
	response->header.length = sizeof(GetInputVoltage_Response);
	response->voltage       = current[INPUT_VOLTAGE_INDEX].result;

	return HANDLE_MESSAGE_RESPONSE_NEW_MESSAGE;
}

BootloaderHandleMessageResponse calibrate_servo_current(const CalibrateServoCurrent *data) {

	return HANDLE_MESSAGE_RESPONSE_EMPTY;
}





void communication_tick(void) {
	communication_callback_tick();
}

void communication_init(void) {
	communication_callback_init();
}
