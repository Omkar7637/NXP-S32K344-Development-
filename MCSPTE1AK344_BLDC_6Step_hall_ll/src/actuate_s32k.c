/*******************************************************************************
*   Project      : MCSPTE1AK344_BLDC_6Step_hall_ll
*   Revision     : 1.2
*   RTD Version  : 3.0.0
*   Brief description  :
*   File contains definition of functions needed for inverter voltage control.
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
/******************************************************************************
| Includes
-----------------------------------------------------------------------------*/
#include "actuate_s32k.h"
#include "gflib.h"
#include "gdflib.h"
#include "gmclib.h"
#include "Emios_Mcl_Ip_PBcfg.h"


/*==================================================================================================
*                                    FUNCTIONS DEFINITION
==================================================================================================*/

/**************************************************************************//*!
@brief Unmask PWM output and set 50% dutycyle

@param[in,out]  

@return
******************************************************************************/

/*FUNCTION**********************************************************************
 *
 * Function Name : ACTUATE_EnableOutput
 * Description   : It unmasks PWM output and set 1% dutycyle.
 *
 *END**************************************************************************/
tBool ACTUATE_EnableOutput(void)
{
    tBool statePWM;
    Lcu_Ip_SyncOutputValueType lcuEnable[6U];
    /* Apply 0.01 duty cycle */
    statePWM = ACTUATE_SetDutycycle(0.01F);
    lcuEnable[0].LogicOutputId = LCU_LOGIC_OUTPUT_0;
    lcuEnable[0].Value = 1U;
    lcuEnable[1].LogicOutputId = LCU_LOGIC_OUTPUT_1;
    lcuEnable[1].Value = 1U;
    lcuEnable[2].LogicOutputId = LCU_LOGIC_OUTPUT_2;
    lcuEnable[2].Value = 1U;
    lcuEnable[3].LogicOutputId = LCU_LOGIC_OUTPUT_3;
    lcuEnable[3].Value = 1U;
    lcuEnable[4].LogicOutputId = LCU_LOGIC_OUTPUT_4;
    lcuEnable[4].Value = 1U;
    lcuEnable[5].LogicOutputId = LCU_LOGIC_OUTPUT_5;
    lcuEnable[5].Value = 1U;
    Lcu_Ip_SetSyncOutputEnable(lcuEnable, 6U);

    return (statePWM);
}


/*FUNCTION**********************************************************************
 *
 * Function Name : ACTUATE_DisableOutput
 * Description   : It masks PWM output and set 1% dutycyle.
 *
 *END**************************************************************************/
tBool ACTUATE_DisableOutput(void)
{
    tBool statePWM;
    Lcu_Ip_SyncOutputValueType lcuDisable[6U];
    lcuDisable[0].LogicOutputId = LCU_LOGIC_OUTPUT_0;
    lcuDisable[0].Value = 0U;
    lcuDisable[1].LogicOutputId = LCU_LOGIC_OUTPUT_1;
    lcuDisable[1].Value = 0U;
    lcuDisable[2].LogicOutputId = LCU_LOGIC_OUTPUT_2;
    lcuDisable[2].Value = 0U;
    lcuDisable[3].LogicOutputId = LCU_LOGIC_OUTPUT_3;
    lcuDisable[3].Value = 0U;
    lcuDisable[4].LogicOutputId = LCU_LOGIC_OUTPUT_4;
    lcuDisable[4].Value = 0U;
    lcuDisable[5].LogicOutputId = LCU_LOGIC_OUTPUT_5;
    lcuDisable[5].Value = 0U;
    Lcu_Ip_SetSyncOutputEnable(lcuDisable, 6U);
    /* Apply 0.01 duty cycle */
    statePWM = ACTUATE_SetDutycycle(0.01F);
    
    return (statePWM);
}

/*FUNCTION**********************************************************************
 *
 * Function Name : ACTUATE_SetDutycycle
 * Description   : It sets PWM dutycyle, the dutycyle will by updated on next reload event.
 *
 *END**************************************************************************/
tBool ACTUATE_SetDutycycle(tFloat fltpwm)
{
    tBool statePwm = TRUE;
    /*pwm timebase period generated by config tool*/
    uint16 pwmPeriodHalf =(uint16) (Emios_Mcl_Ip_0_MasterBusConfig_BOARD_INITPERIPHERALS[0].defaultPeriod >> 1);
    float pwmPeriod =(float) Emios_Mcl_Ip_0_MasterBusConfig_BOARD_INITPERIPHERALS[0].defaultPeriod;

    Emios_Mcl_Ip_ComparatorTransferDisable(0U,(uint32_t)0b10U);

    uint16 pwmShiftA = (pwmPeriodHalf - (uint16_t)((fltpwm) * (float)pwmPeriodHalf));
    uint16 pwmDutyA= (uint16_t)((fltpwm) * (float)pwmPeriod);

    Emios_Pwm_Ip_UpdateUCRegA(0U, 1U, pwmShiftA);
    Emios_Pwm_Ip_UpdateUCRegB(0U, 1U, pwmShiftA+pwmDutyA);

    Emios_Mcl_Ip_ComparatorTransferEnable(0U,(uint32_t)0b10U);

    return (statePwm);
}
