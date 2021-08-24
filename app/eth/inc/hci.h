////////////////////////////////////////////////////////////////////////////////
/// @file     HCI.H
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
#ifndef __HCI_H
#define __HCI_H


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

#ifdef _HCI_C_

#define GLOBAL

GLOBAL bool K1Flag = false;
GLOBAL bool K2Flag = false;
GLOBAL bool K3Flag = false;

#else
#define GLOBAL extern

#endif

GLOBAL bool K1Flag,K2Flag,K3Flag,K4Flag;
GLOBAL u16  LDFreq[4], LDCnt[4];

#undef GLOBAL

/// @}

////////////////////////////////////////////////////////////////////////////////
/// @defgroup UID_Exported_Functions
/// @{

/// @}

void hci_task(void);
void hci_tick();

void Key1Down(void);
void Key2Down(void);
void Key3Down(void);
void Key4Down(void);
void Key1Pressing(void);
void Key2Pressing(void);
void Key3Pressing(void);
void Key4Pressing(void);

/// @}

/// @}

////////////////////////////////////////////////////////////////////////////////
#endif /* __MUSIC_H */
////////////////////////////////////////////////////////////////////////////////
