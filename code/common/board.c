/*
    ChibiOS/RT - Copyright (C) 2006-2013 Giovanni Di Sirio

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

#include "ch.h"
#include "hal.h"
#include "vectors.h"
#include "common.h"
#include <stdlib.h>

extern void initialise_monitor_handles(void);

#if defined(VECTORS_SECTION)
typedef struct {
  uint32_t      *init_stack;
  irq_vector_t  reset_handler;
} vectors_base_t;

extern uint32_t __main_stack_end__;
extern void Reset_Handler(void);

__attribute__ ((used, aligned(128), section(".vectors")))
vectors_base_t _vectors_base = {&__main_stack_end__, Reset_Handler};
#endif

#if HAL_USE_PAL || defined(__DOXYGEN__)
/**
 * @brief   PAL setup.
 * @details Digital I/O ports static configuration as defined in @p board.h.
 *          This variable is used by the HAL when initializing the PAL driver.
 */
const PALConfig pal_default_config =
{
  {VAL_GPIOA_MODER, VAL_GPIOA_OTYPER, VAL_GPIOA_OSPEEDR, VAL_GPIOA_PUPDR,
   VAL_GPIOA_ODR,   VAL_GPIOA_AFRL,   VAL_GPIOA_AFRH},
  {VAL_GPIOB_MODER, VAL_GPIOB_OTYPER, VAL_GPIOB_OSPEEDR, VAL_GPIOB_PUPDR,
   VAL_GPIOB_ODR,   VAL_GPIOB_AFRL,   VAL_GPIOB_AFRH},
  {VAL_GPIOC_MODER, VAL_GPIOC_OTYPER, VAL_GPIOC_OSPEEDR, VAL_GPIOC_PUPDR,
   VAL_GPIOC_ODR,   VAL_GPIOC_AFRL,   VAL_GPIOC_AFRH},
  {VAL_GPIOD_MODER, VAL_GPIOD_OTYPER, VAL_GPIOD_OSPEEDR, VAL_GPIOD_PUPDR,
   VAL_GPIOD_ODR,   VAL_GPIOD_AFRL,   VAL_GPIOD_AFRH},
  {VAL_GPIOE_MODER, VAL_GPIOE_OTYPER, VAL_GPIOE_OSPEEDR, VAL_GPIOE_PUPDR,
   VAL_GPIOE_ODR,   VAL_GPIOE_AFRL,   VAL_GPIOE_AFRH},
  {VAL_GPIOF_MODER, VAL_GPIOF_OTYPER, VAL_GPIOF_OSPEEDR, VAL_GPIOF_PUPDR,
   VAL_GPIOF_ODR,   VAL_GPIOF_AFRL,   VAL_GPIOF_AFRH}
};
#endif

/**
 * @brief   Early initialization code.
 * @details This initialization must be performed just after stack setup
 *          and before any other initialization.
 */
void __early_init(void) {

  stm32_clock_init();
}

/**
 * @brief   Board-specific initialization code.
 * @todo    Add your board-specific code, if any.
 */
void boardInit(void) {

  DBGMCU->APB1FZ |= DBGMCU_APB1_FZ_DBG_TIM2_STOP;
  DBGMCU->APB1FZ |= DBGMCU_APB1_FZ_DBG_TIM3_STOP;
  DBGMCU->APB1FZ |= DBGMCU_APB1_FZ_DBG_CAN_STOP;
  DBGMCU->APB2FZ |= DBGMCU_APB2_FZ_DBG_TIM1_STOP;

  DBGMCU->APB1FZ |= DBGMCU_APB1_FZ_DBG_IWDG_STOP;
  DBGMCU->APB1FZ |= DBGMCU_APB1_FZ_DBG_WWDG_STOP;

  DBGMCU->CR |= DBGMCU_CR_DBG_SLEEP;
  DBGMCU->CR |= DBGMCU_CR_DBG_STOP;

  /* Generate IRQ (PVD_IRQn) if VDD goes below 2.9v */
  PWR->CR |= PWR_CR_PLS_LEV7;
  PWR->CR |= PWR_CR_PVDE;

  nvicEnableVector(PVD_IRQn, 0); /* Max priority */
  srand(ch.tm.offset);
}
