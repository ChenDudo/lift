////////////////////////////////////////////////////////////////////////////////
/// @file     COM.C
/// @author   D CHEN
/// @version  v2.0.0
/// @date     2019-03-13
/// @brief    THIS FILE PROVIDES ALL THE UID EXAMPLE.
////////////////////////////////////////////////////////////////////////////////
/// @attention
///
/// THE EXISTING FIRMWARE IS ONLY FOR REFERENCE, WHICH IS DESIGNED TO PROVIDE
/// CUSTOMERS WITH CODING INFORMATION ABOUT THEIR PRODUCTS SO THEY CAN SAVE
/// TIME. THEREFORE, MINDMOTION SHALL NOT BE LIABLE FOR ANY DIRECT, INDIRECT OR
/// CONSEQUENTIAL DAMAGES ABOUT ANY CLAIMS ARISING OUT OF THE CONTENT OF SUCH
/// HARDWARE AND/OR THE USE OF THE CODING INFORMATION CONTAINED HEREIN IN
/// CONNECTION WITH PRODUCTS MADE BY CUSTOMERS.
///
/// <H2><CENTER>&COPY; COPYRIGHT 2018-2019 MINDMOTION </CENTER></H2>
////////////////////////////////////////////////////////////////////////////////

// Define to prevent recursive inclusion  --------------------------------------
#define _MASTER_C_

#if defined(__MasterTest)
// Includes  -------------------------------------------------------------------
#include <string.h>
#include <stdlib.h>

#include "mm32_types.h"

#include "bsp_lcd.h"
#include "com.h"
#include "eth.h"
#include "lcd.h"
#include "device.h"
#include "master.h"

////////////////////////////////////////////////////////////////////////////////
/// @addtogroup MM32_Example_Layer
/// @{

////////////////////////////////////////////////////////////////////////////////
/// @addtogroup EXAMPLE_UID
/// @{

////////////////////////////////////////////////////////////////////////////////
/// @addtogroup UID_Exported_Functions
/// @{

#define BCPEIROD 2

devType revDev[MAXDEVICE];
u32 masterTickCnt;
void masterSendBroadcast();
void masterDecode();
void loopDisplayRxFLoor();

////////////////////////////////////////////////////////////////////////////////
void master_task()
{
    //masterDecode();
}

////////////////////////////////////////////////////////////////////////////////
void dispMasterLED(u8 phy)
{
    u16 x2 = (WIDTH - 30)/2;
    u16 y2 = 302;
    phy ? drawRec (x2, y2, 30,  16, Green) : drawRec (x2, y2, 30,  16, Red);
}

////////////////////////////////////////////////////////////////////////////////
void dispMasterRxIdx(u16 idx)
{
    drawNum(WIDTH/2-88, (HEIGHT-107)/2, 1, 2, idx, White);
}

////////////////////////////////////////////////////////////////////////////////
void dispMasterRxButton(u8 upflag, u8 dnflag)
{
    u16 c1 = upflag ? Black : White;
    u16 c2 = dnflag ? Black : White;

    drawTriangle(220, HEIGHT/2-10, 20, 30, 0, c1);
    drawTriangle(220, HEIGHT/2+10, 20, 30, 1, c2);
}

////////////////////////////////////////////////////////////////////////////////
void dispMasterMyIdx(u16 idx)
{
    drawNum(0, 320-40, 1, 0, idx, Blue);
}

////////////////////////////////////////////////////////////////////////////////
void dispMasterMyButton(u8 upflag, u8 dnflag)
{
    u16 c1 = upflag ? Yellow : White;
    u16 c2 = dnflag ? Yellow : White;

    drawTriangle((WIDTH-100)/2, HEIGHT/2-90, 100, 40, 0, c1);
    drawTriangle((WIDTH-100)/2, HEIGHT/2+90, 100, 40, 1, c2);
}

