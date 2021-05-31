////////////////////////////////////////////////////////////////////////////////
/// @file     BSP_ETH.C
/// @author   AE Team
/// @version  v2.0.0
/// @date     2019-02-18
/// @brief    THIS FILE PROVIDES ALL THE LED BSP LAYER FUNCTIONS.
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
#define _BSP_ETH_C_

// Files includes  -------------------------------------------------------------
#include <string.h>
#include "hal_gpio.h"
#include "hal_rcc.h"
#include "hal_eth.h"
#include "hal_nvic.h"

#include "bsp.h"
#include "common.h"

#include "bsp_eth.h"
#include "bsp_phy8720.h"

/* -----------------------------------------------------------------------------
----------  P e r i p h e r a l s    i n i t i a l i z a t i o n   -------------
----------------------------------------------------------------------------- */
////////////////////////////////////////////////////////////////////////////////
/// @addtogroup MM32_Board_Support_Package
/// @{

////////////////////////////////////////////////////////////////////////////////
/// @addtogroup BEEP_BSP
/// @{

////////////////////////////////////////////////////////////////////////////////
/// @addtogroup BEEP_Exported_Functions
/// @{

////////////////////////////////////////////////////////////////////////////////
/// @brief  Initialize.
/// @param  None.
/// @retval None.
////////////////////////////////////////////////////////////////////////////////
static void ETH_MAC_DMA_Config()
{
    EthStatus = 0;
    ETH_StructInit(&ETH_InitStructure);
    ETH_InitStructure.ETH_AutoNegotiation          = ETH_AutoNegotiation_Disable;
    ETH_InitStructure.ETH_Speed                    = ETH_Speed_100M;
    ETH_InitStructure.ETH_Mode                     = ETH_Mode_FullDuplex;
    ETH_InitStructure.ETH_LoopbackMode             = ETH_LoopbackMode_Disable;
    ETH_InitStructure.ETH_RetryTransmission        = ETH_RetryTransmission_Disable;
    ETH_InitStructure.ETH_ReceiveAll               = ETH_ReceiveAll_Enable;
    ETH_InitStructure.ETH_PromiscuousMode          = ETH_PromiscuousMode_Disable;
    ETH_InitStructure.ETH_ChecksumOffload          = ETH_ChecksumOffload_Disable;
    ETH_InitStructure.ETH_UnicastFramesFilter      = ETH_UnicastFramesFilter_Perfect;
    ETH_InitStructure.ETH_AutomaticPadCRCStrip     = ETH_AutomaticPadCRCStrip_Disable;
    ETH_InitStructure.ETH_MulticastFramesFilter    = ETH_MulticastFramesFilter_Perfect;
    ETH_InitStructure.ETH_BroadcastFramesReception = ETH_BroadcastFramesReception_Enable;
    /*  When we use the Checksum offload feature, we need to enable the Store and Forward mode:
    // the store and forward guarantee that a whole frame is stored in the FIFO, so the MAC can insert/verify the checksum,
    // if the checksum is OK the DMA can handle the frame otherwise the frame is dropped */
    ETH_InitStructure.ETH_ReceiveStoreForward      = ETH_ReceiveStoreForward_Enable;
    ETH_InitStructure.ETH_TransmitStoreForward     = ETH_TransmitStoreForward_Enable;
    ETH_InitStructure.ETH_ForwardErrorFrames       = ETH_ForwardErrorFrames_Disable;
    ETH_InitStructure.ETH_SecondFrameOperate       = ETH_SecondFrameOperate_Enable;
    ETH_InitStructure.ETH_AddressAlignedBeats      = ETH_AddressAlignedBeats_Disable;
    ETH_InitStructure.ETH_FixedBurst               = ETH_FixedBurst_Enable;
    ETH_InitStructure.ETH_RxDMABurstLength         = 0;
    ETH_InitStructure.ETH_TxDMABurstLength         = ETH_TxDMABurstLength_32Beat;
    ETH_InitStructure.ETH_DMAArbitration           = ETH_DMAArbitration_RoundRobin_RxTx_1_1;
    ETH_InitStructure.ETH_DropTCPIPChecksumErrorFrame = ETH_DropTCPIPChecksumErrorFrame_Enable;
    ETH_InitStructure.ETH_ForwardUndersizedGoodFrames = ETH_ForwardUndersizedGoodFrames_Disable;

    COMMON_EnableIpClock(emCLOCK_ETH);
    ETH_DeInit();
    ETH_SoftwareReset();
    while (ETH_GetSoftwareResetStatus());
    
    exETH_MDCClock();
    EthStatus = ETH_Init(&ETH_InitStructure);
}

////////////////////////////////////////////////////////////////////////////////
void ETH_NVIC_Config(void)
{
    NVIC_InitTypeDef   NVIC_InitStructure;
    NVIC_PriorityGroupConfig(NVIC_PriorityGroup_4);

    NVIC_InitStructure.NVIC_IRQChannel = ETH_IRQn;
    NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 6;
    NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;
    NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
    NVIC_Init(&NVIC_InitStructure);
}

////////////////////////////////////////////////////////////////////////////////
void BSP_ETH_Configure()
{
    ETH_MAC_DMA_Config();
    ETH_DMAITConfig(ETH_DMA_IT_NIS | ETH_DMA_IT_R,ENABLE); 
}

/// @}

/// @}

/// @}
