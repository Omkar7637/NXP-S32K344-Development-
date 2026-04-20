/*******************************************************************************
*   Project      : MCSPTE1AK344_BLDC_6Step_hall_ll
*   Revision     : 1.2
*   RTD Version  : 3.0.0
*   Brief description  :
*   File contains definition of
*   - application entry function (main)
*   - interrupt service routines
*   - state machine functions
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
#include "freemaster.h"
#include "freemaster_s32k3xx_lpuart.h"
#include "actuate_s32k.h"
#include "Adc_Sar_Ip.h"
#include "amclib.h"
#include "Bctu_Ip.h"
#include "Clock_Ip.h"
#include "Emios_Mcl_Ip.h"
#include "Emios_Icu_Ip.h"
#include "Emios_Pwm_Ip.h"
#include "gd3000_init.h"
#include "IntCtrl_Ip.h"
#include "Lcu_Ip.h"
#include "Lpspi_Ip.h"
#include "Lpuart_Uart_Ip.h"
#include "Mcal.h"
#include "meas_s32k.h"
#include "motor_structure.h"
#include "Pit_Ip.h"
#include "Siul2_Port_Ip.h"
#include "Siul2_Dio_Ip.h"
#include "Siul2_Icu_Ip.h"
#include "state_machine.h"
#include "Trgmux_Ip.h"
#include "config/BLDC_appconfig.h"

/*==================================================================================================
*                                       LOCAL MACROS
==================================================================================================*/
#define ROTATION_DIR_CW                 0
#define ROTATION_DIR_CCW                1

/* ADVANCE_ANGLE' = 0.5 * ADVANCED_ANGLE  */
#define ADVANCE_ANGLE                   FRAC16(0.3815)

/* Duty cycle limit for DC bus current measurement */
#define DC_THRESHOLD                    10.0F

/* DC Bus Voltage MA filter defined by Lambda */
#define DCBV_FILTER_MA_LAMBDA            0.25F
/* DC Bus Current Offset MA filter defined by Lambda */
#define CALIB_FILTER_MA_LAMBDA           0.001F
/* Wait 0.5s to settle DC bus current offset
 * CALIB_TIMER = PWM freq/2Hz = 20kHz/2Hz */
#define CALIB_TIMER                      10000

/* Speed increase step [RPM] */
#define SPEED_INC                        100.0F
/* Speed decrease step [RPM] */
#define SPEED_DEC                        100.0F
/* Maximal speed [RPM] */
#define SPEED_MAX                        2000.0F

/* Maximum number of stall check errors */
#define STALLCHECK_MAX_ERRORS            6
/* Minimal stall commutation period */
/* 20KRPM => 125us => 156.25 @625kHz */
#define STALLCHECK_MIN_CMT_PERIOD        156

/* User switch debounce timeout */
#define SW_PRESS_DEBOUNCE                75
/* User switch input blocking delay */
#define SW_PRESS_OFF                     250
/* User LED flashing period */
#define LED_FLASH_FREQ                   80000

/*==================================================================================================
*                                      GLOBAL VARIABLES
==================================================================================================*/
#define APP_START_SEC_VAR_INIT_UNSPECIFIED_DTCM
#include "App_MemMap.h"

/* Temperature measurement */
uint16 TemperatureRaw;
volatile tFloat MeasTempFlt;
Adc_Sar_Ip_StatusType TemperatureGetStatus;

/* Motor control measurements */
Bctu_Ip_FifoResultType measuredValues[16];
uint8_t mCount = 0;

volatile int exit_code = 0;

tFloat speed_fb = 0.0F;
Lcu_Ip_SyncInputValueType InputCW = {LCU_LOGIC_INPUT_11, 0U};
Lcu_Ip_SyncInputValueType InputCCW = {LCU_LOGIC_INPUT_11, 1U};
Lcu_Ip_SyncInputValueType InputSector[4] =
{
    {LCU_LOGIC_INPUT_8, 0U},
    {LCU_LOGIC_INPUT_9, 0U},
    {LCU_LOGIC_INPUT_10, 0U},
    {LCU_LOGIC_INPUT_11, 0U}
};

Lcu_Ip_SyncInputValueType Alignment[6] =
{
    {LCU_LOGIC_INPUT_0, 0U},
    {LCU_LOGIC_INPUT_1, 0U},
    {LCU_LOGIC_INPUT_2, 0U},
    {LCU_LOGIC_INPUT_4, 0U},
    {LCU_LOGIC_INPUT_5, 0U},
    {LCU_LOGIC_INPUT_6, 0U}
};

/* Application State and Control Variables */
uint8_t      appState = APP_INIT;
uint8_t      rotationDir = ROTATION_DIR_CCW;
uint8_t      appSwitchState = 0, faultSwitchClear;
int16_t      switchCounter[2], switchOffCounter;
uint32_t     ledCounter;
tDriveStatus driveStatus;
tFaultStatus faultStatus, faultStatusLatched;

/* Measurement/Actuate Variables */
tADCresults  ADCResults;
tFloat       duty_cycle;

/* BEMF Zero Cross Detection and SixStep Commutation control Variables */
uint32_t     period6ZC;
uint16_t     calibTimer, alignmentTimer;
tSensorHall  SensorHall;