////////////////////////////////////////////////////////////////////////////////
void master_tick()
{
    static u16 BCCount, dCnt, eCnt, fCnt;
    static bool mFlag;
    static u16 tick_1s;
    
    masterDecode();
    
    if (BCModeCoutinue){
        if( ++BCCount > BCPEIROD){
            BCCount = 0;
        }
    }
    masterSendBroadcast();
    if (dCnt++ > 50){
        dCnt = 0;
    }
    if (eCnt++ > 250){
        eCnt = 0;
        dispMasterMyIdx(msDev.id);
        mFlag = !mFlag;
        mFlag ? dispMasterMyButton(msDev.up, msDev.dn): dispMasterMyButton(0, 0);
    }
    if (fCnt++ > 500){
        fCnt = 0;
        dispMasterLED(phyA_Linked);
        loopDisplayRxFLoor();
    }
    if (masterTickCnt++ >= 1500){
        masterTickCnt = 0;
        tick_1s = true;
        MAXFLOOR = getlastBoardIdx;
    }
    
    /* simulate the lift running */
    if (tick_1s){
        tick_1s = false;
        switch (dir){
            case 1: /* up */
            if (msDev.id >= MAXFLOOR){
                dir = 2;
                return;
            }
            msDev.up = 1;
            msDev.dn = 0;
            msDev.id ++;
            break;
            case 2: /* down */
            if(msDev.id == MINFLOOR){
                dir = 1;
                return;
            }
            msDev.up = 0;
            msDev.dn = 1;
            msDev.id --;
            break;
            default:
            msDev.up = 0;
            msDev.dn = 0;
            break;
        }
    }
    
}

////////////////////////////////////////////////////////////////////////////////
void loopDisplayRxFLoor()
{
    if (rxFloorCnt <= 1){
        dispMasterRxIdx(revDev[0].id);
        dispMasterRxButton(revDev[0].up, revDev[0].dn);
    }
    else {
        if (iTick < rxFloorCnt) {
            dispMasterRxIdx(revDev[iTick].id);
            dispMasterRxButton(revDev[iTick].up, revDev[iTick].dn);
            iTick ++;
            if (iTick >= rxFloorCnt)
                iTick = 0;
        }
    }
}

////////////////////////////////////////////////////////////////////////////////
void masterSendBroadcast()
{
    /* Head */
    u16 headLen = sizeof(sendBCBuf.dirAddr) + sizeof(sendBCBuf.srcAddr) +\
        sizeof(sendBCBuf.FrameType) + sizeof(sendBCBuf.head);
    memset(sendBCBuf.dirAddr, 0x55, sizeof(sendBCBuf.dirAddr));
    memset(sendBCBuf.srcAddr, 0x66, sizeof(sendBCBuf.srcAddr));
    sendBCBuf.FrameType[0] = 0xAE;
    sendBCBuf.FrameType[1] = 0x88;
    sendBCBuf.head[0] = 0;      // broadcast idx
    sendBCBuf.head[1] = bcType; //(ackNeedFlag) ?  (bcType | 0x80) : bcType;
    sendBCBuf.head[2] = 1;      // sub-frame num = 1
    
    /* data */
    u16 datalen = 3 * sendBCBuf.head[2];
    sendBCBuf.data[0] = msDev.id;
    sendBCBuf.data[1] = dir;
    sendBCBuf.data[2] = 0;

    /* checksum */
    u16 chsumlen = 0;
#if USE_CHECKSUM
    chsumlen = 2;
    u16 checksum = chksum((u8*)&sendBCBuf, headLen + datalen);
    sendBCBuf.data[datalen] = checksum;
    sendBCBuf.data[datalen+1] = (checksum & 0xFF00) >> 8;
#endif
    
    /* copy to ptr */
    memcpy(sendptr, (u8*)&sendBCBuf, headLen+datalen+chsumlen);
    
    sendBuffer(sendptr, sizeof(sendptr));
}

////////////////////////////////////////////////////////////////////////////////
void masterDecode()
{
    if (revBCAckFlag){
        rxFloorCnt = revPtr->head[2];
        for(u8 i = 0; i <= rxFloorCnt; i++){
            revDev[i].id = revPtr->data[i * 3];
            revDev[i].up = revPtr->data[i * 3 + 1];
            revDev[i].dn = revPtr->data[i * 3 + 2];
        }
    }
}

////////////////////////////////////////////////////////////////////////////////
void BSP_MASTER_Configure()
{
    dir = 1;
    BCModeCoutinue = true;
    //putStr(10, 20, 2, 1, "SIMULATOR");
}

#endif

/// @}

/// @}

/// @}
