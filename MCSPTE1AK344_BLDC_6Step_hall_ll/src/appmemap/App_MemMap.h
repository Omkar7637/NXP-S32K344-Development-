/*******************************************************************************
*   Project      : MCSPTE1AK344\MCSPTE1AK344_BLDC_6Step_hall_ll
*   Revision     : 1.2
*   RTD Version  : 3.0.0
*   Brief description  :
*   Memory mapping specification.
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
/**
* @brief Symbol used for checking correctness of the includes
*/
#define MEMMAP_ERROR

/**************************************** App - Motor Control *******************************/
#ifdef APP_START_SEC_CODE_FLASH
    /**
    * @file App_MemMap.h
    */
    #undef APP_START_SEC_CODE_FLASH
    #define ENTERED_APP_START_SEC_CODE_FLASH
    #ifndef MEMMAP_MATCH_ERROR
        #define MEMMAP_MATCH_ERROR
    #else
        #ifndef APP_STOP_SEC_CODE_FLASH
        #error "MemMap.h, no valid matching start-stop section defined."
        #endif
    #endif
    /**
    * @file App_MemMap.h
    */
    #undef MEMMAP_ERROR
    #pragma GCC section text ".mcal_text" 
#endif

#ifdef APP_STOP_SEC_CODE_FLASH
    /**
    * @file App_MemMap.h
    */
    #ifdef ENTERED_APP_START_SEC_CODE_FLASH
        #undef ENTERED_APP_START_SEC_CODE_FLASH
    #else
        #error "MemMap.h, no valid matching start-stop section defined."
    #endif
    #ifdef MEMMAP_MATCH_ERROR
        #undef MEMMAP_MATCH_ERROR
    #endif
    #undef APP_STOP_SEC_CODE_FLASH
    /**
    * @file App_MemMap.h
    */
    #undef MEMMAP_ERROR
    #pragma GCC section text
#endif

#ifdef APP_START_SEC_CODE_RAM
    /**
    * @file App_MemMap.h
    */
    #undef APP_START_SEC_CODE_RAM
    #define ENTERED_APP_START_SEC_CODE_RAM
    #ifndef MEMMAP_MATCH_ERROR
        #define MEMMAP_MATCH_ERROR
    #else
        #ifndef APP_STOP_SEC_CODE_RAM
        #error "MemMap.h, no valid matching start-stop section defined."
        #endif
    #endif
    /**
    * @file App_MemMap.h
    */
    #undef MEMMAP_ERROR
    #pragma GCC section text ".ramcode"
#endif

#ifdef APP_STOP_SEC_CODE_RAM
    /**
    * @file App_MemMap.h
    */
    #ifdef ENTERED_APP_START_SEC_CODE_RAM
        #undef ENTERED_APP_START_SEC_CODE_RAM
    #else
        #error "MemMap.h, no valid matching start-stop section defined."
    #endif
    #ifdef MEMMAP_MATCH_ERROR
        #undef MEMMAP_MATCH_ERROR
    #endif
    #undef APP_STOP_SEC_CODE_RAM
    /**
    * @file App_MemMap.h
    */
    #undef MEMMAP_ERROR
    #pragma GCC section text
#endif

#ifdef APP_START_SEC_CODE_ITCM
    /**
    * @file App_MemMap.h
    */
    #undef APP_START_SEC_CODE_ITCM
    #define ENTERED_APP_START_SEC_CODE_ITCM
    #ifndef MEMMAP_MATCH_ERROR
        #define MEMMAP_MATCH_ERROR
    #else
        #ifndef APP_STOP_SEC_CODE_ITCM
        #error "MemMap.h, no valid matching start-stop section defined."
        #endif
    #endif
    /**
    * @file App_MemMap.h
    */
    #undef MEMMAP_ERROR
    #pragma GCC section text ".itcm_text"
#endif

#ifdef APP_STOP_SEC_CODE_ITCM
    /**
    * @file App_MemMap.h
    */
    #ifdef ENTERED_APP_START_SEC_CODE_ITCM
        #undef ENTERED_APP_START_SEC_CODE_ITCM
    #else
        #error "MemMap.h, no valid matching start-stop section defined."
    #endif
    #ifdef MEMMAP_MATCH_ERROR
        #undef MEMMAP_MATCH_ERROR
    #endif
    #undef APP_STOP_SEC_CODE_ITCM
    /**
    * @file App_MemMap.h
    */
    #undef MEMMAP_ERROR
    #pragma GCC section text
#endif

#ifdef APP_START_SEC_VAR_CLEARED_UNSPECIFIED_RAM
    /**
    * @file App_MemMap.h
    */
    #undef APP_START_SEC_VAR_CLEARED_UNSPECIFIED_RAM
    #define ENTERED_APP_START_SEC_VAR_CLEARED_UNSPECIFIED_RAM
    #ifndef MEMMAP_MATCH_ERROR
        #define MEMMAP_MATCH_ERROR
    #else
        #ifndef APP_STOP_SEC_VAR_CLEARED_UNSPECIFIED_RAM
        #error "MemMap.h, no valid matching start-stop section defined."
        #endif
    #endif
    /**
    * @file App_MemMap.h
    */
    #undef MEMMAP_ERROR
    #pragma GCC section bss ".mcal_bss"
#endif

