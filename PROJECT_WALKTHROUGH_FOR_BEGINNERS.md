# MCSPTE1AK344 BLDC 6‑Step (Hall) — Project Walkthrough (Beginner)

This document explains **how the whole project works**, step-by-step, for someone who is new to the MCSPTE1AK344 kit and this codebase.

**Project folder:** `MCSPTE1AK344_BLDC_6Step_hall_ll`  
**Main file:** `src/main.c`  
**Kit:** S32K344 EVB + DEVKIT‑MOTORGD power stage (GD3000 predriver)

---

## 0. What this project does (1 paragraph)

This firmware runs a **3‑phase BLDC motor** using **six-step commutation** (trapezoidal control) with **Hall sensors** for rotor position. The MCU reads DC bus voltage/current and Hall timing, computes a duty cycle (throttle) using PI controllers, and outputs PWM/commutation signals to the power stage. A state machine controls the high-level behavior (Init → Stop → Calib → Alignment → Run → Fault). Fault logic stops the motor and turns the RGB LED red.

---

## 1. Hardware blocks (what connects to what)

### 1.1 Main hardware components

- **S32K344 MCU (EVB board)**: runs the control algorithm, reads sensors, generates PWM.
- **DEVKIT‑MOTORGD power stage**: contains the **GD3000** gate driver and MOSFET half-bridges.
- **BLDC motor with Hall sensors**: provides 3 digital Hall signals (A/B/C).
- **Supply**: typically 12 V to the motor power stage (DC bus).

### 1.2 The most important MCU peripherals used

- **LPSPI1**: SPI link to GD3000 (predriver configuration + status).
- **ADC + BCTU**: measure DC bus voltage/current and phase currents (depending on configuration).
- **LCU + eMIOS PWM**: generate commutation/PWM signals and timing.
- **eMIOS ICU**: timestamps Hall events (for speed estimation).
- **PIT**: periodic control loop interrupt (speed/current control).
- **LPUART + FreeMASTER**: monitoring/tuning via PC tool.

---

## 2. Where to start reading the code

If you only read three files first:

1. **`src/main.c`** — everything is wired together here (init + interrupts + main loop + state machine + faults).
2. **`src/state_machine.c` / `src/state_machine.h`** — names and meaning of each application state.
3. **`src/config/BLDC_appconfig.h`** — key thresholds and tuning constants (voltage/current limits, PI gains, alignment time).

---

## 3. Program flow overview (big picture)

At a high level, the firmware runs in three “layers” at runtime:

1. **Interrupt layer (time-critical)**  
   - ADC / BCTU FIFO ISR: refreshes measurements frequently  
   - PIT ISR: runs the main control loop regularly  
   - eMIOS ICU ISR: timestamps Hall events → speed feedback  
   - GD3000 INT ISR: marks that GD3000 has an interrupt to read/clear

2. **Background loop layer (main `while(1)`)**  
   - FreeMASTER servicing  
   - Calls the application state machine function for the current state  
   - Updates LEDs  
   - Runs fault checks  
   - Services GD3000 status if needed

3. **State machine layer (high-level motor behavior)**  
   - Handles “what should happen now” (e.g., calibrate offset, align rotor, run motor, stop, fault).

---

## 4. Step-by-step startup sequence (`main()`)

This is the order (conceptually) of what `main()` does in `src/main.c`:

### Step 4.1 Clock + OS abstraction

- Initializes the clock tree (`Clock_Ip_Init`)
- Initializes OSIF (`OsIf_Init`) used by some drivers

### Step 4.2 Interrupt controller

- Enables configured interrupts (`IntCtrl_Ip_Init`)

### Step 4.3 Pin muxing

- Configures all pins using the generated pin array:  
  `Siul2_Port_Ip_Init(NUM_OF_CONFIGURED_PINS0, g_pin_mux_InitConfigArr0);`

### Step 4.4 TRGMUX, UART, ADC init + calibration

