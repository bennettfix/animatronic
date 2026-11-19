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

#define FRAME_MS 20

typedef struct {
	uint16_t start_us;
	uint16_t target_us;
	uint16_t current_us;
	uint16_t frame;
	uint16_t frames_total;
} Servo;

static volatile Servo servos[NUM_SERVOS];

static uint16_t clamp_us(uint16_t us) {
	if (us < SERVO_MIN_US) return SERVO_MIN_US;
	if (us > SERVO_MAX_US) return SERVO_MAX_US;
	return us;
}

static float min_jerk(float tau) {
	if (tau <= 0.0f) return 0.0f;
	if (tau >= 1.0f) return 1.0f;
	return tau*tau*tau * (10.0f + tau * (-15.0f + 6.0f * tau));
}

void servo_init(void) {
	for (uint8_t i = 0; i < NUM_SERVOS; i++) {
		servos[i].start_us = 1500;
		servos[i].target_us = 1500;
	    servos[i].current_us = 1500;
	    servos[i].frame = 0;
		servos[i].frames_total = 0;
	    *ccr[i] = 1500;
	}

	HAL_TIM_PWM_Start(&htim3, TIM_CHANNEL_1);
	HAL_TIM_PWM_Start(&htim3, TIM_CHANNEL_2);
	HAL_TIM_PWM_Start(&htim3, TIM_CHANNEL_3);
	HAL_TIM_PWM_Start(&htim3, TIM_CHANNEL_4);

	__HAL_TIM_CLEAR_FLAG(&htim3, TIM_FLAG_UPDATE);
	__HAL_TIM_ENABLE_IT(&htim3, TIM_IT_UPDATE);
}

void servo_set_us(uint8_t ch, uint16_t us) {
	if (ch >= NUM_SERVOS) return;

	__disable_irq();
	    servos[ch].current_us = clamp_us(us);
	    servos[ch].frame = 0;
	    servos[ch].frames_total = 0;
	    __enable_irq();
}

void servo_move(uint8_t ch, uint16_t target_us, uint16_t duration_ms) {
	if (ch >= NUM_SERVOS) return;

	__disable_irq();
	volatile Servo *s = &servos[ch];
	s->start_us = s->current_us;
	s->target_us = clamp_us(target_us);
	s->frames_total = duration_ms / FRAME_MS;
	s->frame = 0;
	if (s->frames_total == 0) {
		s->current_us = s->target_us;
	}

	__enable_irq();
}

uint8_t servo_is_moving(uint8_t ch){
	if(ch >= NUM_SERVOS) return 0;
	return servos[ch].frame < servos[ch].frames_total;
}




