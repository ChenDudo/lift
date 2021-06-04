////////////////////////////////////////////////////////////////////////////////
/// @file     COM.H
/// @author   
/// @version  v2.0.0
/// @date     2019-03-13
/// @brief    THIS FILE CONTAINS ALL THE FUNCTIONS PROTOTYPES FOR THE COMMUNICATION
///           EXAMPLES.
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
#ifndef __COM_H
#define __COM_H

// Files includes  -------------------------------------------------------------

////////////////////////////////////////////////////////////////////////////////
/// @addtogroup MM32_Example_Layer
/// @{

#define USE_CHECKSUM    0
#define BCOVERTIME      50
#define MAXDEVICE       100

typedef enum {
    pass = 0,
    sendAckError,
    changePhyError,
    revNextError
} ErrorCodeNum;

typedef enum {
    bcType = 0x01, //broadcast
    hrType = 0x02, //heartbeat
    others = 0x7F
} codeType;

typedef struct {
    u8 dirAddr[6];
    u8 srcAddr[6];
    u8 FrameType[2];
    u8 head[3];
    u8 data[3*MAXDEVICE];
} pbuf;


////////////////////////////////////////////////////////////////////////////////
/// @defgroup EXAMPLE_UID
/// @brief UID example modules
/// @{

////////////////////////////////////////////////////////////////////////////////
/// @defgroup UID_Exported_Variables
/// @{

#ifdef _COM_C_
#define GLOBAL


#else
#define GLOBAL extern
#endif

GLOBAL u8 myBoardIdx;
GLOBAL u8 saveLastBoardIdx;
GLOBAL u8 sendptr[17+3*MAXDEVICE];
GLOBAL u8 broadcastSMState;
GLOBAL u8 heartBeatSMState;
GLOBAL ErrorCodeNum SMERROR;
GLOBAL u8 getfirstBoardIdx;
GLOBAL u8 getlastBoardIdx;
GLOBAL u8 sendBoardIdx;
GLOBAL u8 sendBCType; 
GLOBAL u32 comTick;

GLOBAL bool preBCFlag;
GLOBAL bool comTickFlag;
GLOBAL bool revBCFlag;
GLOBAL bool revBCAckFlag;
GLOBAL bool revHBFlag;
GLOBAL bool revHBAckFlag;
GLOBAL bool sendBCFinishFlag;
GLOBAL pbuf* revPtr;
GLOBAL pbuf sendBCBuf;

//chendo
GLOBAL pbuf revBCBuf1;
GLOBAL pbuf revBCBuf2;

#undef GLOBAL

/// @}

////////////////////////////////////////////////////////////////////////////////
/// @defgroup UID_Exported_Functions
/// @{

void com_task(void);
void com_tick(void);

void broadcastStateMachine();
void decodeFramDat(u8* rvBuff, u16 len);
void prepareBroadCastDat(u8 boardidx, u8 mode);
u16 chksum(void *dataptr, u16 len);

/// @}


/// @}

/// @}

////////////////////////////////////////////////////////////////////////////////
#endif /* __ETH_H */
////////////////////////////////////////////////////////////////////////////////
