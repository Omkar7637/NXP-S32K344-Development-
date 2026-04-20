/*******************************************************************************
*   Project      : MCSPTE1AK344_BLDC_6Step_hall_ll
*   Revision     : 1.2
*   RTD Version  : 3.0.0
*   Brief description  :
*   File contains declaration of application structures used for motor control.
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
#ifndef _MOTOR_STRUCTURE_H
#define _MOTOR_STRUCTURE_H

/******************************************************************************
* Includes
******************************************************************************/
#include "amclib.h"
#include "gdflib.h"
#include "gflib.h"
#include "gmclib.h"
#include "state_machine.h"

/*==================================================================================================
*                              STRUCTURES AND OTHER TYPEDEFS
==================================================================================================*/

/*------------------------------------------------------------------------*//*!
@brief  Structure containing position/speed module variables
*//*-------------------------------------------------------------------------*/
typedef union {
    uint16_t R;
    struct {
        uint16_t Alignment:1;
        uint16_t Sensorless:1;
        uint16_t StallCheckReq:1;
        uint16_t EnableCMT:1;
        uint16_t AfterCMT:1;
        uint16_t CloseLoop:1;
        uint16_t NewZC:1;
        uint16_t AdcSaved:1;
        uint16_t CurrentLimiting:1;
        uint16_t Fault:1;
        uint16_t Freewheeling:1;
        uint16_t Calib:1;
        uint16_t HallEvent:1;
        uint16_t Reserved:3;
    }B;
}tDriveStatus;

typedef union {
    uint8_t R;
    struct {
        uint8_t OverDCBusCurrent:1;
        uint8_t OverDCBusVoltage:1;
        uint8_t UnderDCBusVoltage:1;
        uint8_t PreDriverError:1;
        uint8_t StallError:1;
        uint8_t Reserved:3;
    }B;
}tFaultStatus;

typedef struct {
    tFloat BEMFVoltage;
    tFloat DCBVVoltage;
    tFloat DCBIVoltage;
    tFloat DCBIVoltageRaw;
    tFloat DCBIOffset;
}tADCresults;

typedef struct {
    uint8_t     InA;
    uint8_t     InB;
    uint8_t     InC;
    uint8_t     InABC;
    uint8_t     Sector;
    uint32_t    Period[6];
}tSensorHall;

typedef union
{
    tU16 R;
    struct
    {
        tU16 gd3000IntFlag          : 1;   /*  */
        tU16 gd3000ClearErr         : 1;   /*  */
        tU16 gd3000Error            : 1;   /*  */
        tU16 gd3000InitDone         : 1;
        tU16 Reserved               : 12;   /* RESERVED */
    }B;
}gd3000Status_t;

#endif /* _MOTOR_STRUCTURE */
