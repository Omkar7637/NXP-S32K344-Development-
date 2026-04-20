/*******************************************************************************
*   Project      : MCSPTE1AK344_BLDC_6Step_hall_ll
*   Revision     : 1.2
*   RTD Version  : 3.0.0
*   Brief description  :
*   File contains definition of the application state machine.
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
/*==================================================================================================
*                                        INCLUDE FILES
==================================================================================================*/
#include "state_machine.h"

/*==================================================================================================
*                                      GLOBAL VARIABLES
==================================================================================================*/

/* Array with pointers to the state machine functions */
const tPointerFcn AppStateMachine[] = \
{
    AppInit,                    // #0
    AppCalib,                   // #1
    AppAlignment,               // #2
    AppStart,                   // #3
    AppRun,                     // #4
    AppStop,                    // #5
    AppFault                    // #6
};


/* Array with pointers to the RGB Led state functions */
const tPointerFcn AppStateLed[] = \
{
    StateRGBLedOFF,                 // #0
    StateRGBLedGreenFlashing,       // #1
    StateRGBLedGreenFlashing,       // #2
    StateRGBLedGreenFlashing,       // #3
    StateRGBLedBlueON,              // #4
    StateRGBLedGreenON,             // #5
    StateRGBLedRedON                // #6
};
