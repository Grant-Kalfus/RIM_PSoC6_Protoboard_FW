/*******************************************************************************
* File Name: cycfg_peripherals.h
*
* Description:
* Peripheral Hardware Block configuration
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

#if !defined(CYCFG_PERIPHERALS_H)
#define CYCFG_PERIPHERALS_H

#include "cycfg_notices.h"
#include "cy_scb_spi.h"
#include "cy_sysclk.h"
#include "cy_scb_uart.h"
#include "cy_tcpwm_pwm.h"
#include "cycfg_routing.h"

#if defined(__cplusplus)
extern "C" {
#endif

#define SPI_HW SCB1
#define SPI_IRQ scb_1_interrupt_IRQn
#define UARTD_HW SCB5
#define UARTD_IRQ scb_5_interrupt_IRQn
#define SERVO_1_HW TCPWM0
#define SERVO_1_NUM 1UL
#define SERVO_1_MASK (1UL << 1)
#define SERVO_2_HW TCPWM0
#define SERVO_2_NUM 2UL
#define SERVO_2_MASK (1UL << 2)

extern const cy_stc_scb_spi_config_t SPI_config;
extern const cy_stc_scb_uart_config_t UARTD_config;
extern const cy_stc_tcpwm_pwm_config_t SERVO_1_config;
extern const cy_stc_tcpwm_pwm_config_t SERVO_2_config;

void init_cycfg_peripherals(void);

#if defined(__cplusplus)
}
#endif


#endif /* CYCFG_PERIPHERALS_H */
