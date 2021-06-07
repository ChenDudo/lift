////////////////////////////////////////////////////////////////////////////////
#define _LCD_C_
////////////////////////////////////////////////////////////////////////////////
#if defined(__MDM2803) || defined(__MDM2803_N)
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include "mm32_types.h"

#include "common.h"
#include "hal_gpio.h"
#include "bsp_lcd.h" 
#include "lcd.h" 
#include "eth.h"
#include "com.h"
#include "device.h"
#include "main.h" 

/* WIDTH = 320, HEIGHT = 240 */

////////////////////////////////////////////////////////////////////////////////
void lcd_tick()
{
#if !defined(__MasterTest)
    static bool toggleFlag;
    if(lcdCnt1++ > 50){
        lcdCnt1 = 0;
        dispMyButton(myDev.up, myDev.dn);
    }
    if(lcdCnt2++ > 250){
        lcdCnt2 = 0;
        dispIdx(rxDev.id, Yellow);
        toggleFlag = !toggleFlag;
        toggleFlag ? dispButton(rxDev.up, rxDev.dn) : dispButton(0, 0);
    }
    if(lcdCnt3++ > 500){
        lcdCnt3 = 0;
        dispMyIdx(myDev.id);
        dispLED(phyA_Linked, phyB_Linked);
    }
#endif
}

////////////////////////////////////////////////////////////////////////////////
void dispLED(u8 phy1, u8 phy2)
{
    static u16 x1,x2,y1,y2;
    if (WIDTH > HEIGHT){
        y1 = 224;
        x1 = 20;
        x2 = 270;
        y2 = y1;
    }
    else {
        x1 = (WIDTH - 30)/2;
        x2 = x1;
        y1 = 2;
        y2 = 302;
    }
    phy1 ? drawRec (x1,  y1 , 30,  16, Green) : drawRec (x1,  y1 , 30,  16, Red);
    phy2 ? drawRec (x2,  y2 , 30,  16, Green) : drawRec (x2,  y2 , 30,  16, Red);
}

////////////////////////////////////////////////////////////////////////////////
void dispButton(u8 upflag, u8 dnflag)
{
    u16 c1 = upflag ? Yellow : Black;
    u16 c2 = dnflag ? Yellow : Black;
    if(WIDTH > HEIGHT){
        drawTriangle(292, HEIGHT/2-10, 20, 30, 0, c1);
        drawTriangle(292, HEIGHT/2+10, 20, 30, 1, c2);
    }
    else{
        drawTriangle(WIDTH - 40, HEIGHT/2-25, 20, 20, 0, c1);
        drawTriangle(WIDTH - 40, HEIGHT/2+25, 20, 20, 1, c2);
    }
}

////////////////////////////////////////////////////////////////////////////////
void dispMyButton(u8 upflag, u8 dnflag)
{
    u16 c1 = upflag ? White : Black;
    u16 c2 = dnflag ? White : Black;
    if(WIDTH > HEIGHT){
        drawTriangle(WIDTH/2-32-30, 40, 20, 30, 0, c1);
        drawTriangle(WIDTH/2+32+10, 10, 20, 30, 1, c2);
    }
    else{
        //drawTriangle(55, 47, 20, 37, 0, c1);
        //drawTriangle(160,10, 20, 37, 1, c2);
        drawTriangle(WIDTH/2-40, HEIGHT/2-10, 80, 100, 0, c1);
        drawTriangle(WIDTH/2-40, HEIGHT/2+10, 80, 100, 1, c2);
    }
}

////////////////////////////////////////////////////////////////////////////////
void dispIdx(u16 idx, u16 c)
{
    if(WIDTH > HEIGHT)
        drawNum(72, 66, 1, 2, idx, c);
    else {
        drawNum(WIDTH - 64, (HEIGHT-32)/2, 1, 0, idx, c);
    }
}

////////////////////////////////////////////////////////////////////////////////
void dispMyIdx(u16 idx)
{
    if(WIDTH > HEIGHT)
        drawNum(WIDTH/2-32, 10, 1, 0, idx, LightGrey);
    else
        drawNum(0, (HEIGHT-32)/2, 1, 0, idx, White);
}

////////////////////////////////////////////////////////////////////////////////
void clearLeftScreen()
{
	drawRec (dx , dy , dw, dh + 95, Black);
}

////////////////////////////////////////////////////////////////////////////////
void clearAllScreen()
{
	drawRec (0 , 0 , WIDTH, HEIGHT, Black);
}

////////////////////////////////////////////////////////////////////////////////
void clearButtomScreen()
{
	drawRec (dx , dy + dh + 95 , dw, 240, Black);
}

////////////////////////////////////////////////////////////////////////////////
void dispScreen()
{
	text.fore = White;
	text.back = Black;
}

////////////////////////////////////////////////////////////////////////////////
u16 getColor(u8 c)
{
	u16 colorTab[] = { 0x0000, 0x000F, 0x03E0, 0x03EF, 0x7800, 0x780F, 0x7BE0, 
    0xC618, 0x7BEF, 0x001F, 0x07E0, 0x07FF, 0xF800, 0xF81F, 0xFFE0, 0xFFFF};
	return colorTab[c];
}

////////////////////////////////////////////////////////////////////////////////
void BSP_LCD_Configure()
{
    initGPIO_LCD();
	initFSMC();
    
	LCDC_Init_Reg();
	lcdFillColor(Black);
	lcdBlcH();
    clearAllScreen();
}

#endif