/* Speed and Current Control Loop Variables */
tFloat       torqueErr;
tFloat       speedErr;
tFloat       requiredSpeed = N_MIN;
tFloat       requiredSpeedRamp;
tFloat       actualSpeed = 0.0F;
tFloat       speedPIOut, currentPIOut;
tFloat       u_dc_bus_filt, torque_filt;

GFLIB_CONTROLLER_PIAW_P_T_FLT speedPIPrms, currentPIPrms;
GFLIB_RAMP_T_FLT              speedRampPrms;
GDFLIB_FILTER_MA_T_FLT        Idcb_filt,Udcb_filt, Idcb_calib,speed_filt;

/*------------------------------------
 * MCAT - Referenced Variables
 * ----------------------------------*/

/* MCAT Parameters tab */
tFloat    mcat_alignVoltage    = ALIGN_VOLTAGE;
uint16_t  mcat_alignDuration   = ALIGN_DURATION;

/* MCAT Sensorless tab */
tFloat    mcat_NMin            = N_MIN;
uint16_t  mcat_FreewheelTLong  = FREEWHEEL_T_LONG;
uint16_t  mcat_FreewheelTShort = FREEWHEEL_T_SHORT;
uint8_t   mcat_startCmtCnt     = STARTUP_CMT_CNT;
uint16_t  mcat_startCmtPer     = STARTUP_CMT_PER;
tFloat    mcat_startCmtAcceler = START_CMT_ACCELER;
tFloat    mcat_cmtTOff         = CMT_T_OFF;
tFloat    mcat_integThr        = INTEG_TRH;

/*  FreeMASTER constants */
tU32   fm_voltage;
tU32   fm_current;
tU32   fm_speed;
eMios_Icu_ValueType IcuTimeStampBuffer[7];

/* GD3000 Variables */
gd3000Status_t    gd3000Status;   /* GD3000 status variables */
tpp_drv_config_t  tppDrvConfig;   /* GD3000 configuration structure */

#define APP_STOP_SEC_VAR_INIT_UNSPECIFIED_DTCM
#include "App_MemMap.h"

/*==================================================================================================
*                                   LOCAL FUNCTION PROTOTYPES
==================================================================================================*/
void CheckSwitchState(void);
static void LCUHALL_EnableOutput(void);
static void MCAT_Init(void);

#define APP_START_SEC_CODE_ITCM
#include "App_MemMap.h"

void Pit1Notif(void);
void eMIOS1IcuNotify(void);

#define APP_STOP_SEC_CODE_ITCM
#include "App_MemMap.h"

/*==================================================================================================
*                                   TSA tables for FreeMASTER
==================================================================================================*/
#include "config/BLDC_appfreemaster_TSA.h"

/*==================================================================================================
*                                    FUNCTIONS DEFINITION
==================================================================================================*/

/*FUNCTION**********************************************************************
 *
 * Function Name : main
 * Description   : It initializes peripherals,GD3000 predriver and Feemaster communication.
 *                 On the background it maintains freemaster and GD3000 communication and
 *                 measures micro temperature.
 *
 *END**************************************************************************/
