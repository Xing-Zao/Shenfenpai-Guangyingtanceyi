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
 */

#include "text.h"
#include "w25qxx.h"
#include "lcd.h"
#include "text.h"
#include "string.h"
#include  "fatfs.h"
#include  "stdio.h"
//code 字符指针开始
//从字库中查找出字模
//code 字符串的开始地址,GBK码
//mat  数据存放地址 (size/8+((size%8)?1:0))*(size) bytes大小
//size:字体大小


void Get_HzMat(unsigned char *code,unsigned char *mat,u8 size)
{
	 char path[20];
	UINT num;

	unsigned char qh,ql;
	unsigned char i;
	unsigned long foffset;
	u16 csize=(size/8+((size%8)?1:0))*(size);//得到字体一个字符对应点阵集所占的字节数
	qh=*code;
	ql=*(++code);
	if(qh<0x81||ql<0x40||ql==0xff||qh==0xff)//非 常用汉字
	{
	    for(i=0;i<csize;i++)*mat++=0x00;//填充满格
	    return; //结束访问
	}
	if(ql<0x7f)ql-=0x40;//注意!
	else ql-=0x41;
	qh-=0x81;
	foffset=((unsigned long)190*qh+ql)*csize;	//得到字库中的字节偏移量
	sprintf(path,"GBK%d.FON",size);

	/* Register the file system object to the FatFs module */
	if(f_mount(&USERFatFS, "0:", 0) == FR_OK)
	{
		//* Create and Open a new text file object with write access */
		if(f_open(&USERFile, path, FA_READ) == FR_OK) // FA_WRITE  identity_true_blue.bin
		{
			 f_lseek(&USERFile,foffset);
			 f_read(&USERFile,mat,csize,&num);
			f_close(&USERFile); // 读完后关闭点阵字库文件
		}
		f_mount(NULL, "0:", 0);
	}
}
//显示一个指定大小的汉字
//x,y :汉字的坐标
//font:汉字GBK码
//size:字体大小
//mode:0,正常显示,1,叠加显示
void Show_Font(u16 x,u16 y,u8 *font,u8 size,u8 mode)
{
	u8 temp,t,t1;
	u16 y0=y;
	u8 dzk[512];
	u16 csize=(size/8+((size%8)?1:0))*(size);//得到字体一个字符对应点阵集所占的字节数
	if(size!=12&&size!=16&&size!=24&&size!=32)return;	//不支持的size
	Get_HzMat(font,dzk,size);	//得到相应大小的点阵数据
	for(t=0;t<csize;t++)
	{
		temp=dzk[t];			//得到点阵数据
		for(t1=0;t1<8;t1++)
		{
			if(temp&0x80)LCD_Fast_DrawPoint(x,y,POINT_COLOR);
			else if(mode==0)LCD_Fast_DrawPoint(x,y,BACK_COLOR);
			temp<<=1;
			y++;
			if((y-y0)==size)
			{
				y=y0;
				x++;
				break;
			}
		}
	}
}

//在指定位置开始显示一个字符串
//支持自动换行
//(x,y):起始坐标
//width,height:区域
//str  :字符串
//size :字体大小
//mode:0,非叠加方式;1,叠加方式
void Show_Str(u16 x,u16 y,u16 width,u16 height,u8*str,u8 size,u8 mode)
{

	u16 x0=x;
	u16 y0=y;
    u8 bHz=0;     //字符或者中文
    while(*str!=0)//数据未结束
    {
        if(!bHz)
        {
	        if(*str>0x80)bHz=1;//中文
	        else              //字符
	        {
                if(x>(x0+width-size/2))//换行
				{
					y+=size;
					x=x0;
				}
		        if(y>(y0+height-size))break;//越界返回
		        if(*str==13)//换行符号
		        {
		            y+=size;
					x=x0;
		            str++;
		        }
		        else LCD_ShowChar(x,y,*str,size,mode);//有效部分写入
				str++;
		        x+=size/2; //字符,为全字的一半
	        }
        }else//中文
        {
            bHz=0;//有汉字库
            if(x>(x0+width-size))//换行
			{
				y+=size;
				x=x0;
			}
	        if(y>(y0+height-size))break;//越界返回
	        Show_Font(x,y,str,size,mode); //显示这个汉字,空心显示
	        str+=2;
	        x+=size;//下一个汉字偏移
        }
    }

}
//在指定宽度的中间显示字符串
//如果字符长度超过了len,则用Show_Str显示
//len:指定要显示的宽度
void Show_Str_Mid(u16 x,u16 y,u8*str,u8 size,u8 len)
{
	u16 strlenth=0;
   	strlenth=strlen((const char*)str);
	strlenth*=size/2;
	if(strlenth>len)Show_Str(x,y,lcddev.width,lcddev.height,str,size,1);
	else
	{
		strlenth=(len-strlenth)/2;
	    Show_Str(strlenth+x,y,lcddev.width,lcddev.height,str,size,1);
	}
}





