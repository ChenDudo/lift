////////////////////////////////////////////////////////////////////////////////
/// @file     UID.C
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
#define _HCI_C_

// Includes  -------------------------------------------------------------------
#include <string.h>
#include "mm32_types.h"

#include "common.h"

#include "hci.h"

#include "com.h"
#include "eth.h"
#include "device.h"

#include "bsp_eth.h"
#include "bsp_phy8720.h"
#include "bsp_key.h"
#include "bsp_led.h"

#if defined(__MasterTest)
#include "master.h"
#endif

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
void hci_task()
{
    BSP_Key(1);
    BSP_Key(2);
    BSP_Key(3);
    BSP_Key(4);
    
    ledStatus[0] ? LD1_on() : LD1_off();
    ledStatus[1] ? LD2_on() : LD2_off();
    ledStatus[2] ? LD3_on() : LD3_off();
    ledStatus[3] ? LD4_on() : LD4_off();
    
    if(K1Flag){
        dev_Up = true;
        K1Flag = false;
    }
    if(K2Flag){
        dev_Dn = true;
        K2Flag = false;
    }
    if(K3Flag){
        K3Flag = false;
    }
    if(K4Flag){      
        K4Flag = false;
    }
    
    if (keyDoubleFlag[0]){
        dev_Up = false;
        keyDoubleFlag[0] = false;
    }
    if (keyDoubleFlag[1]){
        dev_Dn = false;
        keyDoubleFlag[1] = false;
    }
}

////////////////////////////////////////////////////////////////////////////////
void hci_tick()
{
    static u16 hciCnt;
    if (hciCnt++ > 499){
        hciCnt = 0;
        ledStatus[3] = !ledStatus[3];
    }
    BSP_KeyTick();
    ledStatus[0] = dev_Up;
    ledStatus[1] = dev_Dn;
}

////////////////////////////////////////////////////////////////////////////////
void Key1Down()
{
    K1Flag = true;
    ledStatus[0] = !ledStatus[0];
}

////////////////////////////////////////////////////////////////////////////////
void Key2Down()
{
    K2Flag = true;
    ledStatus[1] = !ledStatus[1];
}

////////////////////////////////////////////////////////////////////////////////
void Key3Down()
{
    K3Flag = true;
    ledStatus[2] = !ledStatus[2];
}

////////////////////////////////////////////////////////////////////////////////
void Key4Down()
{
    K4Flag = true;
    ledStatus[3] = !ledStatus[3];
}

////////////////////////////////////////////////////////////////////////////////
void Key1Pressing()
{
    ledStatus[0] = 1;
}

////////////////////////////////////////////////////////////////////////////////
void Key2Pressing()
{
    ledStatus[1] = 1;
}

////////////////////////////////////////////////////////////////////////////////
void Key3Pressing()
{
    ledStatus[2] = 1;
}

////////////////////////////////////////////////////////////////////////////////
void resetTest()
{
    void (*fp)(void);
    fp = (void (*)(void))(* (vu32 *)(0x8000004));
    (*fp)(); 
}


////////////////////////////////////////////////////////////////////////////////
void Key4Pressing()
{
    //COMMON_DisableIpClock(emCLOCK_ETH);
    //ETH_Configure();
    //resetTest();
}

/// @}

/// @}

/// @}
