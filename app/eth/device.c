////////////////////////////////////////////////////////////////////////////////
/// @file     DEVICE.C
/// @author   ChenDo
/// @version  v1.0.0
/// @date     2021-07-01
/// @brief    THIS FILE PROVIDES ALL THE LIFT EXAMPLE.
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
#define _DEVICE_C_

// Includes  -------------------------------------------------------------------
#include <string.h>
#include "mm32_types.h"

#include "device.h"
#include "com.h"
#include "eth.h"

#include "bsp_beep.h"
#include "common.h"

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
void device_task()
{
    myDev.id = myBoardIdx;
    myDev.up = dev_Up;
    myDev.dn = dev_Dn;
}

////////////////////////////////////////////////////////////////////////////////
void device_tick()
{
    static u16 beepOpenTime;
    if (myDev.up){
        if ((rxDev.id == myDev.id) && rxDev.up){
            beepOpenTime = 100;
            dev_Up = false;
        }
    }
    if (myDev.dn){
        if ((rxDev.id == myDev.id) && rxDev.dn){
            beepOpenTime = 100;
            dev_Dn = false;
        }
    }
    if (beepOpenTime > 0){
        BEEP_on(1300);
        beepOpenTime--;
    }
    else {
        BEEP_off();
        beepOpenTime = 0;
    }
}

////////////////////////////////////////////////////////////////////////////////
void BSP_Device_Configure()
{
    BSP_BEEP_Configure(1000);
    for(u8 i = 0; i < 2; i++){
        BEEP_on(1000);
        while(!delay(50));
        BEEP_off();
        while(!delay(200));
    }
}


/// @}

/// @}

/// @}
