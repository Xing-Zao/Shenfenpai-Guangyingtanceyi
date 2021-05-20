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
 * @file         : \user\remoter.h
 * @author       : LonlyPan
 * @version      : V0.0.0
 * @date         : 2021-01-06 14:12
 * @brief        : 红外接收驱动
 * @attention    : 
 * @Modification : 初始版本
 * @LastEditTime : 2021-05-08 11:36
 * @History      : 
 *   1.Version: 
 *     Author:
 *     date:    
 *     Modification: 
 *   2.Version: 
 *     ......
 */
#ifndef REMOTE_H_
#define REMOTE_H_

#include "common.h"

#define RDATA   PAin(0)		//红外数据输入脚

//红外遥控识别码(ID),每款遥控器的该值基本都不一样,但也有一样的.
//我们选用的遥控器识别码为0
#define REMOTE_ID 0

extern u8 RmtCnt;	        //按键按下的次数

void Remote_Init(void);     //红外传感器接收头引脚初始化
u8 Remote_Scan(void);
void remote_isr();
#endif /* REMOTE_H_ */
