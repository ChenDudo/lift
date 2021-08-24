////////////////////////////////////////////////////////////////////////////////
/// @file     ETH.C
/// @author   Nanjing AE Team
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
#define _ETH_C_

// Includes  -------------------------------------------------------------------
#include <string.h>
#include "mm32_types.h"
#include "mm32_system.h"

#include "mm32.h"
#include "eth.h"
#include "com.h"
#include "common.h"

#include "hci.h"
#include "bsp_eth.h"
#include "bsp_phy8720.h"
#include "bsp_led.h"

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
void eth_task()
{
    if(ethIT_R_flag){
        recvBuffer();
        ethIT_R_flag = false;
    }
}

////////////////////////////////////////////////////////////////////////////////
void eth_tick()
{
    static u16 ethCnt1;
    if (ethCnt1++ > 9){
        ethCnt1 = 0;
        checkPhyStatus(phySelA);
    }
    //phyLink_Flag ? checkPhyStatus(phySelA) : writePhyConfig(phySelA);

    //if (!phySelA) {
    //    //if (phyLink_Flag){
    //    if (){
    //        phyA_Linking = true;
    //        phyA_Linked = true;
    //        phyA_LinkCnt = 2000;
    //    }
    //    else
    //        phyA_Linking = false;
    //}
    //if (phySelA) {
    //    if (phyLink_Flag){
    //        phyB_Linking = true;
    //        phyB_Linked = true;
    //        phyB_LinkCnt = 2000;
    //    }
    //    else
    //        phyB_Linking = false;
    //}
}

////////////////////////////////////////////////////////////////////////////////
void ETH_IRQHandler()
{
    if (ETH_GetDMAITStatus(ETH_DMA_IT_R)) {
        ethIT_R_flag = true;
        ETH_DMAClearITPendingBit(ETH_DMA_IT_R);
    }
    if (ETH_GetDMAITStatus(ETH_DMA_IT_T)) {

        ETH_DMAClearITPendingBit(ETH_DMA_IT_T);
    }
    if (ETH_GetDMAITStatus(ETH_DMA_FLAG_AIS)) {

        ETH_DMAClearITPendingBit(ETH_DMA_FLAG_AIS);
    }
    ETH_DMAClearITPendingBit(ETH_DMA_IT_NIS);
}

////////////////////////////////////////////////////////////////////////////////
s8 sendBuffer(u8* p, u16 len)
{
    memcpy((u8*)DMATxDescToSet->BUF1ADDR, (u8*)p, len);
    return ETH_Prepare_Transmit_Descriptors(len);
}

////////////////////////////////////////////////////////////////////////////////
s8 recvBuffer()
{
    u8* addrTemp = (u8*)DMARxDescToGet->BUF1ADDR;
    u16 lenTemp = (u16)((DMARxDescToGet->CS & ETH_DMA_RDES_FL) >> ETH_DMA_RDES_FL_Pos) - 4;
    decodeFramDat(addrTemp, lenTemp);

    DMARxDescToGet->CS |= ETH_DMA_RDES_OWN;
    DMARxDescToGet = (ETH_DMADESCTypeDef*)(DMARxDescToGet->BUF2NDADDR);
    if (ETH->DMASR & ETH_DMASR_RBUS) {
        ETH->DMASR = ETH_DMASR_RBUS;
        ETH->DMARPDR = 0;
    }
    return 0;
}

////////////////////////////////////////////////////////////////////////////////
void ETH_Configure(void)
{
    RCC->APB1ENR |= RCC_APB1ENR_PWR;
    PWR->CR |= 3 << 14;

    PHY8720_GPIO_Config();
    BSP_ETH_Configure();
    ETH_DMATxDescChainInit(DMATxDscrTab, &Tx_Buff[0][0], ETH_TX_BUF_NUM);
    ETH_DMARxDescChainInit(DMARxDscrTab, &Rx_Buff[0][0], ETH_RX_BUF_NUM);
    ETH_DMAITConfig(ETH_DMAIER_TIE | ETH_DMAIER_RIE, ENABLE);
    BSP_PHY8720_Configure(defaultphySelA);
    ETH_NVIC_Config();
    ETH_Start();
}

/// @}

/// @}

/// @}
