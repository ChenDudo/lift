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

#define BCPEIROD 200

devType revDev[MAXDEVICE];
u32 masterTickCnt;
void masterSendBroadcast();
void masterDecode();

////////////////////////////////////////////////////////////////////////////////
void master_task()
{
    masterDecode();
}

////////////////////////////////////////////////////////////////////////////////
void master_tick()
{
    static u16 BCCount, dCnt, mFlag;
    static u16 tick_1s;
    
    if (BCModeCoutinue){
        if( ++BCCount > BCPEIROD){
            masterSendBroadcast();
            BCCount = 0;
        }
    }
    if (masterTickCnt++ >= 1000){
        masterTickCnt = 0;
        tick_1s = true;
        dispMyIdx(msDev.id);
        
    }
    if (dCnt++ > 300){
        dCnt = 0;
        mFlag = !mFlag;
        dispIdx(revDev[0].id);
        dispButton(revDev[0].up, revDev[0].dn);
        mFlag ? dispMyButton(msDev.up, msDev.dn): dispMyButton(0, 0);
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
        u8 datLen = revPtr->head[2];
        for(u8 i = 0; i <= datLen; i++){
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
    putStr(10, 20, 2, 1, "SIMULATOR");
}

#endif

/// @}

/// @}

/// @}
