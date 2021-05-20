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
 * @file         : Badge.c
 * @author       : LonlyPan
 * @version      : V1.0.1
 * @date         : 2020-12-21 16:29
 * @brief        : 胸牌主程序
 * @attention    : 
 */
#include <remote.h>
#include "lcd.h"
#include "Badge.h"
#include "main.h"
#include "stdbool.h"
#include "spi.h"
#include "w25qxx.h"
#include "fatfs.h"
#include "text.h"
#include "text.h"
#include "tim.h"
#include "led.h"

// 图片显示y轴偏移，由于外壳和屏幕装配原因，图片不是顶格显示
const u8 pic_y_offset = 3;

// 当前身份  true：上民  false：尘民
static bool curret_identity = true;  

// flash中的图片文件名索引
char*const IMG_NAME[11]=
{
	"4068_gre.bin",             // 4068字样 绿色
	"reading_gre.bin",          // 信息读取中字样 绿色
	"identity_true_gre.bin",    // 身份识别成功字样 绿色
	"changing_gre.bin",         // 身份修改中字样 绿色
	"change_true_gre.bin",      // 身份修改成功字样 绿色
	"4068_blu.bin",             
	"reading_blu.bin",
	"identity_true_blu.bin",
	"changing_blu.bin",
	"change_true_blu.bin",
	"dianzan_blu.bin",          // 点赞收藏投币 蓝色
};

/**
 * @brief: 显示用户自定义名称或三连图片
 * @note: 名称或图片只能同时显示一个
 *        自定义名称来自文件 name.txt，需要用户自己修改，为空则不显示
 */
void show_name()
{
	u8 name_buf[128] = " ";  // 字符数组，用于存放从自定义名称，< 10个字符，多了屏幕显示不下
	UINT num =1;             // 如果不初始化，该值会保存最后一次值（不是局部变量吗？实际测竟会保留值。。。）
	u16 i = 0;               // 数组元素索引
	/* Register the file system object to the FatFs module */
	if(f_mount(&USERFatFS, "0:", 0) == FR_OK)
	{
		//* Create and Open a new text file object with write access */
		if(f_open(&USERFile, "name.txt", FA_READ) == FR_OK) // FA_WRITE
		{
			 f_lseek(&USERFile,0);  // 定位到文件头
			 while(num != 0){ // 读取文件内容直到结束
				 f_read(&USERFile,name_buf+i,1,&num);
				 i++;
			 }
			 f_close(&USERFile); // 读完后关闭文件
		}
		f_mount(NULL, "0:", 0);
	}
	if(i > 1){  // 文件内容不为空
        /************* 如果要显示自定义名称，取消下面内容的注释，并注释掉最后一行代码 ******************/
		// if(curret_identity)                                  // 检测身份
		// 	LCD_Fill(79,42,320,70,0x371F);		   				// 字符背景填充为蓝色
		// else
		// 	LCD_Fill(79,42,320,70,0x77EC);		   				// 字符背景填充为绿色
        // Show_Str(79,42,240,38,name_buf,32,1);                // 显示字符

		LCD_Flash_Pic(0,pic_y_offset,320,176,IMG_NAME[10]);
	}
}

/**
 * @brief: 转为上民
 * @note: 根据动漫内容，展示从尘民变为上民的胸牌显示变化
 */
void change_to_up()
{
	if(!curret_identity){
		LCD_Flash_Pic(0,pic_y_offset,320,176,IMG_NAME[3]);  // 身份修改中 蓝色
		HAL_Delay(300);
		LCD_Flash_Pic(0,pic_y_offset,320,176,IMG_NAME[9]);  // 身份修改成功 蓝色
		set_led_status(RGB_BLU);
		curret_identity = true;
		LCD_Flash_Pic(0,pic_y_offset,320,176,IMG_NAME[5]);  // 4068 蓝色
	}
}
/**
 * @brief: 转为尘民
 * @note: 根据动漫内容，展示从上民变为尘民的胸牌显示变化
 */
void change_to_down()
{
	if(curret_identity){
		LCD_Flash_Pic(0,pic_y_offset,320,176,IMG_NAME[8]);
		HAL_Delay(300);
		LCD_Flash_Pic(0,pic_y_offset,320,176,IMG_NAME[4]);
		set_led_status(RGB_GRE);
		curret_identity = false;
		LCD_Flash_Pic(0,pic_y_offset,320,176,IMG_NAME[0]);
	}
}

/**
 * @brief: 红外遥控处理
 * @note: 可使用遥控更改胸牌界面显示内容，不同遥控对应的键值可能会不同，需自行适配
 * @param {u8} key 红外遥控键值
 */
void remote_change_pic(u8 key)
{
    static u8 last_key = 0;
	if(last_key != key)  // 重复按键无效
	{
		last_key = key;
		switch(key)
		{
			case 0:;break;  //ERRO
			case 162:LCD_Flash_Pic(0,pic_y_offset,320,176,IMG_NAME[0]);break;  // 1
			case 98:LCD_Flash_Pic(0,pic_y_offset,320,176,IMG_NAME[1]);break;   // 2
			case 226:LCD_Flash_Pic(0,pic_y_offset,320,176,IMG_NAME[2]);break;  // 3
			case 34:LCD_Flash_Pic(0,pic_y_offset,320,176,IMG_NAME[3]);break;   // 4
			case 2:LCD_Flash_Pic(0,pic_y_offset,320,176,IMG_NAME[4]);break;    // 5
			case 194:LCD_Flash_Pic(0,pic_y_offset,320,176,IMG_NAME[5]);break;  // 6
			case 224:LCD_Flash_Pic(0,pic_y_offset,320,176,IMG_NAME[6]);break;  // 7
			case 168:LCD_Flash_Pic(0,pic_y_offset,320,176,IMG_NAME[7]);break;  // 8
			case 144:LCD_Flash_Pic(0,pic_y_offset,320,176,IMG_NAME[8]);break;  // 9
			case 104:LCD_Flash_Pic(0,pic_y_offset,320,176,IMG_NAME[10]);break; // *
			case 152:LCD_Flash_Pic(0,pic_y_offset,320,176,IMG_NAME[9]);break;  // 0
			case 176:;break;                                                   // #
			case 24:change_to_up();break;                                      // up
			case 16: ; break;                                                  // LEFT
			case 90: ; break;                                                  // RIGHT
			case 74:change_to_down();break;                                    // down
			case 56:show_name();break;                                         // ok
		}
	}

}

// 初始化
void setup()
{
	LCD_Init();
	__HAL_SPI_ENABLE(&hspi1);
	HAL_TIM_IC_Start_IT(&htim2,TIM_CHANNEL_1);   // 输入捕获中断
	__HAL_TIM_ENABLE_IT(&htim2,TIM_IT_UPDATE);   // 定时器中断
	HAL_TIM_Base_Start_IT(&htim10);              // LED 模拟PWM定时器
	W25QXX_Init();                               // W25Q256初始化
	while(W25QXX_ReadID()!=W25Q256)				 // W25Q256检测
	{
		LCD_ShowString(50,50,200,35,16,(u8 *)"W25Q64 Check Failed!");
	}
	HAL_Delay(1000);
	LCD_Flash_Pic(0,pic_y_offset,320,176,IMG_NAME[5]);  // 显示图片 上民
	set_led_status(RGB_BLU);                            // 设置 LED 状态 蓝色，匹配图片主色
	show_name();                                        // 显示自定义名称
}

// 主循环
void loop()
{
	remote_change_pic(Remote_Scan());
}
