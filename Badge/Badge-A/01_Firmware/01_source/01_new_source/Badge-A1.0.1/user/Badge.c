/**
 * �������� Firmware
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
 * @brief        : ����������
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

// ͼƬ��ʾy��ƫ�ƣ�������Ǻ���Ļװ��ԭ��ͼƬ���Ƕ�����ʾ
const u8 pic_y_offset = 3;

// ��ǰ���  true������  false������
static bool curret_identity = true;  

// flash�е�ͼƬ�ļ�������
char*const IMG_NAME[11]=
{
	"4068_gre.bin",             // 4068���� ��ɫ
	"reading_gre.bin",          // ��Ϣ��ȡ������ ��ɫ
	"identity_true_gre.bin",    // ���ʶ��ɹ����� ��ɫ
	"changing_gre.bin",         // ����޸������� ��ɫ
	"change_true_gre.bin",      // ����޸ĳɹ����� ��ɫ
	"4068_blu.bin",             
	"reading_blu.bin",
	"identity_true_blu.bin",
	"changing_blu.bin",
	"change_true_blu.bin",
	"dianzan_blu.bin",          // �����ղ�Ͷ�� ��ɫ
};

/**
 * @brief: ��ʾ�û��Զ������ƻ�����ͼƬ
 * @note: ���ƻ�ͼƬֻ��ͬʱ��ʾһ��
 *        �Զ������������ļ� name.txt����Ҫ�û��Լ��޸ģ�Ϊ������ʾ
 */
void show_name()
{
	u8 name_buf[128] = " ";  // �ַ����飬���ڴ�Ŵ��Զ������ƣ�< 10���ַ���������Ļ��ʾ����
	UINT num =1;             // �������ʼ������ֵ�ᱣ�����һ��ֵ�����Ǿֲ�������ʵ�ʲ⾹�ᱣ��ֵ��������
	u16 i = 0;               // ����Ԫ������
	/* Register the file system object to the FatFs module */
	if(f_mount(&USERFatFS, "0:", 0) == FR_OK)
	{
		//* Create and Open a new text file object with write access */
		if(f_open(&USERFile, "name.txt", FA_READ) == FR_OK) // FA_WRITE
		{
			 f_lseek(&USERFile,0);  // ��λ���ļ�ͷ
			 while(num != 0){ // ��ȡ�ļ�����ֱ������
				 f_read(&USERFile,name_buf+i,1,&num);
				 i++;
			 }
			 f_close(&USERFile); // �����ر��ļ�
		}
		f_mount(NULL, "0:", 0);
	}
	if(i > 1){  // �ļ����ݲ�Ϊ��
        /************* ���Ҫ��ʾ�Զ������ƣ�ȡ���������ݵ�ע�ͣ���ע�͵����һ�д��� ******************/
		// if(curret_identity)                                  // ������
		// 	LCD_Fill(79,42,320,70,0x371F);		   				// �ַ��������Ϊ��ɫ
		// else
		// 	LCD_Fill(79,42,320,70,0x77EC);		   				// �ַ��������Ϊ��ɫ
        // Show_Str(79,42,240,38,name_buf,32,1);                // ��ʾ�ַ�

		LCD_Flash_Pic(0,pic_y_offset,320,176,IMG_NAME[10]);
	}
}

/**
 * @brief: תΪ����
 * @note: ���ݶ������ݣ�չʾ�ӳ����Ϊ�����������ʾ�仯
 */
void change_to_up()
{
	if(!curret_identity){
		LCD_Flash_Pic(0,pic_y_offset,320,176,IMG_NAME[3]);  // ����޸��� ��ɫ
		HAL_Delay(300);
		LCD_Flash_Pic(0,pic_y_offset,320,176,IMG_NAME[9]);  // ����޸ĳɹ� ��ɫ
		set_led_status(RGB_BLU);
		curret_identity = true;
		LCD_Flash_Pic(0,pic_y_offset,320,176,IMG_NAME[5]);  // 4068 ��ɫ
	}
}
/**
 * @brief: תΪ����
 * @note: ���ݶ������ݣ�չʾ�������Ϊ�����������ʾ�仯
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
 * @brief: ����ң�ش���
 * @note: ��ʹ��ң�ظ������ƽ�����ʾ���ݣ���ͬң�ض�Ӧ�ļ�ֵ���ܻ᲻ͬ������������
 * @param {u8} key ����ң�ؼ�ֵ
 */
void remote_change_pic(u8 key)
{
    static u8 last_key = 0;
	if(last_key != key)  // �ظ�������Ч
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

// ��ʼ��
void setup()
{
	LCD_Init();
	__HAL_SPI_ENABLE(&hspi1);
	HAL_TIM_IC_Start_IT(&htim2,TIM_CHANNEL_1);   // ���벶���ж�
	__HAL_TIM_ENABLE_IT(&htim2,TIM_IT_UPDATE);   // ��ʱ���ж�
	HAL_TIM_Base_Start_IT(&htim10);              // LED ģ��PWM��ʱ��
	W25QXX_Init();                               // W25Q256��ʼ��
	while(W25QXX_ReadID()!=W25Q256)				 // W25Q256���
	{
		LCD_ShowString(50,50,200,35,16,(u8 *)"W25Q64 Check Failed!");
	}
	HAL_Delay(1000);
	LCD_Flash_Pic(0,pic_y_offset,320,176,IMG_NAME[5]);  // ��ʾͼƬ ����
	set_led_status(RGB_BLU);                            // ���� LED ״̬ ��ɫ��ƥ��ͼƬ��ɫ
	show_name();                                        // ��ʾ�Զ�������
}

// ��ѭ��
void loop()
{
	remote_change_pic(Remote_Scan());
}
