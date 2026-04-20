/*==================================================================================================
*   Project              : RTD AUTOSAR 4.7
*   Platform             : CORTEXM
*   Peripheral           : DMA,CACHE,TRGMUX,LCU,EMIOS,FLEXIO
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
*
*   NXP Confidential. This software is owned or controlled by NXP and may only be
*   used strictly in accordance with the applicable license terms. By expressly
*   accepting such terms or by downloading, installing, activating and/or otherwise
*   using the software, you are agreeing that you have read, and that you agree to
*   comply with and are bound by, such license terms. If you do not agree to be
*   bound by the applicable license terms, then you may not retain, install,
*   activate or otherwise use the software.
==================================================================================================*/
/* Prevention from multiple including the same header */
#ifndef LCU_IP_CFG_H_
#define LCU_IP_CFG_H_


/*******************************************************************************
 * Included files
 ******************************************************************************/

#include "Lcu_Ip_PBcfg.h"

/*==================================================================================================
                               SOURCE FILE VERSION INFORMATION
==================================================================================================*/
#define LCU_IP_CFG_VENDOR_ID                       43
#define LCU_IP_CFG_AR_RELEASE_MAJOR_VERSION        4
#define LCU_IP_CFG_AR_RELEASE_MINOR_VERSION        7
#define LCU_IP_CFG_AR_RELEASE_REVISION_VERSION     0
#define LCU_IP_CFG_SW_MAJOR_VERSION                3
#define LCU_IP_CFG_SW_MINOR_VERSION                0
#define LCU_IP_CFG_SW_PATCH_VERSION                0

/*==================================================================================================
                                      FILE VERSION CHECKS
==================================================================================================*/
/* Check if header file and Lcu_Ip_PBcfg.h file are of the same vendor */
#if (LCU_IP_CFG_VENDOR_ID != LCU_IP_PBCFG_VENDOR_ID)
    #error "Lcu_Ip_Cfg.h and Lcu_Ip_PBcfg.h have different vendor ids"
#endif
/* Check if header file and Lcu_Ip_PBcfg.h file are of the same Autosar version */
#if ((LCU_IP_CFG_AR_RELEASE_MAJOR_VERSION != LCU_IP_PBCFG_AR_RELEASE_MAJOR_VERSION) || \
     (LCU_IP_CFG_AR_RELEASE_MINOR_VERSION != LCU_IP_PBCFG_AR_RELEASE_MINOR_VERSION) || \
     (LCU_IP_CFG_AR_RELEASE_REVISION_VERSION != LCU_IP_PBCFG_AR_RELEASE_REVISION_VERSION) \
    )
    #error "AutoSar Version Numbers of Lcu_Ip_Cfg.h and Lcu_Ip_PBcfg.h are different"
#endif
/* Check if header file and Lcu_Ip_PBcfg.h file are of the same Software version */
#if ((LCU_IP_CFG_SW_MAJOR_VERSION != LCU_IP_PBCFG_SW_MAJOR_VERSION) || \
     (LCU_IP_CFG_SW_MINOR_VERSION != LCU_IP_PBCFG_SW_MINOR_VERSION) || \
     (LCU_IP_CFG_SW_PATCH_VERSION != LCU_IP_PBCFG_SW_PATCH_VERSION) \
    )
    #error "Software Version Numbers of Lcu_Ip_Cfg.h and Lcu_Ip_PBcfg.h are different"
#endif
    
/*===============================================================================================
                                       DEFINES AND MACROS
===============================================================================================*/

/* Number Of Configured Logic Instances */
#define LCU_IP_NOF_CFG_LOGIC_INSTANCES    ((uint32)2U)

/* Logic Instance 0 */
#define LCU_LOGIC_INSTANCE_0              ((uint32)0U)

/* Logic Instance 1 */
#define LCU_LOGIC_INSTANCE_1              ((uint32)1U)


/* Number Of Configured Logic Inputs */
#define LCU_IP_NOF_CFG_LOGIC_INPUTS       ((uint32)12U)

/* Logic Instance 0 */
#define LCU_LOGIC_INPUT_0                 ((uint32)0U)

/* Logic Instance 1 */
#define LCU_LOGIC_INPUT_1                 ((uint32)1U)

