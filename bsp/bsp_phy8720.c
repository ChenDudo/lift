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
#define _BSP_PHY8720_C_

// Files includes  -------------------------------------------------------------
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
void PHY8720_GPIO_Config(void)
{
    GPIO_InitTypeDef GPIO_InitStructure;

    COMMON_EnableIpClock(emCLOCK_GPIOA);
    COMMON_EnableIpClock(emCLOCK_GPIOB);
    COMMON_EnableIpClock(emCLOCK_GPIOC);
    COMMON_EnableIpClock(emCLOCK_GPIOD);
    COMMON_EnableIpClock(emCLOCK_GPIOE);
    COMMON_EnableIpClock(emCLOCK_EXTI);
    
    // REF_CLK; CRS_DV; RD0; RD1
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_1 | GPIO_Pin_7;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_FLOATING;
    GPIO_Init(GPIOA, &GPIO_InitStructure); 
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_4 | GPIO_Pin_5;
    GPIO_Init(GPIOC, &GPIO_InitStructure);
    
    // MDIO; TX_EN; TD0; TD1; MDC
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_2;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;
    GPIO_Init(GPIOA, &GPIO_InitStructure);
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_11 | GPIO_Pin_12 | GPIO_Pin_13;
    GPIO_Init(GPIOB, &GPIO_InitStructure); 
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_1;
    GPIO_Init(GPIOC, &GPIO_InitStructure);
    
    GPIO_PinAFConfig(GPIOA, GPIO_PinSource1, GPIO_AF_11);   // ETH_RX_CLK   PA1
    GPIO_PinAFConfig(GPIOA, GPIO_PinSource2, GPIO_AF_11);   // ETH_MDIO     PA2
    GPIO_PinAFConfig(GPIOA, GPIO_PinSource7, GPIO_AF_11);   // ETH_CRS_DV   PA7
    GPIO_PinAFConfig(GPIOB, GPIO_PinSource11,GPIO_AF_11);   // ETH_TX_EN    PB11
    GPIO_PinAFConfig(GPIOB, GPIO_PinSource12,GPIO_AF_11);   // ETH_TXD0     PB12
    GPIO_PinAFConfig(GPIOB, GPIO_PinSource13,GPIO_AF_11);   // ETH_TXD1     PB13
    GPIO_PinAFConfig(GPIOC, GPIO_PinSource1, GPIO_AF_11);   // ETH_MDC      PC1
    GPIO_PinAFConfig(GPIOC, GPIO_PinSource4, GPIO_AF_11);   // ETH_RXD0     PC4
    GPIO_PinAFConfig(GPIOC, GPIO_PinSource5, GPIO_AF_11);   // ETH_RXD1     PC5

    SYSCFG->CFGR2 |= 0X01<<20;   //select RMII
}

////////////////////////////////////////////////////////////////////////////////
void PHY_LoopBackCmd(uint16_t addr, FunctionalState sta)
{
    u16 temp_val;
    
    temp_val = ETH_ReadPHYRegister(addr, PHY_BCR);
    sta ? (temp_val |= PHY_Loopback) : (temp_val &= ~PHY_Loopback);
    ETH_WritePHYRegister(addr, PHY_BCR, temp_val);
}

////////////////////////////////////////////////////////////////////////////////
void closePhy_8720(u16 phyAddr)
{    
    ETH_WritePHYRegister(phyAddr, PHY_BCR, PHY_Powerdown);
}

////////////////////////////////////////////////////////////////////////////////
void changeAutoMDIX(u16 phyAddr, u8 mode)
{
    u16 temp = ETH_ReadPHYRegister(phyAddr, 27);
    
    temp |= 1 << 15;                                    // Disable AutoMDIX
    (mode) ? (temp &= ~(1 << 13)) : (temp |= 1 << 13);  // 0:Tx=Tx, 1:Tx=Rx
    ETH_WritePHYRegister(phyAddr, 27, temp);
}

////////////////////////////////////////////////////////////////////////////////
void writePhyConfig(u16 phy_addr)
{
    u16 readValue;
    
    if (ETH_InitStructure.ETH_AutoNegotiation != ETH_AutoNegotiation_Disable) {
        while (!(ETH_ReadPHYRegister(phy_addr, PHY_BSR) & PHY_Linked_Status));
        ETH_WritePHYRegister(phy_addr, PHY_BCR, PHY_AutoNegotiation);
        if(ETH_ReadPHYRegister(phy_addr, PHY_BSR) & PHY_AutoNego_Complete)
            phyANEG_Flag = true;
    }
    else
        ETH_WritePHYRegister(phy_addr, PHY_BCR, ((u16)(ETH_InitStructure.ETH_Mode >> 3) | (u16)(ETH_InitStructure.ETH_Speed >> 1) ));
    
    (ETH_Speed_10M == ETH_InitStructure.ETH_Speed) ? (SYSCFG->CFGR2 &= ~(1 << 21)) : (SYSCFG->CFGR2 |= (1 << 21));
    readValue = ETH_ReadPHYRegister(phy_addr, PHY_BSR);
    phyLink_Flag = (readValue & PHY_Linked_Status) ? true : false;
}

