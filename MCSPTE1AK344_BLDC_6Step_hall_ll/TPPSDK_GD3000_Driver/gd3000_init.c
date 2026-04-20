/*******************************************************************************
*
* Copyright 2006-2015 Freescale Semiconductor, Inc.
* Copyright 2016-2021 NXP
*
****************************************************************************//*!
*
* @file     gd3000_init.c
*
* @date     Jul-9-2021
*
* @brief    MC34GD3000 Initialization
*
*******************************************************************************/
/*******************************************************************************
* Includes
*******************************************************************************/
#include "gd3000_init.h"

/*******************************************************************************
*
* Function:     status_t GD3000_Init(void)
*
* Description:  This function initialize MC34GD3000 MOSFET pre-driver.
*               MC34GD3000 SW driver uses S32K344 LPSPI1 module as a communication
*               interface to configure MC34GD3000 operation mode and to track MC34GD3000
*               Status0/Status1 registers.
*
*******************************************************************************/
status_t GD3000_Init(void)
{
    /* Local Variables */
    status_t GDInitErr = kStatus_Success;

   /* GD3000 pin configuration */
    tppDrvConfig.en1PinIndex    = AML_GD_EN1EN2_PIN;
    tppDrvConfig.en1PinInstance = AML_GD_EN1EN2_PORT;
    tppDrvConfig.en2PinIndex    = AML_GD_EN1EN2_PIN;
    tppDrvConfig.en2PinInstance = AML_GD_EN1EN2_PORT;
    tppDrvConfig.rstPinIndex    = AML_GD_RST_PIN;
    tppDrvConfig.rstPinInstance = AML_GD_RST_PORT;

    /* GD3000 device configuration */
    tppDrvConfig.deviceConfig.deadtime =    INIT_DEADTIME;
    tppDrvConfig.deviceConfig.intMask0 =    INIT_INTERRUPTS0;
    tppDrvConfig.deviceConfig.intMask1 =    INIT_INTERRUPTS1;
    tppDrvConfig.deviceConfig.modeMask =    INIT_MODE;

    /* GD3000 status registers configuration */
    tppDrvConfig.deviceConfig.statusRegister[0U] = 0U;
    tppDrvConfig.deviceConfig.statusRegister[1U] = 0U;
    tppDrvConfig.deviceConfig.statusRegister[2U] = 0U;
    tppDrvConfig.deviceConfig.statusRegister[3U] = 0U;

    /* GD3000 SPI configuration */
    tppDrvConfig.csPinIndex = AML_GD_SPI_PCS_PIN;
    tppDrvConfig.csPinInstance = AML_GD_SPI_PCS_PORT;
#if (SDK_VERSION == KINETIS_SDK_2_0 || SDK_VERSION == S32K1_SDK_3_0_0)
    /* Initialize Peripherals:
     * The code below only applies to Kinetis SDK and S32K1 SDK.
     * When S32K3 RTD is used, the peripherals (GPIO and SPI) are configured
     * through RTD APIs. */
    tppDrvConfig.spiInstance = AML_GD_SPI_INST;
    tppDrvConfig.spiTppConfig.baudRateHz =    LPSPI_FREQ;
    tppDrvConfig.spiTppConfig.sourceClockHz = 4000000U;
    TPP_ConfigureGpio(&tppDrvConfig);
    TPP_ConfigureSpi(&tppDrvConfig, NULL);
#endif

    GDInitErr = TPP_Init(&tppDrvConfig, tppModeEnable);

    return GDInitErr;
}