int main(void)
{
    status_t GDStatus;
    StatusType status;

    /***********************************************************************************************
    * Clock Driver
    ***********************************************************************************************/
    Clock_Ip_Init(&Clock_Ip_aClockConfig[0]);
    /* Initialize osif. */
    OsIf_Init(NULL_PTR);
    /***********************************************************************************************
    *Configure and enable interrupts
    ***********************************************************************************************/
    IntCtrl_Ip_Init(&IntCtrlConfig_0);
    /***********************************************************************************************
    * Port Driver
    ***********************************************************************************************/
    Siul2_Port_Ip_Init(NUM_OF_CONFIGURED_PINS0, g_pin_mux_InitConfigArr0);
    /***********************************************************************************************
    * Triggermux Driver
    ***********************************************************************************************/
    Trgmux_Ip_Init(&Trgmux_Ip_xTrgmuxInitPB);
    /* Assign eMIOS1_CH1 input to TRGMUX_OUT_48 */
    IP_SIUL2->IMCR[81] = SIUL2_IMCR_SSS(6U);
    
    /***********************************************************************************************
    * UART Driver
    ***********************************************************************************************/
    Lpuart_Uart_Ip_Init(6U, &Lpuart_Uart_Ip_xHwConfigPB_6);

    /***********************************************************************************************
    * ADC Driver
    ***********************************************************************************************/
    do {
        status = (StatusType)Adc_Sar_Ip_Init(0U, &AdcHwUnit_0);
    } while (status != E_OK);

    do {
        status = (StatusType)Adc_Sar_Ip_Init(2U, &AdcHwUnit_2);
    } while (status != E_OK);

    do {
        status = (StatusType)Adc_Sar_Ip_DoCalibration(0U);
    } while (status != E_OK);

    do {
        status = (StatusType)Adc_Sar_Ip_DoCalibration(2U);
    } while (status != E_OK);

    /* TempSenseEnable */
    Adc_Sar_Ip_TempSenseEnable(0U);

    /***********************************************************************************************
    * LCU Driver
    ***********************************************************************************************/
    Lcu_Ip_Init(&Lcu_Ip_xLcuInitPB);
    LCUHALL_EnableOutput();

    /***********************************************************************************************
    * SPI Driver
    ***********************************************************************************************/
    Lpspi_Ip_Init(&Lpspi_Ip_PhyUnitConfig_SpiPhyUnit_0_Instance_1);

    /***********************************************************************************************
    * PIT Driver
    ***********************************************************************************************/
    Pit_Ip_Init(1U, &PIT_1_InitConfig_PB);
    Pit_Ip_InitChannel(1U, PIT_1_ChannelConfig_PB);
    Pit_Ip_EnableChannelInterrupt(1U, 1U);
    Pit_Ip_StartChannel(1U, 1U, 40000U);  /* PIT 1ms */

    /***********************************************************************************************
    * Initialize GD3000 through TPPSDK.
    ***********************************************************************************************/
    do {
        GDStatus = GD3000_Init();
    } while (GDStatus != kStatus_Success);
    gd3000Status.B.gd3000InitDone = true;
    /* Initialize ICU channel for GD3000 interrupt. */
    Siul2_Icu_Ip_Init(0U, &Siul2_Icu_Ip_0_Config_PB);
    /* Enable ICU edge detect for GD3000 interrupt. */
    Siul2_Icu_Ip_EnableInterrupt(0U, 7U);
    Siul2_Icu_Ip_EnableNotification(0U, 7U);

    /***********************************************************************************************
    * FreeMASTER
    ***********************************************************************************************/
    /* Set FreeMASTER serial base address. */
	FMSTR_SerialSetBaseAddress((FMSTR_ADDR)IP_LPUART_6_BASE);
    /* Freemaster initalization      */
    FMSTR_Init();

    /***********************************************************************************************
    * Application
    ***********************************************************************************************/
    /* MCAT variables initialization */
    MCAT_Init();

    /***********************************************************************************************
    * eMios Driver
    ***********************************************************************************************/
    Emios_Pwm_Ip_InitChannel(0U, &Emios_Pwm_Ip_I0_Ch1);
    Emios_Pwm_Ip_InitChannel(0U, &Emios_Pwm_Ip_I0_Ch3);
    Emios_Icu_Ip_Init(1U, &eMios_Icu_Ip_1_Config_PB);
    Emios_Icu_Ip_StartTimestamp(1U, 1U, &IcuTimeStampBuffer[0], 7U, 7U);
    Emios_Icu_Ip_EnableNotification(1U, 1U);

    /***********************************************************************************************
    * BCTU Driver
    ***********************************************************************************************/
    Bctu_Ip_Init(0U, &BctuHwUnit_0);

    /* Enable eMIOS clock at last to ensure the correct trigger order */
    Emios_Mcl_Ip_Init(0U, &Emios_Mcl_Ip_0_Config_BOARD_INITPERIPHERALS);
    Emios_Mcl_Ip_Init(1U, &Emios_Mcl_Ip_1_Config_BOARD_INITPERIPHERALS);

    while (1)
    {
        FMSTR_Poll();
        AppStateMachine[appState]();
        AppStateLed[appState]();
        /* Check power stage faults */
        CheckFaults();

        /* Read GD3000 Status register 0 and Status register 1, if there is GD3000 interrupt */
        if (gd3000Status.B.gd3000IntFlag)
        {
            gd3000Status.B.gd3000IntFlag = false;
            TPP_GetStatusRegister(&tppDrvConfig, tppSR0_deviceEvents, &(tppDrvConfig.deviceConfig.statusRegister[tppSR0_deviceEvents]));
        }

        if (gd3000Status.B.gd3000ClearErr)
        {
            tppDrvConfig.deviceConfig.statusRegister[0U] = 0U;
            gd3000Status.B.gd3000ClearErr = false;
            faultStatus.B.PreDriverError = 0;
            TPP_ClearInterrupts(&tppDrvConfig, TPP_CLINT0_MASK, TPP_CLINT1_MASK);
        }

        /* MCU chip temperature detection. */
        TemperatureGetStatus = Adc_Sar_Ip_TempSenseGetTemp(2U, 0U, &TemperatureRaw);
        if (ADC_SAR_IP_STATUS_SUCCESS == TemperatureGetStatus)
        {
            /* Transform to float number. */
            MeasTempFlt = ((TemperatureRaw&0x7FF0)>>4) + (TemperatureRaw&0x0F)/16.0F;
            if ((TemperatureRaw & 0x8000) != 0)
            {
                MeasTempFlt = -(MeasTempFlt);
            }
        }
    }

    return exit_code;
}

/*FUNCTION**********************************************************************
 *
 * Function Name : GD3000_INT_Handler
 * Description   : INT pin IRQ handler.
 *
 *END**************************************************************************/
void GD3000_INT_Handler(void)
{
    /* Set GD3000 INT flag. */
    gd3000Status.B.gd3000IntFlag = true;
}

