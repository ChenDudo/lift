////////////////////////////////////////////////////////////////////////////////
/// @file     MASTER.H
/// @author   Nanjing AE Team
/// @version  v1.0.0
/// @date     2021-07-01
/// @brief    THIS FILE CONTAINS ALL THE FUNCTIONS PROTOTYPES FOR THE LIFT
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
#ifndef __MASTER_H
#define __MASTER_H

#if defined(__MasterTest)

// Files includes  -------------------------------------------------------------

////////////////////////////////////////////////////////////////////////////////
/// @addtogroup MM32_Example_Layer
/// @{


////////////////////////////////////////////////////////////////////////////////
/// @defgroup EXAMPLE_UID
/// @brief UID example modules
/// @{

////////////////////////////////////////////////////////////////////////////////
/// @defgroup UID_Exported_Variables
/// @{

#ifdef _MASTER_C_
#define GLOBAL

GLOBAL u8 MAXFLOOR = 20;
GLOBAL u8 MINFLOOR = 0;
GLOBAL bool BCModeCoutinue = true;

#else
#define GLOBAL extern
#endif

GLOBAL u8 dir;
GLOBAL u8 rxFloorCnt;
GLOBAL u8 MAXFLOOR;
GLOBAL u8 MINFLOOR;
GLOBAL bool phyComFlag;
GLOBAL u16 iTick;

#undef GLOBAL

/// @}

////////////////////////////////////////////////////////////////////////////////
/// @defgroup UID_Exported_Functions
/// @{

void master_task(void);
void master_tick(void);
void BSP_MASTER_Configure(void);
void masterDeInitBroadcast(void);
void dispMasterLED(u8 phy);
void dispMasterRxIdx(u16 idx);
void dispMasterMyIdx(u16 idx);
void dispMasterMyButton(u8 upflag, u8 dnflag);
void masterSendBroadcast(void);
void masterDecode(void);
void loopDisplayRxFLoor(void);

/// @}


/// @}

/// @}

#endif

////////////////////////////////////////////////////////////////////////////////
#endif /* __ETH_H */
////////////////////////////////////////////////////////////////////////////////
