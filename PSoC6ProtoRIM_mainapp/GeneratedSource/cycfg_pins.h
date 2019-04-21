/*******************************************************************************
* File Name: cycfg_pins.h
*
* Description:
* Pin configuration
* This file was automatically generated and should not be modified.
* 
********************************************************************************
* Copyright 2017-2019 Cypress Semiconductor Corporation
* SPDX-License-Identifier: Apache-2.0
*
* Licensed under the Apache License, Version 2.0 (the "License");
* you may not use this file except in compliance with the License.
* You may obtain a copy of the License at
*
*     http://www.apache.org/licenses/LICENSE-2.0
*
* Unless required by applicable law or agreed to in writing, software
* distributed under the License is distributed on an "AS IS" BASIS,
* WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
* See the License for the specific language governing permissions and
* limitations under the License.
********************************************************************************/

#if !defined(CYCFG_PINS_H)
#define CYCFG_PINS_H

#include "cycfg_notices.h"
#include "cy_gpio.h"
#include "cycfg_routing.h"

#if defined(__cplusplus)
extern "C" {
#endif

#define ioss_0_port_0_pin_2_PORT GPIO_PRT0
#define ioss_0_port_0_pin_2_PIN 2U
#define ioss_0_port_0_pin_2_NUM 2U
#define ioss_0_port_0_pin_2_DRIVEMODE CY_GPIO_DM_OD_DRIVESLOW
#define ioss_0_port_0_pin_2_INIT_DRIVESTATE 1
#ifndef ioss_0_port_0_pin_2_HSIOM
	#define ioss_0_port_0_pin_2_HSIOM HSIOM_SEL_GPIO
#endif
#define ioss_0_port_0_pin_2_IRQ ioss_interrupts_gpio_0_IRQn
#define ioss_0_port_0_pin_3_PORT GPIO_PRT0
#define ioss_0_port_0_pin_3_PIN 3U
#define ioss_0_port_0_pin_3_NUM 3U
#define ioss_0_port_0_pin_3_DRIVEMODE CY_GPIO_DM_OD_DRIVESLOW
#define ioss_0_port_0_pin_3_INIT_DRIVESTATE 1
#ifndef ioss_0_port_0_pin_3_HSIOM
	#define ioss_0_port_0_pin_3_HSIOM HSIOM_SEL_GPIO
#endif
#define ioss_0_port_0_pin_3_IRQ ioss_interrupts_gpio_0_IRQn
#define e3_enable_PORT GPIO_PRT0
#define e3_enable_PIN 4U
#define e3_enable_NUM 4U
#define e3_enable_DRIVEMODE CY_GPIO_DM_STRONG
#define e3_enable_INIT_DRIVESTATE 1
#ifndef ioss_0_port_0_pin_4_HSIOM
	#define ioss_0_port_0_pin_4_HSIOM HSIOM_SEL_GPIO
#endif
#define e3_enable_HSIOM ioss_0_port_0_pin_4_HSIOM
#define e3_enable_IRQ ioss_interrupts_gpio_0_IRQn
#define ioss_0_port_10_pin_0_PORT GPIO_PRT10
#define ioss_0_port_10_pin_0_PIN 0U
#define ioss_0_port_10_pin_0_NUM 0U
#define ioss_0_port_10_pin_0_DRIVEMODE CY_GPIO_DM_STRONG_IN_OFF
#define ioss_0_port_10_pin_0_INIT_DRIVESTATE 1
#ifndef ioss_0_port_10_pin_0_HSIOM
	#define ioss_0_port_10_pin_0_HSIOM HSIOM_SEL_GPIO
#endif
#define ioss_0_port_10_pin_0_IRQ ioss_interrupts_gpio_10_IRQn
#define ioss_0_port_10_pin_1_PORT GPIO_PRT10
#define ioss_0_port_10_pin_1_PIN 1U
#define ioss_0_port_10_pin_1_NUM 1U
#define ioss_0_port_10_pin_1_DRIVEMODE CY_GPIO_DM_HIGHZ
#define ioss_0_port_10_pin_1_INIT_DRIVESTATE 1
#ifndef ioss_0_port_10_pin_1_HSIOM
	#define ioss_0_port_10_pin_1_HSIOM HSIOM_SEL_GPIO
#endif
#define ioss_0_port_10_pin_1_IRQ ioss_interrupts_gpio_10_IRQn
#define ioss_0_port_10_pin_2_PORT GPIO_PRT10
#define ioss_0_port_10_pin_2_PIN 2U
#define ioss_0_port_10_pin_2_NUM 2U
#define ioss_0_port_10_pin_2_DRIVEMODE CY_GPIO_DM_STRONG_IN_OFF
#define ioss_0_port_10_pin_2_INIT_DRIVESTATE 1
#ifndef ioss_0_port_10_pin_2_HSIOM
	#define ioss_0_port_10_pin_2_HSIOM HSIOM_SEL_GPIO
#endif
#define ioss_0_port_10_pin_2_IRQ ioss_interrupts_gpio_10_IRQn
#define m3_enable_PORT GPIO_PRT11
#define m3_enable_PIN 2U
#define m3_enable_NUM 2U
#define m3_enable_DRIVEMODE CY_GPIO_DM_STRONG
#define m3_enable_INIT_DRIVESTATE 1
#ifndef ioss_0_port_11_pin_2_HSIOM
	#define ioss_0_port_11_pin_2_HSIOM HSIOM_SEL_GPIO
#endif
#define m3_enable_HSIOM ioss_0_port_11_pin_2_HSIOM
#define m3_enable_IRQ ioss_interrupts_gpio_11_IRQn
#define busy_3_PORT GPIO_PRT11
#define busy_3_PIN 3U
#define busy_3_NUM 3U
#define busy_3_DRIVEMODE CY_GPIO_DM_HIGHZ
#define busy_3_INIT_DRIVESTATE 1
#ifndef ioss_0_port_11_pin_3_HSIOM
	#define ioss_0_port_11_pin_3_HSIOM HSIOM_SEL_GPIO
#endif
#define busy_3_HSIOM ioss_0_port_11_pin_3_HSIOM
#define busy_3_IRQ ioss_interrupts_gpio_11_IRQn
#define busy_4_PORT GPIO_PRT12
#define busy_4_PIN 1U
#define busy_4_NUM 1U
#define busy_4_DRIVEMODE CY_GPIO_DM_HIGHZ
#define busy_4_INIT_DRIVESTATE 1
#ifndef ioss_0_port_12_pin_1_HSIOM
	#define ioss_0_port_12_pin_1_HSIOM HSIOM_SEL_GPIO
#endif
#define busy_4_HSIOM ioss_0_port_12_pin_1_HSIOM
#define busy_4_IRQ ioss_interrupts_gpio_12_IRQn
#define rst_3_PORT GPIO_PRT12
#define rst_3_PIN 3U
#define rst_3_NUM 3U
#define rst_3_DRIVEMODE CY_GPIO_DM_STRONG
#define rst_3_INIT_DRIVESTATE 1
#ifndef ioss_0_port_12_pin_3_HSIOM
	#define ioss_0_port_12_pin_3_HSIOM HSIOM_SEL_GPIO
#endif
#define rst_3_HSIOM ioss_0_port_12_pin_3_HSIOM
#define rst_3_IRQ ioss_interrupts_gpio_12_IRQn
#define m4_enable_PORT GPIO_PRT12
#define m4_enable_PIN 4U
#define m4_enable_NUM 4U
#define m4_enable_DRIVEMODE CY_GPIO_DM_STRONG
#define m4_enable_INIT_DRIVESTATE 1
#ifndef ioss_0_port_12_pin_4_HSIOM
	#define ioss_0_port_12_pin_4_HSIOM HSIOM_SEL_GPIO
#endif
#define m4_enable_HSIOM ioss_0_port_12_pin_4_HSIOM
#define m4_enable_IRQ ioss_interrupts_gpio_12_IRQn
#define rst_4_PORT GPIO_PRT12
#define rst_4_PIN 5U
#define rst_4_NUM 5U
#define rst_4_DRIVEMODE CY_GPIO_DM_STRONG
#define rst_4_INIT_DRIVESTATE 1
#ifndef ioss_0_port_12_pin_5_HSIOM
	#define ioss_0_port_12_pin_5_HSIOM HSIOM_SEL_GPIO
#endif
#define rst_4_HSIOM ioss_0_port_12_pin_5_HSIOM
#define rst_4_IRQ ioss_interrupts_gpio_12_IRQn
#define e4_enable_PORT GPIO_PRT13
#define e4_enable_PIN 6U
#define e4_enable_NUM 6U
#define e4_enable_DRIVEMODE CY_GPIO_DM_STRONG
#define e4_enable_INIT_DRIVESTATE 1
#ifndef ioss_0_port_13_pin_6_HSIOM
	#define ioss_0_port_13_pin_6_HSIOM HSIOM_SEL_GPIO
#endif
#define e4_enable_HSIOM ioss_0_port_13_pin_6_HSIOM
#define e4_enable_IRQ ioss_interrupts_gpio_13_IRQn
#define ioss_0_port_2_pin_4_PORT GPIO_PRT2
#define ioss_0_port_2_pin_4_PIN 4U
#define ioss_0_port_2_pin_4_NUM 4U
#define ioss_0_port_2_pin_4_DRIVEMODE CY_GPIO_DM_OD_DRIVESLOW
#define ioss_0_port_2_pin_4_INIT_DRIVESTATE 1
#ifndef ioss_0_port_2_pin_4_HSIOM
	#define ioss_0_port_2_pin_4_HSIOM HSIOM_SEL_GPIO
#endif
#define ioss_0_port_2_pin_4_IRQ ioss_interrupts_gpio_2_IRQn
#define ioss_0_port_2_pin_5_PORT GPIO_PRT2
#define ioss_0_port_2_pin_5_PIN 5U
#define ioss_0_port_2_pin_5_NUM 5U
#define ioss_0_port_2_pin_5_DRIVEMODE CY_GPIO_DM_OD_DRIVESLOW
#define ioss_0_port_2_pin_5_INIT_DRIVESTATE 1
#ifndef ioss_0_port_2_pin_5_HSIOM
	#define ioss_0_port_2_pin_5_HSIOM HSIOM_SEL_GPIO
#endif
#define ioss_0_port_2_pin_5_IRQ ioss_interrupts_gpio_2_IRQn
#define ioss_0_port_5_pin_0_PORT GPIO_PRT5
#define ioss_0_port_5_pin_0_PIN 0U
#define ioss_0_port_5_pin_0_NUM 0U
#define ioss_0_port_5_pin_0_DRIVEMODE CY_GPIO_DM_HIGHZ
#define ioss_0_port_5_pin_0_INIT_DRIVESTATE 1
#ifndef ioss_0_port_5_pin_0_HSIOM
	#define ioss_0_port_5_pin_0_HSIOM HSIOM_SEL_GPIO
#endif
#define ioss_0_port_5_pin_0_IRQ ioss_interrupts_gpio_5_IRQn
#define ioss_0_port_5_pin_1_PORT GPIO_PRT5
#define ioss_0_port_5_pin_1_PIN 1U
#define ioss_0_port_5_pin_1_NUM 1U
#define ioss_0_port_5_pin_1_DRIVEMODE CY_GPIO_DM_STRONG_IN_OFF
#define ioss_0_port_5_pin_1_INIT_DRIVESTATE 1
#ifndef ioss_0_port_5_pin_1_HSIOM
	#define ioss_0_port_5_pin_1_HSIOM HSIOM_SEL_GPIO
#endif
#define ioss_0_port_5_pin_1_IRQ ioss_interrupts_gpio_5_IRQn
#define rst_0_PORT GPIO_PRT5
#define rst_0_PIN 2U
#define rst_0_NUM 2U
#define rst_0_DRIVEMODE CY_GPIO_DM_STRONG
#define rst_0_INIT_DRIVESTATE 1
#ifndef ioss_0_port_5_pin_2_HSIOM
	#define ioss_0_port_5_pin_2_HSIOM HSIOM_SEL_GPIO
#endif
#define rst_0_HSIOM ioss_0_port_5_pin_2_HSIOM
#define rst_0_IRQ ioss_interrupts_gpio_5_IRQn
#define busy_0_PORT GPIO_PRT5
#define busy_0_PIN 4U
#define busy_0_NUM 4U
#define busy_0_DRIVEMODE CY_GPIO_DM_HIGHZ
#define busy_0_INIT_DRIVESTATE 1
#ifndef ioss_0_port_5_pin_4_HSIOM
	#define ioss_0_port_5_pin_4_HSIOM HSIOM_SEL_GPIO
#endif
#define busy_0_HSIOM ioss_0_port_5_pin_4_HSIOM
#define busy_0_IRQ ioss_interrupts_gpio_5_IRQn
#define m0_enable_PORT GPIO_PRT5
#define m0_enable_PIN 5U
#define m0_enable_NUM 5U
#define m0_enable_DRIVEMODE CY_GPIO_DM_STRONG
#define m0_enable_INIT_DRIVESTATE 1
#ifndef ioss_0_port_5_pin_5_HSIOM
	#define ioss_0_port_5_pin_5_HSIOM HSIOM_SEL_GPIO
#endif
#define m0_enable_HSIOM ioss_0_port_5_pin_5_HSIOM
#define m0_enable_IRQ ioss_interrupts_gpio_5_IRQn
#define rst_1_PORT GPIO_PRT6
#define rst_1_PIN 0U
#define rst_1_NUM 0U
#define rst_1_DRIVEMODE CY_GPIO_DM_STRONG
#define rst_1_INIT_DRIVESTATE 1
#ifndef ioss_0_port_6_pin_0_HSIOM
	#define ioss_0_port_6_pin_0_HSIOM HSIOM_SEL_GPIO
#endif
#define rst_1_HSIOM ioss_0_port_6_pin_0_HSIOM
#define rst_1_IRQ ioss_interrupts_gpio_6_IRQn
#define m1_enable_PORT GPIO_PRT6
#define m1_enable_PIN 1U
#define m1_enable_NUM 1U
#define m1_enable_DRIVEMODE CY_GPIO_DM_STRONG
#define m1_enable_INIT_DRIVESTATE 1
#ifndef ioss_0_port_6_pin_1_HSIOM
	#define ioss_0_port_6_pin_1_HSIOM HSIOM_SEL_GPIO
#endif
#define m1_enable_HSIOM ioss_0_port_6_pin_1_HSIOM
#define m1_enable_IRQ ioss_interrupts_gpio_6_IRQn
#define busy_1_PORT GPIO_PRT6
#define busy_1_PIN 2U
#define busy_1_NUM 2U
#define busy_1_DRIVEMODE CY_GPIO_DM_HIGHZ
#define busy_1_INIT_DRIVESTATE 1
#ifndef ioss_0_port_6_pin_2_HSIOM
	#define ioss_0_port_6_pin_2_HSIOM HSIOM_SEL_GPIO
#endif
#define busy_1_HSIOM ioss_0_port_6_pin_2_HSIOM
#define busy_1_IRQ ioss_interrupts_gpio_6_IRQn
#define SWO_PORT GPIO_PRT6
#define SWO_PIN 4U
#define SWO_NUM 4U
#define SWO_DRIVEMODE CY_GPIO_DM_STRONG_IN_OFF
#define SWO_INIT_DRIVESTATE 1
#ifndef ioss_0_port_6_pin_4_HSIOM
	#define ioss_0_port_6_pin_4_HSIOM HSIOM_SEL_GPIO
#endif
#define SWO_HSIOM ioss_0_port_6_pin_4_HSIOM
#define SWO_IRQ ioss_interrupts_gpio_6_IRQn
#define SWDIO_PORT GPIO_PRT6
#define SWDIO_PIN 6U
#define SWDIO_NUM 6U
#define SWDIO_DRIVEMODE CY_GPIO_DM_PULLUP
#define SWDIO_INIT_DRIVESTATE 1
#ifndef ioss_0_port_6_pin_6_HSIOM
	#define ioss_0_port_6_pin_6_HSIOM HSIOM_SEL_GPIO
#endif
#define SWDIO_HSIOM ioss_0_port_6_pin_6_HSIOM
#define SWDIO_IRQ ioss_interrupts_gpio_6_IRQn
#define SWCLK_PORT GPIO_PRT6
#define SWCLK_PIN 7U
#define SWCLK_NUM 7U
#define SWCLK_DRIVEMODE CY_GPIO_DM_PULLDOWN
#define SWCLK_INIT_DRIVESTATE 1
#ifndef ioss_0_port_6_pin_7_HSIOM
	#define ioss_0_port_6_pin_7_HSIOM HSIOM_SEL_GPIO
#endif
#define SWCLK_HSIOM ioss_0_port_6_pin_7_HSIOM
#define SWCLK_IRQ ioss_interrupts_gpio_6_IRQn
#define ioss_0_port_8_pin_0_PORT GPIO_PRT8
#define ioss_0_port_8_pin_0_PIN 0U
#define ioss_0_port_8_pin_0_NUM 0U
#define ioss_0_port_8_pin_0_DRIVEMODE CY_GPIO_DM_STRONG_IN_OFF
#define ioss_0_port_8_pin_0_INIT_DRIVESTATE 1
#ifndef ioss_0_port_8_pin_0_HSIOM
	#define ioss_0_port_8_pin_0_HSIOM HSIOM_SEL_GPIO
#endif
#define ioss_0_port_8_pin_0_IRQ ioss_interrupts_gpio_8_IRQn
#define ioss_0_port_8_pin_1_PORT GPIO_PRT8
#define ioss_0_port_8_pin_1_PIN 1U
#define ioss_0_port_8_pin_1_NUM 1U
#define ioss_0_port_8_pin_1_DRIVEMODE CY_GPIO_DM_HIGHZ
#define ioss_0_port_8_pin_1_INIT_DRIVESTATE 1
#ifndef ioss_0_port_8_pin_1_HSIOM
	#define ioss_0_port_8_pin_1_HSIOM HSIOM_SEL_GPIO
#endif
#define ioss_0_port_8_pin_1_IRQ ioss_interrupts_gpio_8_IRQn
#define ioss_0_port_8_pin_2_PORT GPIO_PRT8
#define ioss_0_port_8_pin_2_PIN 2U
#define ioss_0_port_8_pin_2_NUM 2U
#define ioss_0_port_8_pin_2_DRIVEMODE CY_GPIO_DM_STRONG_IN_OFF
#define ioss_0_port_8_pin_2_INIT_DRIVESTATE 1
#ifndef ioss_0_port_8_pin_2_HSIOM
	#define ioss_0_port_8_pin_2_HSIOM HSIOM_SEL_GPIO
#endif
#define ioss_0_port_8_pin_2_IRQ ioss_interrupts_gpio_8_IRQn
#define e0_enable_PORT GPIO_PRT8
#define e0_enable_PIN 3U
#define e0_enable_NUM 3U
#define e0_enable_DRIVEMODE CY_GPIO_DM_STRONG
#define e0_enable_INIT_DRIVESTATE 1
#ifndef ioss_0_port_8_pin_3_HSIOM
	#define ioss_0_port_8_pin_3_HSIOM HSIOM_SEL_GPIO
#endif
#define e0_enable_HSIOM ioss_0_port_8_pin_3_HSIOM
#define e0_enable_IRQ ioss_interrupts_gpio_8_IRQn
#define m2_enable_PORT GPIO_PRT8
#define m2_enable_PIN 5U
#define m2_enable_NUM 5U
#define m2_enable_DRIVEMODE CY_GPIO_DM_STRONG
#define m2_enable_INIT_DRIVESTATE 1
#ifndef ioss_0_port_8_pin_5_HSIOM
	#define ioss_0_port_8_pin_5_HSIOM HSIOM_SEL_GPIO
#endif
#define m2_enable_HSIOM ioss_0_port_8_pin_5_HSIOM
#define m2_enable_IRQ ioss_interrupts_gpio_8_IRQn
#define rst_2_PORT GPIO_PRT9
#define rst_2_PIN 0U
#define rst_2_NUM 0U
#define rst_2_DRIVEMODE CY_GPIO_DM_STRONG
#define rst_2_INIT_DRIVESTATE 1
#ifndef ioss_0_port_9_pin_0_HSIOM
	#define ioss_0_port_9_pin_0_HSIOM HSIOM_SEL_GPIO
#endif
#define rst_2_HSIOM ioss_0_port_9_pin_0_HSIOM
#define rst_2_IRQ ioss_interrupts_gpio_9_IRQn
#define e1_enable_PORT GPIO_PRT9
#define e1_enable_PIN 2U
#define e1_enable_NUM 2U
#define e1_enable_DRIVEMODE CY_GPIO_DM_STRONG
#define e1_enable_INIT_DRIVESTATE 1
#ifndef ioss_0_port_9_pin_2_HSIOM
	#define ioss_0_port_9_pin_2_HSIOM HSIOM_SEL_GPIO
#endif
#define e1_enable_HSIOM ioss_0_port_9_pin_2_HSIOM
#define e1_enable_IRQ ioss_interrupts_gpio_9_IRQn
#define busy_2_PORT GPIO_PRT9
#define busy_2_PIN 3U
#define busy_2_NUM 3U
#define busy_2_DRIVEMODE CY_GPIO_DM_HIGHZ
#define busy_2_INIT_DRIVESTATE 1
#ifndef ioss_0_port_9_pin_3_HSIOM
	#define ioss_0_port_9_pin_3_HSIOM HSIOM_SEL_GPIO
#endif
#define busy_2_HSIOM ioss_0_port_9_pin_3_HSIOM
#define busy_2_IRQ ioss_interrupts_gpio_9_IRQn
#define e2_enable_PORT GPIO_PRT9
#define e2_enable_PIN 6U
#define e2_enable_NUM 6U
#define e2_enable_DRIVEMODE CY_GPIO_DM_STRONG
#define e2_enable_INIT_DRIVESTATE 1
#ifndef ioss_0_port_9_pin_6_HSIOM
	#define ioss_0_port_9_pin_6_HSIOM HSIOM_SEL_GPIO
#endif
#define e2_enable_HSIOM ioss_0_port_9_pin_6_HSIOM
#define e2_enable_IRQ ioss_interrupts_gpio_9_IRQn

extern const cy_stc_gpio_pin_config_t ioss_0_port_0_pin_2_config;
extern const cy_stc_gpio_pin_config_t ioss_0_port_0_pin_3_config;
extern const cy_stc_gpio_pin_config_t e3_enable_config;
extern const cy_stc_gpio_pin_config_t ioss_0_port_10_pin_0_config;
extern const cy_stc_gpio_pin_config_t ioss_0_port_10_pin_1_config;
extern const cy_stc_gpio_pin_config_t ioss_0_port_10_pin_2_config;
extern const cy_stc_gpio_pin_config_t m3_enable_config;
extern const cy_stc_gpio_pin_config_t busy_3_config;
extern const cy_stc_gpio_pin_config_t busy_4_config;
extern const cy_stc_gpio_pin_config_t rst_3_config;
extern const cy_stc_gpio_pin_config_t m4_enable_config;
extern const cy_stc_gpio_pin_config_t rst_4_config;
extern const cy_stc_gpio_pin_config_t e4_enable_config;
extern const cy_stc_gpio_pin_config_t ioss_0_port_2_pin_4_config;
extern const cy_stc_gpio_pin_config_t ioss_0_port_2_pin_5_config;
extern const cy_stc_gpio_pin_config_t ioss_0_port_5_pin_0_config;
extern const cy_stc_gpio_pin_config_t ioss_0_port_5_pin_1_config;
extern const cy_stc_gpio_pin_config_t rst_0_config;
extern const cy_stc_gpio_pin_config_t busy_0_config;
extern const cy_stc_gpio_pin_config_t m0_enable_config;
extern const cy_stc_gpio_pin_config_t rst_1_config;
extern const cy_stc_gpio_pin_config_t m1_enable_config;
extern const cy_stc_gpio_pin_config_t busy_1_config;
extern const cy_stc_gpio_pin_config_t SWO_config;
extern const cy_stc_gpio_pin_config_t SWDIO_config;
extern const cy_stc_gpio_pin_config_t SWCLK_config;
extern const cy_stc_gpio_pin_config_t ioss_0_port_8_pin_0_config;
extern const cy_stc_gpio_pin_config_t ioss_0_port_8_pin_1_config;
extern const cy_stc_gpio_pin_config_t ioss_0_port_8_pin_2_config;
extern const cy_stc_gpio_pin_config_t e0_enable_config;
extern const cy_stc_gpio_pin_config_t m2_enable_config;
extern const cy_stc_gpio_pin_config_t rst_2_config;
extern const cy_stc_gpio_pin_config_t e1_enable_config;
extern const cy_stc_gpio_pin_config_t busy_2_config;
extern const cy_stc_gpio_pin_config_t e2_enable_config;

void init_cycfg_pins(void);

#if defined(__cplusplus)
}
#endif


#endif /* CYCFG_PINS_H */
