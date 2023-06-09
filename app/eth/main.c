////////////////////////////////////////////////////////////////////////////////
/// @file     MAIN.C
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
#define _MAIN_C_

// Includes  -------------------------------------------------------------------
#include <string.h>
#include "mm32_types.h"
#include "mm32_system.h"
#include "common.h"

#include "bsp_led.h"
#include "bsp_lcd.h"
#include "bsp_key.h"
#include "bsp_phy8720.h"

#include "main.h"
#include "hci.h"
#include "eth.h"
#include "com.h"
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

////////////////////////////////////////////////////////////////////////////////
void initPara()
{
    SystemTick_Count = 0;

    memset(ledStatus, 0x00, sizeof(ledStatus));
    memset((u8*)&sendBCBuf, 0x00, sizeof(sendBCBuf));

    text.fore = Black;
    text.back = White;
}

////////////////////////////////////////////////////////////////////////////////
void initPeri()
{
    BSP_LCD_Configure();
    BSP_KEY_Configure();
    BSP_KeyFuncSet(1, Key1Down, Key1Pressing);
    BSP_KeyFuncSet(2, Key2Down, Key2Pressing);
    BSP_KeyFuncSet(3, Key3Down, Key3Pressing);
    BSP_KeyFuncSet(4, Key4Down, Key4Pressing);
    BSP_LED_Configure();
    BSP_Device_Configure();
    ETH_Configure();

    for(u8 i = 0; i < 2; i++){
        OpenLED();
        while(!delay(50));
        CloseLED();
        while(!delay(200));
    }
#if defined(__MasterTest)
    BSP_MASTER_Configure();
#endif
}

////////////////////////////////////////////////////////////////////////////////
/// @brief  This function handles App SysTick Handler.
/// @param  None.
/// @retval None.
////////////////////////////////////////////////////////////////////////////////
void AppTaskTick()
{
    SystemTick_Count ++;
    if (tickCnt++ >= 500) {
        tickCnt = 0;
    }
    if(ready){
        hci_tick();
        eth_tick();
        com_tick();
        lcd_tick();
        device_tick();
#if defined(__MasterTest)
        master_tick();
#endif
    }
}

////////////////////////////////////////////////////////////////////////////////
/// @brief  main function.
/// @param  None.
/// @retval None.
////////////////////////////////////////////////////////////////////////////////
int main(void)
{
    MCUID = SetSystemClock(emSYSTICK_On, AppTaskTick);

    initPara();
    initPeri();

    ready = true;

    while (1) {
        hci_task();
        eth_task();
        com_task();
        device_task();
#if defined(__MasterTest)
        master_task();
#endif
    }
}

/// @}

/// @}

/// @}
