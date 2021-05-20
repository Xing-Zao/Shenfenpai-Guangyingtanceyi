/**
 * 灵笼胸牌 Firmware
 * Copyright (c) 2020-2021 LonlyPan. All rights reserved.
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <https://www.gnu.org/licenses/>.
 *
 */
/*
 * @file         : \user\led.c
 * @author       : LonlyPan
 * @version      : V0.0.0
 * @date         : 2021-04-21 16:29
 * @brief        : RGB灯驱动
 * @attention    : 软件PWM
 */
#include "led.h"

// macros
#define BREATHE_PWM_MIN  2           // pwm最小值，0最亮，可能会刺眼
#define BREATHE_PWM_MAX  256         // pwm最小值，256灭灯，>200 led已经变化不明显

#define SOFT_PWM_SCALE 1

u8 red_pwm_high_cycle = 0;  // 0-255
u8 gre_pwm_high_cycle = 0;  // 0-255
u8 blu_pwm_high_cycle = 0;  // 0-255

/** @brief  红绿双色灯（状态指示灯）管理 */
void led_isr()
{
    static uint8_t pwm_count  = 0;
    // avoid multiple loads of pwm_count

    RED_PIN = red_pwm_high_cycle > pwm_count ? 1 : 0;                                                                                                                                                                              RED_PIN = red_pwm_high_cycle > pwm_count ? 1 : 0;
    GRE_PIN = gre_pwm_high_cycle > pwm_count ? 1 : 0;
    BLU_PIN = blu_pwm_high_cycle > pwm_count ? 1 : 0;
	// SOFT_PWM_SCALE to frequency:
	//
	// 0: 16000000/64/256/128 =   7.6294 Hz
	// 1:                / 64 =  15.2588 Hz
	// 2:                / 32 =  30.5176 Hz
	// 3:                / 16 =  61.0352 Hz
	// 4:                /  8 = 122.0703 Hz
	// 5:                /  4 = 244.1406 Hz
	pwm_count += SOFT_PWM_SCALE;
	if (pwm_count >= 255) {
		pwm_count = 0;
	}
}
/** @brief  红绿双色灯（状态指示灯）管理 */
void set_led_value(u8 status, u8 value)
{
   switch(status){
   	   case RGB_RED:
   		red_pwm_high_cycle = value;  // 0-255
   		break;
   	   case RGB_GRE:
   		gre_pwm_high_cycle = value;  // 0-255
   		break;
   	   case RGB_BLU:
   		blu_pwm_high_cycle = value;  // 0-255
   		break;
   	  default:
   		break;
   };
}

/** @brief  红绿双色灯（状态指示灯）管理 */
void set_led_status(u8 status)
{
   switch(status){
   	   case RGB_OFF:
   		red_pwm_high_cycle = 0;  // 0-255
   		gre_pwm_high_cycle = 0;  // 0-255
   		blu_pwm_high_cycle = 0;  // 0-255
   		break;
   	   case RGB_RED:
   		red_pwm_high_cycle = 255;  // 0-255
   		gre_pwm_high_cycle = 0;  // 0-255
   		blu_pwm_high_cycle = 0;  // 0-255
   		break;
   	   case RGB_GRE:
   		red_pwm_high_cycle = 80;  // 0-255
   		gre_pwm_high_cycle = 255;  // 0-255
   		blu_pwm_high_cycle = 0;  // 0-255
   		break;
   	   case RGB_BLU:
   		red_pwm_high_cycle = 15;  // 0-255
   		gre_pwm_high_cycle = 50;  // 0-255
   		blu_pwm_high_cycle = 150;  // 0-255
   		break;
   	  default:
   		red_pwm_high_cycle = 0;  // 0-255
   		gre_pwm_high_cycle = 0;  // 0-255
   		blu_pwm_high_cycle = 0;  // 0-255
   		break;
   };
}
