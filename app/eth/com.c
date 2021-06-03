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
#define _COM_C_

// Includes  -------------------------------------------------------------------
#include <string.h>
#include <stdlib.h>

#include "mm32_types.h"
#include "mm32_system.h"

#include "mm32.h"
#include "com.h"
#include "common.h"
#include "device.h"
#include "bsp_phy8720.h"

#include "eth.h"

////////////////////////////////////////////////////////////////////////////////
/// @addtogroup MM32_Example_Layer
/// @{

////////////////////////////////////////////////////////////////////////////////
/// @addtogroup EXAMPLE_UID
/// @{

////////////////////////////////////////////////////////////////////////////////
/// @addtogroup UID_Exported_Functions
/// @{

////////////////////////////////////////////////////////////////////////////////
void com_task()
{
    /* send bc buffer */
    if(phyLink_Flag && preBCFlag){
        sendBuffer(sendptr, sizeof(sendptr));
        sendBCFinishFlag = true;
        preBCFlag = false;
    }
    broadcastStateMachine();
}

////////////////////////////////////////////////////////////////////////////////
void com_tick()
{
    comTickFlag = true;
    comTick++;
}

////////////////////////////////////////////////////////////////////////////////
void decodeFramDat(u8* rvBuff, u16 len)
{
    /* Check checksum */
#if USE_CHECKSUM
    u16 datalen = 3 * *(rvBuff + 16);
    if (chksum(rvBuff, datalen + 17) != *(u16*)(rvBuff + datalen + 17)) return;
#endif
    
    /* Check Frame Type */
    if(*(u16*)(rvBuff + 12) != 0x88AE) return;
    
    /* decode receive data */
    revPtr = (pbuf*)rvBuff;
    
    switch(revPtr->head[1]){
        /*......... broadcast .............*/
        case 0x01:
        revBCFlag = true;
        getfirstBoardIdx = revPtr->head[0];
        break;
        case 0x81:
        revBCAckFlag = true;
        getlastBoardIdx = revPtr->head[0];
        break;
        
        /*......... heartbeat .............*/
        case 0x02:
        revHBFlag = true;
        break;
        case 0x82:
        revHBAckFlag = true;
        break;
        
        /*......... otherType .............*/
        default:
        break;
    }
}

////////////////////////////////////////////////////////////////////////////////
u16 chksum(void *dataptr, u16 len)
{
    u32 acc;
    u16 src;
    u8 *octetptr;
    
    acc = 0;
    octetptr = (u8*)dataptr;
    while (len > 1) {
        src = (*octetptr) << 8;
        octetptr++;
        src |= (*octetptr);
        octetptr++;
        acc += src;
        len -= 2;
    }
    if (len > 0) {
        src = (*octetptr) << 8;
        acc += src;
    }
    
    acc = (acc >> 16) + (acc & 0x0000ffffUL);
    if ((acc & 0xffff0000UL) != 0) {
        acc = (acc >> 16) + (acc & 0x0000ffffUL);
    }
    
    src = (u16)acc;
    return ~src;
}