/*******************************************************************************
*
* Function:     TPP_InitializeOutputs(void)
*
* Description:  This function initialize output of the MC34GD3000 MOSFET Pre-driver
*
*******************************************************************************/
void TPP_InitializeOutputs (void)
{
    /* ********************************************************************
     *
     *The function is called by TPP_Init during the initialization process and
     *it is responsible for proper initialization of pins selected for GD3000.
     *Please see MC34GD3000 Product data sheet (Rev. 6.0) for more details about
     *GD3000 initialization procedure. In this example all necessary initialization
     *(SIUL and LCU peripheral) is covered by Lcu_Ip_Init and Siul2_Port_Ip_Initfunctions
     * so no additional configuration is needed in this place.
     *
     * *********************************************************************/
}

/*FUNCTION**********************************************************************
 *
 * Function Name : BctuFifoNotif
 * Description   : BCTU FIFO IRQ handler.
 *
 *END**************************************************************************/
void BctuFifoNotif(void)
{
    static tBool getFcnStatus;

    Siul2_Dio_Ip_SetPins(TST_GPIO_C24_PORT, (1 << TST_GPIO_C24_PIN));
    mCount = 0;
    while (Bctu_Ip_GetFifoCount(0U, 0U))
    {
        Bctu_Ip_GetFifoResult(0U, 0U, &measuredValues[mCount++]);
    }

    getFcnStatus    =    true;

    /* DCB voltage, DCB current and phase currents measurement */
    getFcnStatus  = MEAS_GetUdcVoltage(&ADCResults.DCBVVoltage);
    getFcnStatus &= MEAS_GetIdcCurrent(&ADCResults.DCBIVoltageRaw);
    ADCResults.DCBIVoltage = MLIB_Sub(ADCResults.DCBIVoltageRaw, ADCResults.DCBIOffset);
    u_dc_bus_filt = GDFLIB_FilterMA(ADCResults.DCBVVoltage, &Udcb_filt);
    if (duty_cycle > DC_THRESHOLD)
    {
        torque_filt = GDFLIB_FilterMA(ADCResults.DCBIVoltage, &Idcb_filt);
    }
    else
    {
        /* Ignore DC bus current measurement at low duty cycles */
        torque_filt = GDFLIB_FilterMA(0, &Idcb_filt);
    }
    Lcu_Ip_GetSyncLogicInput(InputSector, 3);
    SensorHall.InA = InputSector[0].Value;
    SensorHall.InB = InputSector[1].Value;
    SensorHall.InC = InputSector[2].Value;
    SensorHall.Sector = (SensorHall.InC << 2) + (SensorHall.InB << 1) + SensorHall.InA - 1;
    
    /* Timer for Rotor alignment */
    if(driveStatus.B.Alignment)
    {
        if(alignmentTimer > 0)
        {
            alignmentTimer--;
        }
        driveStatus.B.AdcSaved = 0;
    }

    /* Calibration timer for DC bus current offset measurement */
    if(driveStatus.B.Calib)
    {
        calibTimer--;
    }

    Siul2_Dio_Ip_ClearPins(TST_GPIO_C24_PORT, (1 << TST_GPIO_C24_PIN));
    FMSTR_Recorder(0);
}

/*FUNCTION**********************************************************************
 *
 * Function Name : Pit1Notif
 * Description   : PIT1 IRQ handler.
 *
 *END**************************************************************************/
void Pit1Notif(void)
{
	Siul2_Dio_Ip_SetPins(TST_GPIO_B20_PORT, (1 << TST_GPIO_B20_PIN));
    if (appState == APP_RUN)
    {
        uint8_t i = 1;
        /* here 1 is added to prevent division by 0 during  actualSpeed calculation */
        period6ZC = 1 + SensorHall.Period[0];
        for (i=1; i<6; i++)
        {
            period6ZC += SensorHall.Period[i];
        }
        actualSpeed = 30.0F*625.0F*1000.0F/(tFloat)period6ZC;
    }
    else
    {
        actualSpeed = 0;
    }

    speed_fb = actualSpeed;

    if (driveStatus.B.CloseLoop == 1)
    {
        torqueErr = MLIB_Sub(I_DCB_LIMIT, torque_filt);
        currentPIOut = GFLIB_ControllerPIpAW(torqueErr, &currentPIPrms);

        /* Speed control */

        /* Upper speed limit due to the limited DC bus voltage 12V */
        if (requiredSpeed >= N_NOM)
        {
            requiredSpeed = N_NOM;
        }

        /* Lower speed limit keeping reliable sensorless operation */
        if (requiredSpeed < mcat_NMin)
        {
            requiredSpeed = mcat_NMin;
        }

        requiredSpeedRamp = GFLIB_Ramp(requiredSpeed, &speedRampPrms);
        speedErr = MLIB_Sub(requiredSpeedRamp, speed_fb);
        speedPIOut = GFLIB_ControllerPIpAW(speedErr, &speedPIPrms);

        if (currentPIOut >= speedPIOut)
        {
            /* If max torque not achieved, use speed PI output */
            currentPIPrms.fltIntegPartK_1 = speedPIOut;
            currentPIPrms.fltInK_1 = 0;
            /* PWM duty cycle update <- speed PI */
            duty_cycle = speedPIOut;

            driveStatus.B.CurrentLimiting = 0;
        }
        else
        {
            /* Limit speed PI output by current PI if max. torque achieved */
            speedPIPrms.fltIntegPartK_1 = currentPIOut;
            speedPIPrms.fltInK_1 = 0;
            /* PWM duty cycle update <- current PI */
            duty_cycle = currentPIOut;
            driveStatus.B.CurrentLimiting = 1;
        }
    }

    if (duty_cycle < 1.0F) duty_cycle = 1.0F;
    if (duty_cycle > 90.0F) duty_cycle = 90.0F;

    ACTUATE_SetDutycycle(duty_cycle/100.0F);
    Siul2_Dio_Ip_ClearPins(TST_GPIO_B20_PORT, (1 << TST_GPIO_B20_PIN));
    CheckSwitchState();
}