- TRGMUX routing is initialized (`Trgmux_Ip_Init`)
- UART is initialized for FreeMASTER (LPUART6)
- ADC instances are initialized and calibrated (the code loops until `E_OK`)
- Temperature sensor is enabled

### Step 4.5 LCU + PWM routing enable

- LCU is initialized
- Outputs are enabled so commutation/PWM control can work

### Step 4.6 SPI + GD3000 initialization

- LPSPI is initialized
- GD3000 init is attempted (TPPSDK). The code tracks whether init is done (`gd3000InitDone`) and may retry later in the background loop.

### Step 4.7 FreeMASTER + application init

- FreeMASTER base address is set and FreeMASTER is initialized (`FMSTR_Init`)
- MCAT parameters and software structures are initialized (`MCAT_Init`)

### Step 4.8 eMIOS + BCTU init

- PWM and ICU are initialized
- Timestamp capture is started (for Hall period measurement)
- BCTU is initialized for measurement acquisition

### Step 4.9 Enter main loop

`while (1)` runs forever and orchestrates everything else.

---

## 5. The main loop (`while(1)`) — what happens repeatedly

Inside `while(1)` in `src/main.c`, this is the repeating sequence:

1. **`FMSTR_Poll()`**  
   Services FreeMASTER communications so you can view/tune variables.

2. **GD3000 init retry (only if not initialized)**  
   If `gd3000InitDone == false`, the code periodically retries `GD3000_Init()`.

3. **State machine**  
   `AppStateMachine[appState]();` runs the function for the current state.

4. **LED update**  
   `AppStateLed[appState]();` sets the RGB LED according to the state.

5. **Fault checks**  
   `CheckFaults();` updates fault bits and may force `APP_FAULT`.

6. **GD3000 interrupt service (if INT happened)**  
   If the GD3000 INT flag is set, read status registers through SPI.

7. **GD3000 clear-error service**  
   If software requested clear (e.g., user cleared faults), send clear commands.

8. **Temperature read (optional monitoring)**  
   Reads MCU die temperature and stores it.

---

## 6. Application state machine (what the user experiences)

States are defined in `src/state_machine.h` and wired in `src/state_machine.c`:

- `APP_INIT` (0): reset software flags, disable PWM, go to STOP
- `APP_STOP` (5): wait for the user to start
- `APP_CALIB` (1): measure DC bus current offset for better current/torque estimate
- `APP_ALIGNMENT` (2): apply a fixed commutation pattern to align rotor
- `APP_START` (3): transition into RUN (in this Hall project, START is quick)
- `APP_RUN` (4): closed-loop running; user can change speed; if user stops, go back to INIT
- `APP_FAULT` (6): fault latched; motor outputs disabled; requires user clear action

### 6.1 User buttons (start / speed up / speed down / clear)

Handled in `CheckSwitchState()` in `src/main.c`:

- **SW5 (INC)**: start or increase required speed
- **SW6 (DEC)**: start or decrease required speed / change direction on first press
- **SW5 + SW6 together**: clear faults if in `APP_FAULT`

There is debounce and lockout timing (`SW_PRESS_DEBOUNCE`, `SW_PRESS_OFF`) to prevent accidental multiple triggers.

---

## 7. Interrupts (the real-time “engine”)

### 7.1 BCTU FIFO notification ISR (`BctuFifoNotif`)

Purpose: refresh measurements.

What it does (high level):

- Reads BCTU FIFO results into a buffer
- Updates DC bus voltage (`ADCResults.DCBVVoltage`)
- Updates DC bus current raw and filtered (`DCBIVoltageRaw`, `torque_filt`)
- Reads Hall sector inputs and updates `SensorHall.Sector`
- Decrements timers for calibration and alignment phases
- Records FreeMASTER data (`FMSTR_Recorder(0)`)

### 7.2 PIT ISR (`Pit1Notif`) — main control loop

Purpose: run speed/current control at a fixed periodic rate.

Key steps:

1. Compute **speed feedback** from summed Hall periods (`SensorHall.Period[]`).
2. If `driveStatus.B.CloseLoop == 1`, run:
   - current limiting PI (torque PI) based on DC bus current
   - speed PI based on ramped required speed
   - choose the smaller of speed PI / current PI to limit current
