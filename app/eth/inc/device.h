////////////////////////////////////////////////////////////////////////////////
/// @file     DEVICE.H
/// @author   
/// @version  v2.0.0
/// @date     2019-03-13
/// @brief    THIS FILE CONTAINS ALL THE FUNCTIONS PROTOTYPES FOR THE UID
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
#ifndef __DEVICE_H
#define __DEVICE_H

// Files includes  -------------------------------------------------------------


////////////////////////////////////////////////////////////////////////////////
/// @addtogroup MM32_Example_Layer
/// @{

////////////////////////////////////////////////////////////////////////////////
/// @defgroup EXAMPLE_DEVICE
/// @brief DEVICE example modules
/// @{

typedef struct {
    u8 id;
    u8 up;
    u8 dn;
} devType;

////////////////////////////////////////////////////////////////////////////////
/// @defgroup DEVICE_Exported_Variables
/// @{

#ifdef _DEVICE_C_
#define GLOBAL


#else
#define GLOBAL extern
#endif

GLOBAL devType myDev;
GLOBAL bool dev_Up;
GLOBAL bool dev_Dn;

#undef GLOBAL

/// @}

////////////////////////////////////////////////////////////////////////////////
/// @defgroup DEVICE_Exported_Functions
/// @{

/// @}

void device_task();
void device_tick();

/// @}

/// @}

////////////////////////////////////////////////////////////////////////////////
#endif /* __DEVICE_H */
////////////////////////////////////////////////////////////////////////////////