/*FUNCTION**********************************************************************
 *
 * Function Name : eMIOS1IcuNotify
 * Description   : eMIOS1 ICU IRQ handler.
 *
 *END**************************************************************************/
void eMIOS1IcuNotify(void)
{
    uint8_t i;

    //Siul2_Dio_Ip_SetPins(TST_GPIO_C24_PORT, (1 << TST_GPIO_C24_PIN));

    for (i = 0; i < 6; i++)
    {
        if (IcuTimeStampBuffer[i+1] > IcuTimeStampBuffer[i])
        {
            SensorHall.Period[i] = IcuTimeStampBuffer[i+1] - IcuTimeStampBuffer[i];
        }
        else
        {
            SensorHall.Period[i] = 0xFFFF - IcuTimeStampBuffer[i] + IcuTimeStampBuffer[i+1];
        }
    }

    //Siul2_Dio_Ip_ClearPins(TST_GPIO_C24_PORT, (1 << TST_GPIO_C24_PIN));
}

/*FUNCTION**********************************************************************
 *
 * Function Name : LCUHALL_EnableOutput
 * Description   : LCU outputs enable.
 *
 *END**************************************************************************/
void LCUHALL_EnableOutput(void)
{
    Lcu_Ip_SyncInputValueType SWOverride[1U];
    Lcu_Ip_SyncOutputValueType lcuEnable[4U];

    SWOverride[0].LogicInputId = LCU_LOGIC_INPUT_11;
    SWOverride[0].Value = 1;
    Lcu_Ip_SetSyncInputSwOverrideEnable(SWOverride, 1U);

    lcuEnable[0].LogicOutputId = LCU_LOGIC_OUTPUT_6;
    lcuEnable[0].Value = 1U;
    lcuEnable[1].LogicOutputId = LCU_LOGIC_OUTPUT_7;
    lcuEnable[1].Value = 1U;
    lcuEnable[2].LogicOutputId = LCU_LOGIC_OUTPUT_8;
    lcuEnable[2].Value = 1U;
    lcuEnable[3].LogicOutputId = LCU_LOGIC_OUTPUT_9;
    lcuEnable[3].Value = 1U;
    Lcu_Ip_SetSyncOutputEnable(lcuEnable, 4U);
}

/*FUNCTION**********************************************************************
 *
 * Function Name : MCAT_Init
 * Description   : Load MCAT parameters.
 *
 *END**************************************************************************/
static void MCAT_Init(void)
{
    ADCResults.DCBIOffset  = I_MAX;
    ADCResults.DCBVVoltage = 12.0F;

    fm_voltage = FM_U_DCB_SCALE;
    fm_current = FM_I_SCALE;
    fm_speed   = FM_N_SCALE;

    /* Initialize DC bus voltage moving average filter  */
    Udcb_filt.fltLambda = DCBV_FILTER_MA_LAMBDA;
    GDFLIB_FilterMAInit_FLT(&Udcb_filt);

    speed_filt.fltLambda = 0.005F;
    GDFLIB_FilterMAInit_FLT(&speed_filt);

    /* Initialize DC bus current moving average filter */
    Idcb_filt.fltLambda = TORQUE_LOOP_MAF;
    GDFLIB_FilterMAInit_FLT(&Idcb_filt);

    /* Initialize moving average filter for DC bus current offset calibration */
    Idcb_calib.fltLambda = CALIB_FILTER_MA_LAMBDA;
    GDFLIB_FilterMAInit_FLT(&Idcb_calib);

    /* Speed PI controller initialization */
    speedPIPrms.fltPropGain   = SPEED_LOOP_KP_GAIN;
    speedPIPrms.fltIntegGain  = SPEED_LOOP_KI_GAIN;
    speedPIPrms.fltUpperLimit = CTRL_LOOP_LIM_HIGH;
    speedPIPrms.fltLowerLimit = CTRL_LOOP_LIM_LOW;

    /* Current PI controller initialization */
    currentPIPrms.fltPropGain   = TORQUE_LOOP_KP_GAIN;
    currentPIPrms.fltIntegGain  = TORQUE_LOOP_KI_GAIN;
    currentPIPrms.fltUpperLimit = CTRL_LOOP_LIM_HIGH;;
    currentPIPrms.fltLowerLimit = CTRL_LOOP_LIM_LOW;

    /* SPeed ramp initialization */
    speedRampPrms.fltRampUp   = SPEED_LOOP_RAMP_UP;
    speedRampPrms.fltRampDown = SPEED_LOOP_RAMP_DOWN;

    /* Hall initialization */
    SensorHall.Sector     = 0;

    /* MCAT Parameters tab */
    mcat_alignVoltage     = ALIGN_VOLTAGE;
    mcat_alignDuration    = ALIGN_DURATION;

    /* MCAT Sensorless tab */
    mcat_NMin             = N_MIN;
    mcat_FreewheelTLong   = FREEWHEEL_T_LONG;
    mcat_FreewheelTShort  = FREEWHEEL_T_SHORT;
    mcat_startCmtCnt      = STARTUP_CMT_CNT;
    mcat_startCmtPer      = STARTUP_CMT_PER;
    mcat_startCmtAcceler  = START_CMT_ACCELER;
    mcat_cmtTOff          = CMT_T_OFF;
    mcat_integThr         = INTEG_TRH;
}

