////////////////////////////////////////////////////////////////////////////////
/// @file     ETH.H
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
#ifndef __ETH_H
#define __ETH_H

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

#ifdef _ETH_C_

#define GLOBAL
#else
#define GLOBAL extern

#endif

GLOBAL bool phySelA;
GLOBAL bool ethIT_R_flag;
GLOBAL bool phyA_Linking;
GLOBAL bool phyB_Linking;
GLOBAL bool phyA_Linked;
GLOBAL bool phyB_Linked;
GLOBAL bool defaultphySelA;

GLOBAL u16 receiveLen;
GLOBAL u8* receiveStr;
GLOBAL u16 phyA_LinkCnt;
GLOBAL u16 phyB_LinkCnt;

#undef GLOBAL

/// @}

////////////////////////////////////////////////////////////////////////////////
/// @defgroup UID_Exported_Functions
/// @{

void ETH_Configure(void);
void eth_task(void);
void eth_tick(void);

s8 sendBuffer(u8* p, u16 len);
s8 sendLongBuffer(u8* p, u16 len);
s8 recvBuffer();

/// @}


/// @}

/// @}

////////////////////////////////////////////////////////////////////////////////
#endif /* __ETH_H */
////////////////////////////////////////////////////////////////////////////////