#ifdef APP_STOP_SEC_VAR_CLEARED_UNSPECIFIED_RAM
    /**
    * @file App_MemMap.h
    */
    #ifdef ENTERED_APP_START_SEC_VAR_CLEARED_UNSPECIFIED_RAM
        #undef ENTERED_APP_START_SEC_VAR_CLEARED_UNSPECIFIED_RAM
    #else
        #error "MemMap.h, no valid matching start-stop section defined."
    #endif
    #ifdef MEMMAP_MATCH_ERROR
        #undef MEMMAP_MATCH_ERROR
    #endif
    #undef APP_STOP_SEC_VAR_CLEARED_UNSPECIFIED_RAM
    /**
    * @file App_MemMap.h
    */
    #undef MEMMAP_ERROR
    #pragma GCC section bss
#endif

#ifdef APP_START_SEC_VAR_CLEARED_UNSPECIFIED_DTCM
    /**
    * @file App_MemMap.h
    */
    #undef APP_START_SEC_VAR_CLEARED_UNSPECIFIED_DTCM
    #define ENTERED_APP_START_SEC_VAR_CLEARED_UNSPECIFIED_DTCM
    #ifndef MEMMAP_MATCH_ERROR
        #define MEMMAP_MATCH_ERROR
    #else
        #ifndef APP_STOP_SEC_VAR_CLEARED_UNSPECIFIED_DTCM
        #error "MemMap.h, no valid matching start-stop section defined."
        #endif
    #endif
    /**
    * @file App_MemMap.h
    */
    #undef MEMMAP_ERROR
    #pragma GCC section bss ".dtcm_bss"
#endif

#ifdef APP_STOP_SEC_VAR_CLEARED_UNSPECIFIED_DTCM
    /**
    * @file App_MemMap.h
    */
    #ifdef ENTERED_APP_START_SEC_VAR_CLEARED_UNSPECIFIED_DTCM
        #undef ENTERED_APP_START_SEC_VAR_CLEARED_UNSPECIFIED_DTCM
    #else
        #error "MemMap.h, no valid matching start-stop section defined."
    #endif
    #ifdef MEMMAP_MATCH_ERROR
        #undef MEMMAP_MATCH_ERROR
    #endif
    #undef APP_STOP_SEC_VAR_CLEARED_UNSPECIFIED_DTCM
    /**
    * @file App_MemMap.h
    */
    #undef MEMMAP_ERROR
    #pragma GCC section bss
#endif

#ifdef APP_START_SEC_VAR_INIT_UNSPECIFIED_RAM
    /**
    * @file App_MemMap.h
    */
    #undef APP_START_SEC_VAR_INIT_UNSPECIFIED_RAM
    #define ENTERED_APP_START_SEC_VAR_INIT_UNSPECIFIED_RAM
    #ifndef MEMMAP_MATCH_ERROR
        #define MEMMAP_MATCH_ERROR
    #else
        #ifndef APP_STOP_SEC_VAR_INIT_UNSPECIFIED_RAM
        #error "MemMap.h, no valid matching start-stop section defined."
        #endif
    #endif
    /**
    * @file App_MemMap.h
    */
    #undef MEMMAP_ERROR
    #pragma GCC section data ".mcal_data"
    #pragma GCC section bss ".mcal_bss"
#endif

#ifdef APP_STOP_SEC_VAR_INIT_UNSPECIFIED_RAM
    /**
    * @file App_MemMap.h
    */
    #ifdef ENTERED_APP_START_SEC_VAR_INIT_UNSPECIFIED_RAM
        #undef ENTERED_APP_START_SEC_VAR_INIT_UNSPECIFIED_RAM
    #else
        #error "MemMap.h, no valid matching start-stop section defined."
    #endif
    #ifdef MEMMAP_MATCH_ERROR
        #undef MEMMAP_MATCH_ERROR
    #endif
    #undef APP_STOP_SEC_VAR_INIT_UNSPECIFIED_RAM
    /**
    * @file App_MemMap.h
    */
    #undef MEMMAP_ERROR
    #pragma GCC section bss
    #pragma GCC section data
#endif

#ifdef APP_START_SEC_VAR_INIT_UNSPECIFIED_DTCM
    /**
    * @file App_MemMap.h
    */
    #undef APP_START_SEC_VAR_INIT_UNSPECIFIED_DTCM
    #define ENTERED_APP_START_SEC_VAR_INIT_UNSPECIFIED_DTCM
    #ifndef MEMMAP_MATCH_ERROR
        #define MEMMAP_MATCH_ERROR
    #else
        #ifndef APP_STOP_SEC_VAR_INIT_UNSPECIFIED_DTCM
        #error "MemMap.h, no valid matching start-stop section defined."
        #endif
    #endif
    /**
    * @file App_MemMap.h
    */
    #undef MEMMAP_ERROR
    #pragma GCC section data ".dtcm_data"
    #pragma GCC section bss ".dtcm_bss"
#endif

#ifdef APP_STOP_SEC_VAR_INIT_UNSPECIFIED_DTCM
    /**
    * @file App_MemMap.h
    */
    #ifdef ENTERED_APP_START_SEC_VAR_INIT_UNSPECIFIED_DTCM
        #undef ENTERED_APP_START_SEC_VAR_INIT_UNSPECIFIED_DTCM
    #else
        #error "MemMap.h, no valid matching start-stop section defined."
    #endif
    #ifdef MEMMAP_MATCH_ERROR
        #undef MEMMAP_MATCH_ERROR
    #endif
    #undef APP_STOP_SEC_VAR_INIT_UNSPECIFIED_DTCM
    /**
    * @file App_MemMap.h
    */
    #undef MEMMAP_ERROR
    #pragma GCC section bss
    #pragma GCC section data
#endif
/**************************************************************************************************/
