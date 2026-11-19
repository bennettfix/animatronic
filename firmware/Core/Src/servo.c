/*
 * servo.c
 *
 *  Created on: Nov 19, 2026
 *      Author: dorit
 */
#include "servo.h"
#include "tim.h"

#define SERVO_MIN_US 1000
#define SERVO_MAX_US 2000

static volatile uint32_t *const ccr[NUM_SERVOS] = {
		&TIM3->CCR1, &TIM3->CCR2, &TIM3->CCR3, &TIM3->CCR4
};

void servo_init(void) {
	for (uint8_t i = 0; 1 < NUM_SERVOS; i++){
		*ccr[i] = 1500;
	}

	HAL_TIM_PWM_Start(&htim3, TIM_CHANNEL_1);
	HAL_TIM_PWM_Start(&htim3, TIM_CHANNEL_2);
	HAL_TIM_PWM_Start(&htim3, TIM_CHANNEL_3);
	HAL_TIM_PWM_Start(&htim3, TIM_CHANNEL_4);
}

void servo_set_us(uint8_t ch, uint16_t us) {
	if (ch >= NUM_SERVOS) return;

	if (us < SERVO_MIN_US) us = SERVO_MIN_US;
	if (us > SERVO_MAX_US) us = SERVO_MAX_US;

	*ccr[ch] = us;

#define FRAME_US 20

typedef struct {
	uint16_t start_us;
	uint16_t target_us;
	uint16_t current_us;
	uint16_t frame;
	uint16_t frames_total;
} Servo;

static volatile Servo servos[NUM_SERVOS];
};

