/*==================================================================================================
*   Project              : RTD AUTOSAR 4.7
*   Platform             : CORTEXM
*   Peripheral           : Emios Siul2 Wkpu LpCmp
*   Dependencies         : none
*
*   Autosar Version      : 4.7.0
*   Autosar Revision     : ASR_REL_4_7_REV_0000
*   Autosar Conf.Variant :
*   SW Version           : 3.0.0
*   Build Version        : S32K3_RTD_3_0_0_D2303_ASR_REL_4_7_REV_0000_20230331
*
*   Copyright 2020 - 2023 NXP Semiconductors
*
*   NXP Confidential. This software is owned or controlled by NXP and may only be
*   used strictly in accordance with the applicable license terms. By expressly
*   accepting such terms or by downloading, installing, activating and/or otherwise
*   using the software, you are agreeing that you have read, and that you agree to
*   comply with and are bound by, such license terms. If you do not agree to be
*   bound by the applicable license terms, then you may not retain, install,
*   activate or otherwise use the software.
==================================================================================================*/

/**
 *   @file    Emios_Icu_Ip_SA_PBCfg.c
 *   @version 3.0.0
 *
 *   @brief   AUTOSAR Icu - contains the data exported by the ICU module.
 *   @details Contains the information that will be exported by the module, as requested by AUTOSAR.
 *
 *   @addtogroup emios_icu_ip EMIOS IPL
 *   @{
 */

#ifdef __cplusplus
extern "C"{
#endif

/*==================================================================================================
*                                         INCLUDE FILES
* 1) system and project includes
* 2) needed interfaces from external units
* 3) internal and external interfaces from this unit
*================================================================================================*/
#include "Emios_Icu_Ip_Types.h"

#if (STD_ON == EMIOS_ICU_IP_USED)
    #if ((STD_ON == EMIOS_ICU_IP_SIGNALMEASUREMENT_USES_DMA_IPL) || (STD_ON == EMIOS_ICU_IP_TIMESTAMP_USES_DMA_IPL))
        #include "Dma_Ip.h"
    #endif
#endif


/*==================================================================================================
 *                              SOURCE FILE VERSION INFORMATION
 *================================================================================================*/
#define EMIOS_ICU_IP_SA_PBCFG_VENDOR_ID_C                    43
#define EMIOS_ICU_IP_SA_PBCFG_AR_RELEASE_MAJOR_VERSION_C     4
#define EMIOS_ICU_IP_SA_PBCFG_AR_RELEASE_MINOR_VERSION_C     7
#define EMIOS_ICU_IP_SA_PBCFG_AR_RELEASE_REVISION_VERSION_C  0
#define EMIOS_ICU_IP_SA_PBCFG_SW_MAJOR_VERSION_C             3
#define EMIOS_ICU_IP_SA_PBCFG_SW_MINOR_VERSION_C             0
#define EMIOS_ICU_IP_SA_PBCFG_SW_PATCH_VERSION_C             0

/*==================================================================================================
 *                                      FILE VERSION CHECKS
 *================================================================================================*/
#ifndef DISABLE_MCAL_INTERMODULE_ASR_CHECK
    #if (STD_ON == EMIOS_ICU_IP_USED)
        #if ((STD_ON == EMIOS_ICU_IP_SIGNALMEASUREMENT_USES_DMA_IPL) || (STD_ON == EMIOS_ICU_IP_TIMESTAMP_USES_DMA_IPL))
            /* Check if header file and Dma_Ip.h file are of the same Autosar version */
            #if ((EMIOS_ICU_IP_SA_PBCFG_AR_RELEASE_MAJOR_VERSION_C != DMA_IP_AR_RELEASE_MAJOR_VERSION) || \
                 (EMIOS_ICU_IP_SA_PBCFG_AR_RELEASE_MINOR_VERSION_C != DMA_IP_AR_RELEASE_MINOR_VERSION))
                #error "AutoSar Version Numbers of Emios_Icu_Ip_SA_PBcfg.c and Dma_Ip.h are different"
            #endif
        #endif
    #endif
#endif 
 
#if (EMIOS_ICU_IP_SA_PBCFG_VENDOR_ID_C != EMIOS_ICU_IP_TYPES_VENDOR_ID)
    #error "Emios_Icu_Ip_SA_PBcfg.c and Emios_Icu_Ip_Types.h have different vendor ids"
#endif

/* Check if  source file and Emios_Icu_Ip_Types.h file are of the same Autosar version */
#if ((EMIOS_ICU_IP_SA_PBCFG_AR_RELEASE_MAJOR_VERSION_C != EMIOS_ICU_IP_TYPES_AR_RELEASE_MAJOR_VERSION) || \
     (EMIOS_ICU_IP_SA_PBCFG_AR_RELEASE_MINOR_VERSION_C != EMIOS_ICU_IP_TYPES_AR_RELEASE_MINOR_VERSION) || \
     (EMIOS_ICU_IP_SA_PBCFG_AR_RELEASE_REVISION_VERSION_C != EMIOS_ICU_IP_TYPES_AR_RELEASE_REVISION_VERSION))
    #error "AutoSar Version Numbers of Emios_Icu_Ip_SA_PBcfg.c and Emios_Icu_Ip_Types.h are different"
