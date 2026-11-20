/*
 * servo.h
 *
 *  Created on: Nov 19, 2026
 *      Author: dorit
 */

#ifndef SERVO_H_
#define SERVO_H_

#include <stdint.h>

#define NUM_SERVOS 4

void servo_init(void);
void servo_set_us(uint8_t ch, uint16_t us);

void servo_move(uint8_t ch, uint16_t target_us, uint16_t duration_ms);
uint8_t servo_is_moving(uint8_t ch);
void servo_tick(void);


#endif /* INC_SERVO_H_ */