////////////////////////////////////////////////////////////////////////////////
void prepareBroadCastDat(u8 boardidx, u8 mode)
{
    /* Head */
    u16 headLen = sizeof(sendBCBuf.dirAddr) + sizeof(sendBCBuf.srcAddr) +\
        sizeof(sendBCBuf.FrameType) + sizeof(sendBCBuf.head);
    memset(sendBCBuf.dirAddr, 0x55, sizeof(sendBCBuf.dirAddr));
    memset(sendBCBuf.srcAddr, 0x66, sizeof(sendBCBuf.srcAddr));
    sendBCBuf.FrameType[0] = 0xAE;
    sendBCBuf.FrameType[1] = 0x88;
    sendBCType = mode ? (bcType | 0x80) : bcType;
    sendBCBuf.head[0] = boardidx;       // broadcast idx
    sendBCBuf.head[1] = sendBCType;           //(ackNeedFlag) ?  (bcType | 0x80) : bcType;
    
    /* data */
    u16 datalen;
    if (mode){
        if ((dev_Up || dev_Dn)){
            datalen = 3 * *(u8*)(saveRevPtr + 16);
            sendBCBuf.head[2] = *(u8*)(saveRevPtr + 16) + 1;
            sendBCBuf.data[0] = myDev.id;
            sendBCBuf.data[1] = myDev.up;
            sendBCBuf.data[2] = myDev.dn;
            memcpy(sendBCBuf.data + 3, saveRevPtr + 17, datalen);
            datalen += 3;
        }
        else {
            sendBCBuf.head[2] = *(u8*)(saveRevPtr + 16);
            datalen = 3 * *(u8*)(saveRevPtr + 16);
            memcpy(sendBCBuf.data, saveRevPtr + 17, datalen);
        }
        memset(saveRevPtr, 0x00, sizeof(saveRevPtr));
    }
    else {
        datalen = 3 * *(u8*)(revPtr + 16);
        sendBCBuf.head[2] = *(u8*)(revPtr + 16);        // sub-frame num = 1
        memcpy(sendBCBuf.data, revPtr + 17, datalen);
        memset(revPtr, 0x00, sizeof(revPtr));
    }
    
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
    memset((u8*)&sendBCBuf, 0x00, sizeof(sendBCBuf));
    
    /* Finish */
    preBCFlag = true;
}

////////////////////////////////////////////////////////////////////////////////
void broadcastStateMachine()
{    
    switch(broadcastSMState){
        
        /* state 0: idle state: cheak broadcast */
        case 0:{
            if (revBCFlag){
                broadcastSMState = 1;
                revBCFlag = false;
                comTick = 0;
            }
        }
        break;
        
        /* state 1: ack to forward node */
        case 1:{
            static bool c1first = true;
            if (c1first){
                myBoardIdx = getfirstBoardIdx + 1;
                sendBoardIdx = (myBoardIdx > saveLastBoardIdx) ? myBoardIdx : saveLastBoardIdx;
                prepareBroadCastDat(sendBoardIdx, 1);
                c1first = false;
            }
            if (sendBCFinishFlag) {
                sendBCFinishFlag = false;
                broadcastSMState = 2;
                c1first = true;
            }
            else if (comTick > BCOVERTIME) {
                SMERROR = sendAckError;
                broadcastSMState = 0;
                comTick = 0;
                c1first = true;
            }
        }
        break;
        
        /* state 2: change to phyB, prepare send my idx to next node */
        case 2:{
            static bool c2first = true;
            if (c2first){
                phySelA = !phySelA;
                changePhy(phySelA);
                sendBoardIdx = myBoardIdx;
                sendBCType = bcType;
                prepareBroadCastDat(sendBoardIdx, 0);
                c2first = false;
            }
            if (sendBCFinishFlag) {
                sendBCFinishFlag = false;
                broadcastSMState = 3;
                c2first = true;
            }
            else if (comTick > BCOVERTIME){
                SMERROR = changePhyError;
                broadcastSMState = 4;
                comTick = 0;
                c2first = true;
            }
        }
        break;
        
        /* state 3: get next node ack signal, overtime retry & change PHYA */
        case 3:{
            if (revBCAckFlag){
                saveLastBoardIdx = getlastBoardIdx;
                memcpy(saveRevPtr, revPtr, 140);
                broadcastSMState = 4;
                revBCAckFlag = false;
            }
            if (comTick > BCOVERTIME){
                SMERROR = revNextError;
                broadcastSMState = 4;
                comTick = 0;
            }
        }
        break;
        
        /* state 4: change to phyA */
        case 4:{
            static bool c4first = true;
            if (c4first){
                phySelA = defaultphySelA;
                changePhy(phySelA);
                c4first = false;
            }
            if (phyLink_Flag){
                broadcastSMState = 0;
                c4first = true;
                memset(revPtr, 0x00, sizeof(revPtr));
            }
        }
        break;
        
        /* other */
        default:
        
        break;
    }
}

/// @}

/// @}

/// @}
