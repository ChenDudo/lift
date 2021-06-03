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
    if(lcdCnt1++ >= 50){
        lcdCnt1 = 0;
        dispMyButton(myDev.up, myDev.dn);
    }
    if(lcdCnt2++ >= 250){
        lcdCnt2 = 0;
        dispLED(phyA_Linked, phyB_Linked);
        toggleFlag ? dispButton(rxDev.up, rxDev.dn) : dispButton(0, 0);
        toggleFlag = !toggleFlag;
    }
    if(lcdCnt3++ >= 500){
        lcdCnt3 = 0;
        dispIdx(rxDev.id);
        dispMyIdx(myDev.id);
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
void drawMM(u16 x, u16 y, u16 w)
{
    u16 color = SkyBlue;
    u8 xsub = 1, ysub = 1;
    
    drawLine(x+0*w, y+1*w, x+1*w, y+0*w, color); //M0 - M1
    drawLine(x+1*w, y+0*w, x+2*w-xsub, y+0*w, color); //M1 - M2
    drawLine(x+2*w-xsub, y+0*w, x+2*w-xsub, y+1*w+ysub, color); //M2 - M3
    drawLine(x+1*w, y+2*w, x+2*w, y+1*w, color); //M3 - M4
    drawLine(x+1*w, y+2*w, x+2*w, y+3*w, color); //M4 - M5
    drawLine(x+2*w, y+3*w, x+3*w-xsub, y+2*w+ysub, color); //M5 - M6
    drawLine(x+3*w-xsub, y+2*w+ysub, x+4*w-xsub, y+2*w+ysub, color); //M6 - M7
    drawLine(x+4*w-xsub, y+2*w+ysub, x+4*w-xsub, y+3*w+ysub, color); //M7 - M8
    drawLine(x+3*w, y+4*w, x+4*w-xsub, y+3*w+ysub, color); //M8 - M9
    drawLine(x+3*w, y+4*w, x+1*w, y+4*w, color); //M9 - M10
    drawLine(x+1*w, y+4*w, x+1*w, y+3*w, color); //M10 - M11
    drawLine(x+1*w, y+3*w, x+0*w, y+3*w, color); //M11 - M12
    drawLine(x+0*w, y+3*w, x+0*w, y+1*w, color); //M12 - M0
    
    color = Yellow;
    drawLine(x+2*w+xsub, y+1*w-ysub, x+3*w, y+0*w, color); //M0 - M1
    drawLine(x+3*w, y+0*w, x+5*w, y+0*w, color); //M1 - M2
    drawLine(x+5*w, y+0*w, x+5*w, y+1*w, color); //M2 - M3
    drawLine(x+5*w, y+1*w, x+6*w, y+1*w, color); //M3 - M4
    drawLine(x+6*w, y+1*w, x+6*w, y+3*w, color); //M4 - M5
    drawLine(x+5*w, y+4*w, x+6*w, y+3*w, color); //M5 - M6
    drawLine(x+4*w+xsub, y+4*w, x+5*w, y+4*w, color); //M6 - M7
    drawLine(x+4*w+xsub, y+4*w, x+4*w+xsub, y+3*w-ysub, color); //M7 - M8
    drawLine(x+4*w+xsub, y+3*w-ysub, x+5*w, y+2*w, color); //M8 - M9
    drawLine(x+4*w, y+1*w, x+5*w, y+2*w, color); //M9 - M10
    drawLine(x+3*w+xsub, y+2*w-ysub, x+4*w, y+1*w, color); //M10 - M11
    drawLine(x+2*w+xsub, y+2*w-ysub, x+3*w+xsub, y+2*w-ysub, color); //M11 - M12
    drawLine(x+2*w+xsub, y+2*w-ysub, x+2*w+xsub, y+1*w-ysub, color); //M12 - M0
    
    putStr(x+6*w, y+0*w+2, 2, 1, "MindMotion");
    putStr(x+6*w+3, y+1*w, 0, 1, "Soc Solutions");
    
    text.fore = White;
    
#if defined(__MasterTest)
    putStr(205, y, 2, 1, "Total Floor");
#else
    putStr(205, y, 2, 1, "Current Floor");
#endif
    
    drawLine(198, 225, 315, 225, White);
    putStr(205, 230, 1, 1, "MindMotion Nanjing");
    
}

////////////////////////////////////////////////////////////////////////////////
void randRefresh()
{
    u16 x,y,w,h,c;
    drawSquare(dx,  dy,  dw, dh, SPACE, NUL);
	if (drawBlockCnt++ % 2) {
		x = rand();		x %= (dw - 2);
		y = rand();		y %= (dh - 2);
		w = rand();		w %= DMAX;
		h = rand();		h %= DMAX;
		c = rand(); c &= 0x0f;
		if ((x + w) > (dw - 2)) x = dw - w - 2;
		if ((y + h) > (dh - 2)) y = dh - h - 2;
		drawRec (x + dx + 1, y + dy + 1, w, h, getColor(c));
	}
	else {
		c = rand(); c &= 0x0f;
		drawRec (dx + 1, dy + 1, dw - 2, dh - 2, getColor(c));
	}
}


////////////////////////////////////////////////////////////////////////////////
void initLcdDemo()
{
	text.fore = SkyBlue;
	text.back = Black;
	color.c1 = Blue;
    
	drawSquare(dx,  dy,  dw, dh, SPACE, NUL);
	color.c1 = LightGrey;
	color.c2 = DarkCyan;
	color.c3 = White;
	color.c4 = Yellow;
    
	drawCircle(60,  195, 30, Yellow);
	drawCircle(190, 195, 30, Yellow);
	
	drawSquare(10,  160, 40, 25, SPACE, NUL);
	drawSquare(10,  210, 40, 25, FRAME, NUL);
    
	drawSquare(105, 160, 40, 25, FRAME, NUL);
	drawSquare(105, 210, 40, 25, SPACE, NUL);
	
	drawSquare(205,  160, 40, 25, SPACE, NUL);
	drawSquare(205,  210, 40, 25, FRAME, NUL);
    
	drawLine (10, 160, 50, 185, Red);
	drawLine (205, 185, 245, 160, Red);
}

////////////////////////////////////////////////////////////////////////////////
void refreshLCD(u8 sele,  u16* flag)
{
	if (!*flag) return; 
	*flag = false;
	text.back = DarkCyan;
	
	switch (sele) {
        default:
		break;
	}
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
    
    
    
    //drawMM(20, 155, 20);
}

#endif