////////////////////////////////////////////////////////////////////////////////
void checkPhyStatus(u16 phy_addr)
{
    u16 readValue = 0;
    readValue = ETH_ReadPHYRegister(phy_addr, PHY_BSR);
    if(readValue == 0xFF){
        phyANEG_Flag = false;
        phyLink_Flag = false;
    }
    else{
        phyANEG_Flag = (readValue & PHY_AutoNego_Complete) ? true : false;
        phyLink_Flag = (readValue & PHY_Linked_Status) ? true : false;
    }
#if CHECK_PHY_SPEED
    readValue = ETH_ReadPHYRegister(phy_addr, PHY_SCSR);
    if(readValue == 0xFF){
        phyFull_Flag = false;
        phy100M_Flag = false;
    }
    else{
        phyFull_Flag = (readValue & PHY_DUPLEX_STATUS) ? true : false;
        phy100M_Flag = (readValue & PHY_SPEED_STATUS) ? true : false;
    }
#endif
#if DELAY_CHECK_PHY
    static u16 i = 0;
    switch(i){
        case 10:
        readValue = ETH_ReadPHYRegister(phy_addr, PHY_BSR);
        if(readValue == 0xFF){
            phyANEG_Flag = false;
            phyLink_Flag = false;
        }
        else{
            phyANEG_Flag = (readValue & PHY_AutoNego_Complete) ? true : false;
            phyLink_Flag = (readValue & PHY_Linked_Status) ? true : false;
        }
        break;
        case 20:
        readValue = ETH_ReadPHYRegister(phy_addr, PHY_SCSR);
        if(readValue == 0xFF){
            phyFull_Flag = false;
            phy100M_Flag = false;
        }
        else{
            phyFull_Flag = (readValue & PHY_DUPLEX_STATUS) ? true : false;
            phy100M_Flag = (readValue & PHY_SPEED_STATUS) ? true : false;
        }
        i = 0;
        break;
        default:
        break;
    }
    i++;
#endif
}

////////////////////////////////////////////////////////////////////////////////
void DeInitPhy_8720(u16 phy_addr)
{
    changeAutoMDIX(phy_addr, 0);
    
    if (ETH_InitStructure.ETH_AutoNegotiation != ETH_AutoNegotiation_Disable) {
        while (!(ETH_ReadPHYRegister(phy_addr, PHY_BSR) & PHY_Linked_Status));
        ETH_WritePHYRegister(phy_addr, PHY_BCR, PHY_AutoNegotiation);
        if(ETH_ReadPHYRegister(phy_addr, PHY_BSR) & PHY_AutoNego_Complete)
            phyANEG_Flag = true;
    }
    else
        ETH_WritePHYRegister(phy_addr, PHY_BCR, ((u16)(ETH_InitStructure.ETH_Mode >> 3) | (u16)(ETH_InitStructure.ETH_Speed >> 1) ));
    (ETH_Speed_10M == ETH_InitStructure.ETH_Speed) ? (SYSCFG->CFGR2 &= ~(1 << 21)) : (SYSCFG->CFGR2 |= (1 << 21));
}

u16 readPhyValue;
////////////////////////////////////////////////////////////////////////////////
void initPhy_8720(u16 phy_addr)
{
    readPhyValue = ETH_ReadPHYRegister(phy_addr, PHY_BCR);
    ETH_WritePHYRegister(phy_addr, PHY_BCR, 0x2100);
}

////////////////////////////////////////////////////////////////////////////////
void changePhy(u8 mode)
{
    switch(mode){
        case 0:
        closePhy_8720(PHY_ADDRESS_LAN8720_B);
        initPhy_8720(PHY_ADDRESS_LAN8720_A);
        checkPhyStatus(PHY_ADDRESS_LAN8720_A);
        break;
        case 1:
        closePhy_8720(PHY_ADDRESS_LAN8720_A);
        initPhy_8720(PHY_ADDRESS_LAN8720_B);
        checkPhyStatus(PHY_ADDRESS_LAN8720_B);
        break;
        default:
        closePhy_8720(PHY_ADDRESS_LAN8720_A);
        closePhy_8720(PHY_ADDRESS_LAN8720_B);
        break;
    }
}

////////////////////////////////////////////////////////////////////////////////
void BSP_PHY8720_Configure()
{
    //changePhy(mode);
    DeInitPhy_8720(PHY_ADDRESS_LAN8720_A);
    DeInitPhy_8720(PHY_ADDRESS_LAN8720_B);
    closePhy_8720(PHY_ADDRESS_LAN8720_B);
}

/// @}

/// @}

/// @}
