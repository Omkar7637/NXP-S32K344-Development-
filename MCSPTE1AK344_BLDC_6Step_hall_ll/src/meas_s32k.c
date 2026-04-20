/*******************************************************************************
*   Project      : MCSPTE1AK344_BLDC_6Step_hall_ll
*   Revision     : 1.2
*   RTD Version  : 3.0.0
*   Brief description  :
*   File contains definition of functions related to analog quantities capturing.
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
#include "meas_s32k.h"

/*==================================================================================================
*                                      GLOBAL VARIABLES
==================================================================================================*/
extern volatile Bctu_Ip_FifoResultType measuredValues[16];

/*==================================================================================================
*                                    FUNCTIONS DEFINITION
==================================================================================================*/

/*FUNCTION**********************************************************************
 *
 * Function Name : MEAS_GetIdcCurrent
 * Description   : This function performs measurement of DCB currents from
            	   the shunt resistor.
 *
 *
 *END**************************************************************************/
tBool MEAS_GetIdcCurrent(tFloat *ptr)
{
    uint16_t DCBus_Current = 0;

    DCBus_Current = measuredValues[0].AdcData;
    *ptr = MLIB_Mul(((tFloat)MLIB_Div((tFloat)(DCBus_Current & 0x00003FFF), (tFloat)0x00003FFF)), I_MAX * 2.0F);

    return (TRUE);
}

/*FUNCTION**********************************************************************
 *
 * Function Name : MEAS_GetUdcVoltage
 * Description   : This function performs measurement of DCBus Voltage.
 *
 *END**************************************************************************/
tBool MEAS_GetUdcVoltage(tFloat *ptr)
{
    uint16_t DCBus_Voltage= 0;
    DCBus_Voltage = measuredValues[1].AdcData;
    *ptr = MLIB_Mul(((tFloat)MLIB_Div((tFloat)(DCBus_Voltage & 0x00003FFF), (tFloat)0x00003FFF)), U_DCB_MAX);

    return (TRUE);
}