#endif

/* Check if source file and Emios_Icu_Ip_Types.h file are of the same Software version */
#if ((EMIOS_ICU_IP_SA_PBCFG_SW_MAJOR_VERSION_C != EMIOS_ICU_IP_TYPES_SW_MAJOR_VERSION) || \
     (EMIOS_ICU_IP_SA_PBCFG_SW_MINOR_VERSION_C != EMIOS_ICU_IP_TYPES_SW_MINOR_VERSION) || \
     (EMIOS_ICU_IP_SA_PBCFG_SW_PATCH_VERSION_C != EMIOS_ICU_IP_TYPES_SW_PATCH_VERSION))
    #error "Software Version Numbers of Emios_Icu_Ip_SA_PBcfg.c and Emios_Icu_Ip_Types.h are different"
#endif

/*==================================================================================================
 *                                       GLOBAL FUNCTIONS
 *================================================================================================*/
#if (STD_ON == EMIOS_ICU_IP_USED)

#define ICU_START_SEC_CODE
#include "Icu_MemMap.h"

extern void eMIOS1IcuNotify(void);
#define ICU_STOP_SEC_CODE
#include "Icu_MemMap.h"
/*==================================================================================================
 *                                       GLOBAL CONSTANTS
 *================================================================================================*/
#define ICU_START_SEC_CONFIG_DATA_UNSPECIFIED
#include "Icu_MemMap.h"

/** @brief Variant BOARD_InitPeripherals, instance 1 EMIOS Channels Configuration */
const eMios_Icu_Ip_ChannelConfigType eMios_Icu_Ip_1_ChannelConfig_PB[1U] =
{
        /** @brief IcueMios_0_Channel_0 */
        {
        /** @brief EMIOS hardware channel used by the ICU driver. */
        1U,
        /** @brief Channel DutyCycle measurement mode. */
        EMIOS_ICU_SAIC,
        /** @brief Emios Channel Freeze bit. */
        (boolean)FALSE,
        /** @brief Value of EMIOS channel prescaler. */
        EMIOS_PRESCALER_DIVIDE_1,
        /** @brief Value of EMIOS channel alternate prescaler. */
        EMIOS_PRESCALER_DIVIDE_1,
        /** @brief Channel Counter bus selection. */
        EMIOS_ICU_BUS_A,
        /** @brief eMios IPL mode of operation */
        EMIOS_ICU_MODE_TIMESTAMP,
        /** @brief DMA support for timestamp measurement. */
        EMIOS_ICU_MODE_WITHOUT_DMA,
        /** @brief Measurement mode for signal measurement. */
        EMIOS_ICU_NO_MEASUREMENT,
        /** @brief Edge alignment for signal measurement. */
        EMIOS_ICU_BOTH_EDGES,
        /** @brief Value of EMIOS channel digital filter. */
        EMIOS_DIGITAL_FILTER_02,
        /** @brief Callback function for channels. */
        NULL_PTR,
        /** @brief Callback function for changing channels status */
        NULL_PTR,
        /** @brief Parameters used by callback function. */
        (uint8)255U,
        /** @brief Channel perform measurements without interrupt. */
        (boolean)FALSE,
#if (STD_ON == EMIOS_ICU_IP_TIMESTAMP_API)
        /** @brief Timestamp buffer used. */
        EMIOS_ICU_CIRCULAR_BUFFER,
#endif
#if ((STD_ON == EMIOS_ICU_IP_SIGNALMEASUREMENT_USES_DMA_IPL) || (STD_ON == EMIOS_ICU_IP_TIMESTAMP_USES_DMA_IPL))
        /** @brief Dma channel Id. */
        255U,
#endif
        /** @brief Channel specific notification */
        &eMIOS1IcuNotify,
        /** @brief Channel overflow notification. */
        NULL_PTR
    }
};

/** @brief Emios PB instance 1 IP configuration. */
const eMios_Icu_Ip_ConfigType eMios_Icu_Ip_1_Config_PB = 
{
    /** @brief Number of channels in the configuration */
    (uint8) 1U,
    /** @brief Instance Channels Config */
    &eMios_Icu_Ip_1_ChannelConfig_PB
};

#define ICU_STOP_SEC_CONFIG_DATA_UNSPECIFIED
#include "Icu_MemMap.h"

#endif  /* EMIOS_ICU_IP_USED */

#ifdef __cplusplus
}
#endif

/** @} */

