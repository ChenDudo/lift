////////////////////////////////////////////////////////////////////////////////
/// @file     ETH.C
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
        ethIT_R_flag = false;
        recvBuffer();
    }
    
    // error handle
    /* receive description unavailable */
    if (ETH->DMASR & ETH_DMASR_RPS_Suspended){
        
    }
    if (ETH->DMASR & ETH_DMASR_TBUS){
        
    }
}

////////////////////////////////////////////////////////////////////////////////
void eth_tick()
{
    //phyLink_Flag ? checkPhyStatus(phySelA) : writePhyConfig(phySelA);
    static u16 ethCnt;
    if (ethCnt++ > 9){
        ethCnt = 0;
        checkPhyStatus(phySelA);
    }
    
    if (!phySelA) {
        if (phyLink_Flag){
            phyA_Linking = true;
            phyA_Linked = true;
            phyA_LinkCnt = 2000;
        }
        else
            phyA_Linking = false;
    }
    if (phySelA) {
        if (phyLink_Flag){
            phyB_Linking = true;
            phyB_Linked = true;
            phyB_LinkCnt = 2000;
        }
        else
            phyB_Linking = false;
    }
    if (--phyA_LinkCnt == 0)
        phyA_Linked = false;
    if (--phyB_LinkCnt == 0)
        phyB_Linked = false;
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

/*
////////////////////////////////////////////////////////////////////////////////
s8 sendLongBuffer(u8* p, u16 len)
{
    __IO ETH_DMADESCTypeDef *DmaTxDesc;
    
    u16 frameLen = 0;
    u16 buffOffset = 0;
    u16 pBuffOffset = 0;
    u16 leftToCopy = len;
    u8 *buffer = (u8*)(DMATxDescToSet->BUF1ADDR);
    
    if(DMATxDescToSet->CS & ETH_DMA_TDES_OWN) goto error;
    while ((leftToCopy + buffOffset) > ETH_TX_BUF_SIZE) {
        memcpy((u8*)(buffer + buffOffset), (u8*)(p + pBuffOffset), (ETH_TX_BUF_SIZE - buffOffset));
        DmaTxDesc = (ETH_DMADESCTypeDef*)DMATxDescToSet->BUF2NDADDR;
        if(DMATxDescToSet->CS & ETH_DMA_TDES_OWN) goto error;
        leftToCopy = leftToCopy - (ETH_TX_BUF_SIZE - buffOffset);
        pBuffOffset = pBuffOffset + (ETH_TX_BUF_SIZE - buffOffset);
        frameLen = frameLen + (ETH_TX_BUF_SIZE - buffOffset);
        buffOffset = 0;
        buffer = (u8*)(DmaTxDesc->BUF1ADDR);
    }
    memcpy((u8*)(buffer + buffOffset), (u8*)(p + pBuffOffset), leftToCopy);
    buffOffset = buffOffset + leftToCopy;
    frameLen = frameLen + leftToCopy;
    ETH_Prepare_Transmit_Descriptors(frameLen);
error:
    if (ETH->DMASR & ETH_DMASR_TUS) {
        ETH->DMASR &= ~ETH_DMASR_TUS;
        ETH->DMATPDR = 0;
    }
    return 0;
}
*/

/* send old */
//__IO ETH_DMADESCTypeDef *DmaTxDesc;
//u8 flagTemp = DmaTxDesc->CS & ETH_DMA_TDES_OWN;
//if(flagTemp)  return -1;

/* receive old */
//u16 i;
//FrameTypeDef frame;
//__IO ETH_DMADESCTypeDef *Rxdesc;
//if (!ETH_CheckFrameReceived()) return -1;
//frame = ETH_Get_Received_Frame();
//// Handle the receive Data
//receiveLen = frame.len;
//receiveStr = (u8 *)frame.buf;
//#if 0    
//if (loopBackFlag)
//sendBuffer(receiveStr, receiveLen);   //PHY_LoopBackCmd(phySelA, ENABLE);
//#endif
//// decode receive data
//if (receiveLen > 0){
//decodeFramDat(receiveStr, receiveLen);
//    }
//// Release descriptors to DMA
//Rxdesc = frame.ptrDesc;
//for (i = 0; i < DMA_RX_FRAME_infos->cnt; i++) {
//Rxdesc->CS |= ETH_DMA_RDES_OWN;
//Rxdesc = (ETH_DMADESCTypeDef *)(Rxdesc->BUF2NDADDR);
//    }
//DMA_RX_FRAME_infos->cnt = 0;
//if (ETH->DMASR & ETH_DMASR_RBUS) {
//ETH->DMASR = ETH_DMASR_RBUS; 
//ETH->DMARPDR = 0;
//    }
//return 0;

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