/* Logic Instance 2 */
#define LCU_LOGIC_INPUT_2                 ((uint32)2U)

/* Logic Instance 3 */
#define LCU_LOGIC_INPUT_3                 ((uint32)3U)

/* Logic Instance 4 */
#define LCU_LOGIC_INPUT_4                 ((uint32)4U)

/* Logic Instance 5 */
#define LCU_LOGIC_INPUT_5                 ((uint32)5U)

/* Logic Instance 6 */
#define LCU_LOGIC_INPUT_6                 ((uint32)6U)

/* Logic Instance 7 */
#define LCU_LOGIC_INPUT_7                 ((uint32)7U)

/* Logic Instance 8 */
#define LCU_LOGIC_INPUT_8                 ((uint32)8U)

/* Logic Instance 9 */
#define LCU_LOGIC_INPUT_9                 ((uint32)9U)

/* Logic Instance 10 */
#define LCU_LOGIC_INPUT_10                 ((uint32)10U)

/* Logic Instance 11 */
#define LCU_LOGIC_INPUT_11                 ((uint32)11U)


/* Number Of Configured Logic Outputs */
#define LCU_IP_NOF_CFG_LOGIC_OUTPUTS      ((uint32)10U)

/* Logic Instance 0 */
#define LCU_LOGIC_OUTPUT_0                ((uint32)0U)

/* Logic Instance 1 */
#define LCU_LOGIC_OUTPUT_1                ((uint32)1U)

/* Logic Instance 2 */
#define LCU_LOGIC_OUTPUT_2                ((uint32)2U)

/* Logic Instance 3 */
#define LCU_LOGIC_OUTPUT_3                ((uint32)3U)

/* Logic Instance 4 */
#define LCU_LOGIC_OUTPUT_4                ((uint32)4U)

/* Logic Instance 5 */
#define LCU_LOGIC_OUTPUT_5                ((uint32)5U)

/* Logic Instance 6 */
#define LCU_LOGIC_OUTPUT_6                ((uint32)6U)

/* Logic Instance 7 */
#define LCU_LOGIC_OUTPUT_7                ((uint32)7U)

/* Logic Instance 8 */
#define LCU_LOGIC_OUTPUT_8                ((uint32)8U)

/* Logic Instance 9 */
#define LCU_LOGIC_OUTPUT_9                ((uint32)9U)

#define MCL_START_SEC_CONFIG_DATA_UNSPECIFIED
/* @violates @ref Mcl_Lcu_h_REF_1 MISRA 2012 Required Directive 4.10, Precautions shall be taken in order to prevent the contents of a header file being included more than once. */
#include "Mcl_MemMap.h"
extern const Lcu_Ip_InitType Lcu_Ip_xLcuInitPB;
/* LCU Input De-Initialization Structure */
extern const Lcu_Ip_LogicInputConfigType Lcu_Ip_xLogicInputResetConfig;

/* LCU Output De-Initialization Structure */
extern const Lcu_Ip_LogicOutputConfigType Lcu_Ip_xLogicOutputResetConfig;

#define MCL_STOP_SEC_CONFIG_DATA_UNSPECIFIED
/* @violates @ref Mcl_Lcu_h_REF_1 MISRA 2012 Required Directive 4.10, Precautions shall be taken in order to prevent the contents of a header file being included more than once. */
#include "Mcl_MemMap.h"

#define MCL_START_SEC_CODE
/* @violates @ref Mcl_Lcu_h_REF_1 MISRA 2012 Required Directive 4.10, Precautions shall be taken in order to prevent the contents of a header file being included more than once. */
#include "Mcl_MemMap.h"

void Lcu0_Ip_IRQHandler(void);

void Lcu1_Ip_IRQHandler(void);

#define MCL_STOP_SEC_CODE
/* @violates @ref Mcl_Lcu_h_REF_1 MISRA 2012 Required Directive 4.10, Precautions shall be taken in order to prevent the contents of a header file being included more than once. */
#include "Mcl_MemMap.h"

#endif  /* #ifndef LCU_IP_CFG_H_ */

/*==================================================================================================
 *                                        END OF FILE
==================================================================================================*/