/*FUNCTION**********************************************************************
 *
 * Function Name : AppInit
 * Description   : BLDC application INIT state function.
 *
 *END**************************************************************************/
void AppInit(void)
{
    driveStatus.B.Alignment  = 0;
    driveStatus.B.EnableCMT  = 0;
    driveStatus.B.CloseLoop  = 0;
    driveStatus.B.Calib      = 0;
    driveStatus.B.Sensorless = 0;
    driveStatus.B.NewZC      = 0;

    /* Init parameters for DC bus current offset calibration */
    calibTimer               = CALIB_TIMER;
    ADCResults.DCBIOffset    = I_MAX;
    Idcb_calib.fltAcc        = I_MAX;

    /* Init parameters for Speed control */
    actualSpeed              = 0.0F;
    duty_cycle               = 0.01F;

    /* Disable all PWMs */
    ACTUATE_DisableOutput();

    appState                 = APP_STOP;
}

/*FUNCTION**********************************************************************
 *
 * Function Name : AppStop
 * Description   : BLDC application STOP state function.
 *
 *END**************************************************************************/
void AppStop(void)
{
    //driveStatus.B.StallCheckReq = 1;
    /* Application can be turn on only if rotor stops */
    if (appSwitchState == 1)
    {
        /* Enable actuator */
        duty_cycle= 0.01F;
        ACTUATE_EnableOutput();
        driveStatus.B.Calib = 1;
        appState = APP_CALIB;
    }
}

/*FUNCTION**********************************************************************
 *
 * Function Name : AppCalib
 * Description   : BLDC application CALIB state function.
 *
 *END**************************************************************************/
void AppCalib(void)
{
    /* Measure DC bus current offset */
    ADCResults.DCBIOffset = GDFLIB_FilterMA(ADCResults.DCBIVoltageRaw, &Idcb_calib);

    if (calibTimer == 0)
    {
        AppStopToAlignment();
    }
}

/*FUNCTION**********************************************************************
 *
 * Function Name : AppStopToAlignment
 * Description   : BLDC application STOP to ALIGN state transition function.
 *
 *END**************************************************************************/
void AppStopToAlignment(void)
{
    driveStatus.B.Alignment = 1;
    driveStatus.B.EnableCMT = 0;
    driveStatus.B.CloseLoop = 0;
    driveStatus.B.Calib = 0;
    driveStatus.B.Sensorless = 0;
    driveStatus.B.NewZC = 0;

    duty_cycle = MLIB_Mul(MLIB_Div(mcat_alignVoltage, U_PH_NOM), 100.0F);
    alignmentTimer = mcat_alignDuration;

    Alignment[0].Value = 1;
    Alignment[1].Value = 1;
    Alignment[2].Value = 1;
    Alignment[3].Value = 1;
    Alignment[4].Value = 1;
    Alignment[5].Value = 1;
    Lcu_Ip_SetSyncInputSwOverrideEnable(Alignment, 6U);
    Lcu_Ip_SetSyncInputSwOverrideValue(Alignment, 6U);

    appState = APP_ALIGNMENT;
}

/*FUNCTION**********************************************************************
 *
 * Function Name : AppAlignment
 * Description   : BLDC application ALIGN state function.
 *
 *END**************************************************************************/
void AppAlignment(void)
{
    if (alignmentTimer == 0)
    {
        AppAlignmentToStart();
    }
}

/*FUNCTION**********************************************************************
 *
 * Function Name : AppAlignmentToStart
 * Description   : BLDC application ALIGN to START state transition function.
 *
 *END**************************************************************************/
void AppAlignmentToStart(void)
{
    driveStatus.B.Alignment = 0;
    driveStatus.B.EnableCMT = 1;
    driveStatus.B.AfterCMT  = 0;

    Alignment[0].Value = 0;
    Alignment[1].Value = 0;
    Alignment[2].Value = 0;
    Alignment[3].Value = 0;
    Alignment[4].Value = 0;
    Alignment[5].Value = 0;
    Lcu_Ip_SetSyncInputSwOverrideValue(Alignment, 6U);
    Lcu_Ip_SetSyncInputSwOverrideEnable(Alignment, 6U);

    /* Open loop startup is ignored in Sensorbased mode */
    if (rotationDir == ROTATION_DIR_CW)
        Lcu_Ip_SetSyncInputSwOverrideValue(&InputCW, 1U);
    else
        Lcu_Ip_SetSyncInputSwOverrideValue(&InputCCW, 1U);

    appState = APP_START;
}

/*FUNCTION**********************************************************************
 *
 * Function Name : AppStart
 * Description   : BLDC application START state function.
 *
 *END**************************************************************************/
