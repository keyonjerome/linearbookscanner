/*
 * motor_control.h
 *
 *  Created on: Feb 7, 2025
 *      Author: keyon
 */

#include "cmsis_os.h"
#include "stm32f4xx_hal.h"

#ifndef INC_MOTOR_CONTROL_H_
#define INC_MOTOR_CONTROL_H_


#define MOTOR_PULSE_PIN GPIO_PIN_0
#define MOTOR_DIR_PIN GPIO_PIN_1
#define MOTOR_ENABLE_PIN GPIO_PIN_2

// Constants
#define STEPS_PER_REVOLUTION 200 // For a 1.8° step angle motor
#define MICROSTEPPING 16         // TB6600 microstepping setting (e.g., 1/16)
#define TOTAL_STEPS (STEPS_PER_REVOLUTION * MICROSTEPPING) // Total steps per revolution

void motor_setup(void);
void step_motor(uint32_t steps, uint32_t delay);
void go_to_angle(float target_angle, uint32_t delay);


#endif /* INC_MOTOR_CONTROL_H_ */
