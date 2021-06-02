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

#define BCPEIROD 80

devType revDev[100];
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
    static u16 BCCount;
    if(BCModeCoutinue){
        if( ++BCCount > BCPEIROD){
            masterSendBroadcast();
            BCCount = 0;
        }
    }
    if(masterTickCnt++ >= 100){
        masterTickCnt = 0;
        dispIdx(revDev[0].id);
        dispButton(revDev[0].up, revDev[0].dn);
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
    sendBCBuf.head[2] = 0;      // sub-frame num = 1
    
    /* data */
    u16 datalen = 0;

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
            
            //dispIdx(revDev[i].id);
            //dispButton(revDev[i].up, revDev[i].dn);
        }
    }
}

#endif

/// @}

/// @}

/// @}
