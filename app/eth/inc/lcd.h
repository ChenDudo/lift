////////////////////////////////////////////////////////////////////////////////
#ifndef __LCD_H_
#define __LCD_H_
////////////////////////////////////////////////////////////////////////////////


#if defined(__MDM2803) || defined(__MDM2803_N)

#define  DMAX 	80

#define offsetH		50

////////////////////////////////////////////////////////////////////////////////
#ifdef _LCD_C_
#define GLOBAL

u16 dx = 5;
u16 dy = 10;
u16 dw = 242;
u16 dh = 135;

#else
#define GLOBAL extern
#endif

GLOBAL u16 dx;
GLOBAL u16 dy;
GLOBAL u16 dw;
GLOBAL u16 dh;
GLOBAL u16 drawBlockCnt;
GLOBAL u16 lcdCnt;
#undef GLOBAL
////////////////////////////////////////////////////////////////////////////////

u16 getColor(u8 c);
void randRefresh();
void clearLeftScreen();
void clearButtomScreen();
void clearAllScreen();

void setCAN(u16 w, u16 h, u8 idx, u8 sel);
void setUART(u16 w, u16 h, u8 idx, u8 sel);
void frontBack(u8 sel);
void setTIME(u16 w, u16 h, u8 idx, u8 sel);
void menuCAN(u8 idx, u8 sta, u8 sel);
void menuUART(u8 idx, u8 sta, u8 sel);
void menuTIME(u8 idx, u8 sta, u8 sel);
void dispScreen();

void drawMM(u16 x, u16 y, u16 w);
void dispIdx(u16 idx);
void dispLED(u8 phy1, u8 phy2);
void dispButton(u8 upflag, u8 dnflag);

void initLcdDemo();
void refreshLCD(u8 sele,  u16* flag);
void BSP_LCD_Configure();
void lcd_tick();

#endif
////////////////////////////////////////////////////////////////////////////////
#endif
////////////////////////////////////////////////////////////////////////////////