3. Clamp duty cycle and write it to PWM (`ACTUATE_SetDutycycle`).
4. Call `CheckSwitchState()` to process user inputs.

### 7.3 eMIOS ICU notify (`eMIOS1IcuNotify`) — Hall timing

Purpose: compute Hall period values from ICU timestamps.

It updates `SensorHall.Period[i]` by taking differences of timestamp captures.

### 7.4 GD3000 interrupt handler (`GD3000_INT_Handler`)

Purpose: minimal ISR that sets a flag.

It sets `gd3000Status.B.gd3000IntFlag = true;` and the main loop later reads GD3000 status over SPI.

---

## 8. Fault handling (why the LED turns red)

Fault logic is in `CheckFaults()` in `src/main.c`.

Fault sources include:

- **Over DC bus current** (`I_DCB_OVERCURRENT`)
- **Over DC bus voltage** (`U_DCB_OVERVOLTAGE`)
- **Under DC bus voltage** (`U_DCB_UNDERVOLTAGE`)
- **GD3000 predriver fault** (GD3000 status register 0 indicates events)

### 8.1 Latching behavior

- `faultStatus` contains current cycle faults
- `faultStatusLatched` ORs in faults and keeps them set until cleared
- If `faultStatusLatched != 0`, the firmware forces:
  - `appState = APP_FAULT`
  - PWM outputs disabled (`ACTUATE_DisableOutput()`)
  - RGB LED red (because `APP_FAULT` maps to red in `AppStateLed`)

### 8.2 Clearing faults

User action: press **SW5 + SW6 together** while in `APP_FAULT`.

This sets `faultSwitchClear`, and `AppFault()` resets:

- `faultStatus` and `faultStatusLatched`
- `driveStatus.B.Fault`
- state back to `APP_INIT`
- and requests GD3000 clear interrupts (`gd3000ClearErr = true`)

---

## 9. Motor control behavior (what “six-step hall” means here)

Six-step commutation energizes two phases at a time (one high, one low) according to rotor sector.

In this project:

- Hall inputs determine the **sector**
- Commutation outputs are handled through LCU sync inputs/overrides and PWM channels
- Duty cycle controls average voltage → controls speed/torque

Rotor alignment forces a known energization pattern for a fixed duration so the initial sector is controlled.

---

## 10. Configuration values you will most often tune

File: `src/config/BLDC_appconfig.h`

Typical things to adjust:

- `U_DCB_UNDERVOLTAGE`, `U_DCB_OVERVOLTAGE`
- `I_DCB_OVERCURRENT`, `I_DCB_LIMIT`
- PI gains: `SPEED_LOOP_KP_GAIN`, `SPEED_LOOP_KI_GAIN`, `TORQUE_LOOP_KP_GAIN`, `TORQUE_LOOP_KI_GAIN`
- Alignment: `ALIGN_VOLTAGE`, `ALIGN_DURATION`
- Minimum speed: `N_MIN`

---

## 11. Quick “how to run it” checklist (for a new user)

1. Power the kit correctly (EVB + motor shield seated, DC supply connected).
2. Build and flash the `Debug_FLASH` configuration.
3. After reset:
   - Green flashing states indicate calibration/alignment/start
   - Blue/green states indicate running/stopped (depending on mapping)
   - Red LED means fault latched
4. Press **SW5** to start/increase speed (or SW6 for opposite direction/start).
5. If red LED: press **SW5 + SW6** to clear faults, then start again.
6. Use FreeMASTER/MCAT to observe `appState`, `faultStatus`, `ADCResults`, and speed variables.

---

## 12. Where to look next (deeper reading)

- Measurement scaling: `src/meas_s32k.c`
- Actuation/PWM and commutation: `src/actuate_s32k.c`
- GD3000 driver stack:
  - `TPPSDK_GD3000_Driver/gd3000_init.c`
  - `TPPSDK_GD3000_Driver/tpp/tpp.c`