void AppStart(void)
{
    AppStartToRun();
}

/*FUNCTION**********************************************************************
 *
 * Function Name : AppStartToRun
 * Description   : BLDC application START to RUN state transition function.
 *
 *END**************************************************************************/
void AppStartToRun(void)
{
    /* Speed PI controller initialization */
    speedPIPrms.fltInK_1 = 0;
    speedPIPrms.fltIntegPartK_1 = duty_cycle;

    /* Current PI controller initialization */
    currentPIPrms.fltInK_1 = 0;
    currentPIPrms.fltIntegPartK_1 = speedPIPrms.fltIntegPartK_1;

    /* Speed ramp initialization */
    speedRampPrms.fltState = mcat_NMin;

    appState = APP_RUN;
    faultStatus.B.StallError = 0;

    driveStatus.B.CloseLoop = 1;
}

/*FUNCTION**********************************************************************
 *
 * Function Name : AppRun
 * Description   : BLDC application RUN state function.
 *
 *END**************************************************************************/
void AppRun(void)
{
    if (appSwitchState == 0)
    {
        /* Disable actuator */
        ACTUATE_DisableOutput();

        mcat_FreewheelTShort = 0;
        mcat_integThr = 0;
        driveStatus.B.Freewheeling = 1;

        appState = APP_INIT;
    }
}

/*FUNCTION**********************************************************************
 *
 * Function Name : AppFault
 * Description   : BLDC application FAULT state function.
 *
 *END**************************************************************************/
void AppFault(void)
{
    if (faultSwitchClear == 1)
    {
        driveStatus.B.Fault = 0;
        faultStatus.R = 0;
        faultStatusLatched.R = 0;
        faultSwitchClear = 0;
        appState = APP_INIT;

        gd3000Status.B.gd3000ClearErr = true;     /* Clear GD3000 faults */
    }
}

/*FUNCTION**********************************************************************
 *
 * Function Name : CheckFaults
 * Description   : BLDC application fault detection function.
 *
 *END**************************************************************************/
void CheckFaults(void)
{
    /* DC bus current overcurrent */
    if (ADCResults.DCBIVoltage > I_DCB_OVERCURRENT)
    {
        driveStatus.B.Alignment = 0;
        driveStatus.B.EnableCMT = 0;
        driveStatus.B.CloseLoop = 0;
        driveStatus.B.Sensorless = 0;
        driveStatus.B.NewZC = 0;

        faultStatus.B.OverDCBusCurrent = 1;

        /* Disable actuator */
        ACTUATE_DisableOutput();
    }
    else
    {
        faultStatus.B.OverDCBusCurrent = 0;
    }


    /* DC bus voltage overvoltage */
    if (ADCResults.DCBVVoltage > U_DCB_OVERVOLTAGE)
    {
        faultStatus.B.OverDCBusVoltage = 1;

        driveStatus.B.Alignment = 0;
        driveStatus.B.EnableCMT = 0;
        driveStatus.B.CloseLoop = 0;
        driveStatus.B.Sensorless = 0;
        driveStatus.B.NewZC = 0;

        /* Disable actuator */
        ACTUATE_DisableOutput();
    }
    else
    {
        faultStatus.B.OverDCBusVoltage = 0;
    }

    /* DC bus voltage undervoltage */
    if(ADCResults.DCBVVoltage < U_DCB_UNDERVOLTAGE)
    {
        faultStatus.B.UnderDCBusVoltage = 1;

        driveStatus.B.Alignment = 0;
        driveStatus.B.EnableCMT = 0;
        driveStatus.B.CloseLoop = 0;
        driveStatus.B.Sensorless = 0;
        driveStatus.B.NewZC = 0;

        /* Disable actuator */
        ACTUATE_DisableOutput();
    }
    else
    {
        faultStatus.B.UnderDCBusVoltage = 0;
    }

    /* Check the status of the GD3000 MOSFET pre-driver */
    if (tppDrvConfig.deviceConfig.statusRegister[0U])
    {
        faultStatus.B.PreDriverError = 1;

        driveStatus.B.Alignment = 0;
        driveStatus.B.EnableCMT = 0;
        driveStatus.B.CloseLoop = 0;
        driveStatus.B.Sensorless = 0;
        driveStatus.B.NewZC = 0;

        /* Disable actuator */
        ACTUATE_DisableOutput();
    }
    else
    {
        faultStatus.B.PreDriverError = 0;
    }

    faultStatusLatched.R |= faultStatus.R;

    if (faultStatusLatched.R != 0)
    {
        driveStatus.B.Fault = 1;
        appSwitchState = 0;
        appState = APP_FAULT;
    }
    else
    {
        faultSwitchClear = 0;
    }
}

/*FUNCTION**********************************************************************
 *
 * Function Name : CheckSwitchState
 * Description   : User switch state detection function.
 *
 *END**************************************************************************/
