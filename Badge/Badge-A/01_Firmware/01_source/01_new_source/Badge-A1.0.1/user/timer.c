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
 * @file         : \user\timer.c
 * @author       : LonlyPan
 * @version      : V0.0.0
 * @date         : 2021-04-21 16:29
 * @brief        : 定时器外设驱动
 *                 红外接收、RGB灯定时管理
 * @attention    : // 注意事项
 */
#include "common.h"
#include "remote.h"
#include "led.h"
/* 中断回调函数 */

//定时器更新（溢出）中断回调函数
void HAL_TIM_PeriodElapsedCallback(TIM_HandleTypeDef *htim)
{
	if(htim->Instance==TIM2)
	{
		remote_isr();
	}
	else if(htim->Instance==TIM10)
	{
		led_isr();
	}
}


