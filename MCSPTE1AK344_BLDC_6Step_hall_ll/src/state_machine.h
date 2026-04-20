/*******************************************************************************
*   Project      : MCSPTE1AK344_BLDC_6Step_hall_ll
*   Revision     : 1.2
*   RTD Version  : 3.0.0
*   Brief description  :
*   File contains declaration of structures and functions needed
*   for application state machine.
*
*   Copyright 2023 NXP Semiconductors
*   All Rights Reserved.
*
* This software is owned or controlled by NXP Semiconductors.
* Use of this software is governed by the NXP LA_OPT_NXP_Software_License
* distributed with this Material.
* See the LA_OPT_NXP_Software_License file distributed for more details.
*
* Brief License Summary:
* This software is provided in source form for you to use free of charge,
* but it is not open source software. You are allowed to use this software
* and derivative works of it in source form.
* The software may be used only in connection with a product containing
* a NXP microprocessor, microcontroller, or digital signal processor.
* See license agreement file for full license terms including other
* restrictions.
*
* THIS SOFTWARE IS PROVIDED BY NXP "AS IS" AND ANY EXPRESSED OR
* IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES
* OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE DISCLAIMED.
* IN NO EVENT SHALL NXP OR ITS CONTRIBUTORS BE LIABLE FOR ANY DIRECT,
* INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES
* (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR
* SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION)
* HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT,
* STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING
* IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF
* THE POSSIBILITY OF SUCH DAMAGE.
*******************************************************************************/
#ifndef _STATE_MACHINE_FRAME_H
#define _STATE_MACHINE_FRAME_H

/*==================================================================================================
*                                       LOCAL MACROS
==================================================================================================*/
#define APP_INIT                0   /* Application states */
#define APP_CALIB               1
#define APP_ALIGNMENT           2
#define APP_START               3
#define APP_RUN                 4
#define APP_STOP                5
#define APP_FAULT               6

/*==================================================================================================
*                              STRUCTURES AND OTHER TYPEDEFS
==================================================================================================*/
typedef void (*tPointerFcn)(void);  /* pointer to a function */
typedef void (*tPointerStr)(void);  /* pointer to a structure */

/*------------------------------------------------------------------------*//*!
@brief  Exported Variables
*//*-------------------------------------------------------------------------*/
/* Array with pointers to the state machine functions */
extern const tPointerFcn AppStateMachine[];
/* Array with pointers to the RGB Led state functions */
extern const tPointerFcn AppStateLed[];

/*==================================================================================================
                                    FUNCTION PROTOTYPES
==================================================================================================*/
#define APP_START_SEC_CODE_ITCM
#include "App_MemMap.h"

/* Application control*/
extern void AppInit(void);
extern void AppCalib(void);
extern void AppAlignment(void);
extern void AppStart(void);
extern void AppRun(void);
extern void AppStop(void);
extern void AppFault(void);

extern void AppStopToAlignment(void);
extern void AppAlignmentToStart(void);
extern void AppStartToRun(void);

extern void CheckFaults(void);
extern void CheckSwitchState(void);
extern void StallCheck(void);

#define APP_STOP_SEC_CODE_ITCM
#include "App_MemMap.h"

/* LED control*/
extern void StateRGBLedOFF(void);
extern void StateRGBLedBlueON(void);
extern void StateRGBLedRedON(void);
extern void StateRGBLedGreenON(void);
extern void StateRGBLedGreenFlashing(void);

#endif //_STATE_MACHINE_FRAME_H
