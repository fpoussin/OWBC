/*
    ChibiOS - Copyright (C) 2014 Uladzimir Pylinsky aka barthess

    Licensed under the Apache License, Version 2.0 (the "License");
    you may not use this file except in compliance with the License.
    You may obtain a copy of the License at

        http://www.apache.org/licenses/LICENSE-2.0

    Unless required by applicable law or agreed to in writing, software
    distributed under the License is distributed on an "AS IS" BASIS,
    WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
    See the License for the specific language governing permissions and
    limitations under the License.
*/

#ifndef _HALCONF_COMMUNITY_H_
#define _HALCONF_COMMUNITY_H_

/**
 * @brief   Enables the community overlay.
 */
#if !defined(HAL_USE_COMMUNITY) || defined(__DOXYGEN__)
#define HAL_USE_COMMUNITY           TRUE
#endif

/**
 * @brief   Enables the FSMC subsystem.
 */
#if !defined(HAL_USE_FSMC) || defined(__DOXYGEN__)
#define HAL_USE_FSMC                FALSE
#endif

/**
 * @brief   Enables the NAND subsystem.
 */
#if !defined(HAL_USE_NAND) || defined(__DOXYGEN__)
#define HAL_USE_NAND                FALSE
#endif

/**
 * @brief   Enables the 1-wire subsystem.
 */
#if !defined(HAL_USE_ONEWIRE) || defined(__DOXYGEN__)
#define HAL_USE_ONEWIRE             FALSE
#endif

/**
 * @brief   Enables the EICU subsystem.
 */
#if !defined(HAL_USE_EICU) || defined(__DOXYGEN__)
#define HAL_USE_EICU                FALSE
#endif

/**
 * @brief   Enables the CRC subsystem.
 */
#if !defined(HAL_USE_CRC) || defined(__DOXYGEN__)
#define HAL_USE_CRC                 TRUE
#endif

/**
 * @brief   Enables the USBH subsystem.
 */
#if !defined(HAL_USE_USBH) || defined(__DOXYGEN__)
#define HAL_USE_USBH            FALSE
#endif

/**
 * @brief   Enables the EEPROM subsystem.
 */
#if !defined(HAL_USE_EEPROM) || defined(__DOXYGEN__)
#define HAL_USE_EEPROM            TRUE
#endif

/**
 * @brief   Enables the TIMCAP subsystem.
 */
#if !defined(HAL_USE_TIMCAP) || defined(__DOXYGEN__)
#define HAL_USE_TIMCAP            FALSE
#endif

/*===========================================================================*/
/* FSMCNAND driver related settings.                                         */
/*===========================================================================*/

/**
 * @brief   Enables the @p nandAcquireBus() and @p nanReleaseBus() APIs.
 * @note    Disabling this option saves both code and data space.
 */
#if !defined(NAND_USE_MUTUAL_EXCLUSION) || defined(__DOXYGEN__)
#define NAND_USE_MUTUAL_EXCLUSION   TRUE
#endif

/*===========================================================================*/
/* 1-wire driver related settings.                                           */
/*===========================================================================*/
/**
 * @brief   Enables strong pull up feature.
 * @note    Disabling this option saves both code and data space.
 */
#define ONEWIRE_USE_STRONG_PULLUP   FALSE

/**
 * @brief   Enables search ROM feature.
 * @note    Disabling this option saves both code and data space.
 */
#define ONEWIRE_USE_SEARCH_ROM      FALSE


/*===========================================================================*/
/* EEProm driver related settings.                                           */
/*===========================================================================*/

#define EEPROM_USE_EE24XX FALSE
#define EEPROM_USE_EE25XX TRUE

/*===========================================================================*/
/* CRC driver settings.                                                      */
/*===========================================================================*/

#define rccEnableCRC(lp) rccEnableAHB(RCC_AHBENR_CRCEN, lp)
#define rccDisableCRC(lp) rccDisableAHB(RCC_AHBENR_CRCEN, lp)

/**
 * @brief   Enables DMA engine when performing CRC transactions.
 * @note    Enabling this option also enables asynchronous API.
 */
#if !defined(CRC_USE_DMA) || defined(__DOXYGEN__)
#define CRC_USE_DMA                 FALSE
#endif

/**
 * @brief   Enables the @p crcAcquireUnit() and @p crcReleaseUnit() APIs.
 * @note    Disabling this option saves both code and data space.
 */
#if !defined(CRC_USE_MUTUAL_EXCLUSION) || defined(__DOXYGEN__)
#define CRC_USE_MUTUAL_EXCLUSION    TRUE
#endif


#endif /* _HALCONF_COMMUNITY_H_ */

/** @} */
