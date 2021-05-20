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
 * @file         : \user\text.h
 * @author       : LonlyPan
 * @version      : V0.0.0
 * @date         : 2021-04-06 14:12
 * @brief        : 汉字显示
 * @attention    : 依赖 lcd.h
 * @Modification : 初始版本
 * @LastEditTime : 2021-05-08 11:37
 * @History      : 
 *   1.Version: 
 *     Author:
 *     date:    
 *     Modification: 
 *   2.Version: 
 *     ......
 */

#ifndef TEXT_H_
#define TEXT_H_
#include "common.h"


void Get_HzMat(unsigned char *code,unsigned char *mat,u8 size);			//得到汉字的点阵码
void Show_Font(u16 x,u16 y,u8 *font,u8 size,u8 mode);					//在指定位置显示一个汉字
void Show_Str(u16 x,u16 y,u16 width,u16 height,u8 *str,u8 size,u8 mode);	//在指定位置显示一个字符串
void Show_Str_Mid(u16 x,u16 y,u8 *str,u8 size,u8 len);


#endif /* TEXT_H_ */
