/*
 * motor_control.c
 *
 *  Created on: Feb 7, 2025
 *      Author: keyon
 */

#include <motor_control.h>



void motor_setup() {
	  // Enable motor
	  HAL_GPIO_WritePin(GPIOA, MOTOR_ENABLE_PIN, GPIO_PIN_SET);
	  // Set direction (clockwise)
	  HAL_GPIO_WritePin(GPIOA, MOTOR_DIR_PIN, GPIO_PIN_SET);

	  // Disable motor
	  HAL_GPIO_WritePin(GPIOA, MOTOR_ENABLE_PIN, GPIO_PIN_RESET);
}


void step_motor(uint32_t steps, uint32_t delay) {
    for (uint32_t i = 0; i < steps; i++) {
        HAL_GPIO_WritePin(GPIOA, MOTOR_PULSE_PIN, GPIO_PIN_SET); // Step high
        HAL_Delay(delay);
        HAL_GPIO_WritePin(GPIOA, MOTOR_PULSE_PIN, GPIO_PIN_RESET); // Step low
        HAL_Delay(delay);
    }
}

// Function to move the motor to a specific angle
void go_to_angle(float target_angle, uint32_t delay) {
    // Calculate the number of steps required
    uint32_t steps = (uint32_t)((target_angle / 360.0) * TOTAL_STEPS);

    // Set direction (clockwise or counterclockwise)
    if (target_angle > 0) {
        HAL_GPIO_WritePin(GPIOA, MOTOR_DIR_PIN, GPIO_PIN_SET); // Clockwise
    } else {
        HAL_GPIO_WritePin(GPIOA, MOTOR_DIR_PIN, GPIO_PIN_RESET); // Counterclockwise
    }

    // Step the motor
    step_motor(steps, delay);
}
//
//static void MX_GPIO_Init(void)
//{
///* USER CODE BEGIN MX_GPIO_Init_1 */
///* USER CODE END MX_GPIO_Init_1 */
//    GPIO_InitTypeDef GPIO_InitStruct = {0};
//  /* GPIO Ports Clock Enable */
//    __HAL_RCC_GPIOA_CLK_ENABLE();
//
//    GPIO_InitStruct.Pin = GPIO_PIN_0 | GPIO_PIN_1 | GPIO_PIN_2 ;
//    GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
//    GPIO_InitStruct.Pull = GPIO_NOPULL;
//    GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
//    HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);
//
//
///* USER CODE BEGIN MX_GPIO_Init_2 */
///* USER CODE END MX_GPIO_Init_2 */
//}