void CheckSwitchState(void)
{
    if(switchOffCounter == 0)
    {
        /* Speed up or start the motor */
        if((Siul2_Dio_Ip_ReadPin(BTN_INC_SW5_PORT, BTN_INC_SW5_PIN) & 1))
        {
            switchCounter[0]++;

            if(switchCounter[0] > SW_PRESS_DEBOUNCE)
            {
                if(appSwitchState == 0)
                {
                	rotationDir = ROTATION_DIR_CW;
                    appSwitchState = 1;
                    switchOffCounter = SW_PRESS_OFF;
                }
                else
                {
                    requiredSpeed += SPEED_INC;
                }

                switchCounter[0] = 0;
            }
        }

        /* Speed down or start the motor */
        if((Siul2_Dio_Ip_ReadPin(BTN_DEC_SW6_PORT, BTN_DEC_SW6_PIN) & 1))
        {
            switchCounter[1]++;

            if(switchCounter[1] > SW_PRESS_DEBOUNCE)
            {
            	if(appSwitchState == 0)
            	{
            		rotationDir = ROTATION_DIR_CCW;
            	    appSwitchState = 1;
   	                switchOffCounter = SW_PRESS_OFF;
            	}
            	else
                {
            	    requiredSpeed -= SPEED_DEC;
                }

            	switchCounter[1] = 0;
            }
        }

        /* Clear faults or stop the motor */
        if((Siul2_Dio_Ip_ReadPin(BTN_INC_SW5_PORT, BTN_INC_SW5_PIN) & 1) && (Siul2_Dio_Ip_ReadPin(BTN_DEC_SW6_PORT, BTN_DEC_SW6_PIN) & 1))
        {
            if(appState == APP_FAULT)
            {
                faultSwitchClear = 1;
            }

            appSwitchState = 0;
            switchOffCounter = SW_PRESS_OFF;
        }
    }
    else
    {
        switchOffCounter--;
    }
}

/*FUNCTION**********************************************************************
 *
 * Function Name : StateRGBLedOFF
 * Description   : RGB LED OFF state.
 *
 *END**************************************************************************/
void StateRGBLedOFF(void)
{
    Siul2_Dio_Ip_ClearPins(LED_BLUE_PORT, (1 << LED_BLUE_PIN));     /* led RGB Blue  Led OFF */
    Siul2_Dio_Ip_ClearPins(LED_RED_PORT, (1 << LED_RED_PIN));       /* led RGB Red   Led OFF */
    Siul2_Dio_Ip_ClearPins(LED_GREEN_PORT, (1 << LED_GREEN_PIN));   /* led RGB Green Led OFF */
}

/*FUNCTION**********************************************************************
 *
 * Function Name : StateRGBLedBlueON
 * Description   : Blue RGB LED ON state.
 *
 *END**************************************************************************/
void StateRGBLedBlueON(void)
{
    Siul2_Dio_Ip_SetPins(LED_BLUE_PORT, (1 << LED_BLUE_PIN));       /* led RGB Blue  Led ON  */
    Siul2_Dio_Ip_ClearPins(LED_RED_PORT, (1 << LED_RED_PIN));       /* led RGB Red   Led OFF */
    Siul2_Dio_Ip_ClearPins(LED_GREEN_PORT, (1 << LED_GREEN_PIN));   /* led RGB Green Led OFF */
}

/*FUNCTION**********************************************************************
 *
 * Function Name : StateRGBLedRedON
 * Description   : Red RGB LED ON state.
 *
 *END**************************************************************************/
void StateRGBLedRedON(void)
{
    Siul2_Dio_Ip_ClearPins(LED_BLUE_PORT, (1 << LED_BLUE_PIN));     /* led RGB Blue  Led OFF */
    Siul2_Dio_Ip_SetPins(LED_RED_PORT, (1 << LED_RED_PIN));         /* led RGB Red   Led ON  */
    Siul2_Dio_Ip_ClearPins(LED_GREEN_PORT, (1 << LED_GREEN_PIN));   /* led RGB Green Led OFF */
}

/*FUNCTION**********************************************************************
 *
 * Function Name : StateRGBLedGreenON
 * Description   : Green RGB LED ON state.
 *
 *END**************************************************************************/
void StateRGBLedGreenON(void)
{
    Siul2_Dio_Ip_ClearPins(LED_BLUE_PORT, (1 << LED_BLUE_PIN));   /* led RGB Blue  Led OFF */
    Siul2_Dio_Ip_ClearPins(LED_RED_PORT, (1 << LED_RED_PIN));     /* led RGB Red   Led OFF */
    Siul2_Dio_Ip_SetPins(LED_GREEN_PORT, (1 << LED_GREEN_PIN));   /* led RGB Green Led ON  */
}

/*FUNCTION**********************************************************************
 *
 * Function Name : StateRGBLedGreenFlashing
 * Description   : Green RGB LED FLASHING state.
 *
 *END**************************************************************************/
void StateRGBLedGreenFlashing(void)
{
    ledCounter += 1;

    /* RGB Green Led FLASHING */
    if (ledCounter > LED_FLASH_FREQ)
    {
        Siul2_Dio_Ip_TogglePins(LED_GREEN_PORT, (1 << LED_GREEN_PIN));
        ledCounter = 0;
    }
    Siul2_Dio_Ip_ClearPins(LED_BLUE_PORT, (1 << LED_BLUE_PIN));  /* led RGB Blue  Led OFF */
    Siul2_Dio_Ip_ClearPins(LED_RED_PORT, (1 << LED_RED_PIN));    /* led RGB Red   Led OFF */
}
