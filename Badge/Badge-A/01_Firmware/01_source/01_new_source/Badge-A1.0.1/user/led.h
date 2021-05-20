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
 * @Modification : 初始版本
 * @LastEditTime : 2021-05-11 14:03
 * @History      : 
 *   1.Version: 
 *     Author:
 *     date:    
 *     Modification: 
 *   2.Version: 
 *     ......
 */
#ifndef LED_H_
#define LED_H_

#include "common.h"


/** @brief  红绿双色灯（状态指示灯） 状态 */
enum LED_RGB_STATUS{
	RGB_OFF, // 熄灭
    RGB_RED,    // 红色常量
	RGB_GRE,    // 绿色常量
	RGB_BLU     // 蓝色常量
};

#define RED_PIN PBout(14)  //
#define GRE_PIN PBout(13)  //
#define BLU_PIN PBout(15)  //


/** @brief  红绿双色灯（状态指示灯）管理 */
void led_isr();

void set_led_status(u8 status);
void set_led_value(u8 status, u8 value);
#endif /* LED_H_ */
