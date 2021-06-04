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
        dispIdx(rxDev.id);
    }
    if(lcdCnt2++ > 250){
        lcdCnt2 = 0;
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
    static u16 x1,x2,y1;
    if (WIDTH > HEIGHT){
        y1 = 224;
        x1 = 20;
        x2 = 270;
    }
    else {
        y1 = 304;
        x1 = 10;
        x2 = 200;
    }
    phy1 ? drawRec (x1,  y1 , 30,  16, Green) : drawRec (x1,  y1 , 30,  16, Red);
    phy2 ? drawRec (x2,  y1 , 30,  16, Green) : drawRec (x2,  y1 , 30,  16, Red);
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
        drawTriangle(220, HEIGHT/2-10, 20, 30, 0, c1);
        drawTriangle(220, HEIGHT/2+20, 20, 30, 1, c2);
    }
}

////////////////////////////////////////////////////////////////////////////////
void dispMyButton(u8 upflag, u8 dnflag)
{
    u16 c1 = upflag ? LightGrey : Black;
    u16 c2 = dnflag ? LightGrey : Black;
    if(WIDTH > HEIGHT){
        drawTriangle(WIDTH/2-32-30, 40, 20, 30, 0, c1);
        drawTriangle(WIDTH/2+32+10, 10, 20, 30, 1, c2);
    }
    else{
        drawTriangle(55, 47, 20, 37, 0, c1);
        drawTriangle(160,10, 20, 37, 1, c2);
    }
}

////////////////////////////////////////////////////////////////////////////////
void dispIdx(u16 idx)
{
    if(WIDTH > HEIGHT)
        drawNum(72, 66, 1, 2, idx, White);
    else
        drawNum(32, (HEIGHT-88)/2, 1, 2, idx, White);
}

////////////////////////////////////////////////////////////////////////////////
void dispMyIdx(u16 idx)
{
    if(WIDTH > HEIGHT)
        drawNum(WIDTH/2-32, 10, 1, 0, idx, LightGrey);
    else
        drawNum(88, 10, 1, 0, idx, LightGrey);
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
