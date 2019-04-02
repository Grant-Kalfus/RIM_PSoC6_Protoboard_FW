/* ========================================
 *
 * Copyright YOUR COMPANY, THE YEAR
 * All Rights Reserved
 * UNPUBLISHED, LICENSED SOFTWARE.
 *
 * CONFIDENTIAL AND PROPRIETARY INFORMATION
 * WHICH IS THE PROPERTY OF your company.
 *
 * ========================================
*/

/* [] END OF FILE */
#ifndef _L6470_regdef_
#define _L6470_regdef_

    //#include "project.h"
    #include "L6470_regdef.h"
	#include "cy_sysint.h"

    typedef unsigned char byte;
    typedef unsigned int uint;
    typedef unsigned short uint16;

    extern cy_stc_scb_spi_context_t SPI_context;
    extern cy_stc_sysint_t SPI_intr_cfg;

    byte _bufferTx[1];
    byte _bufferRx[1];

    //Modified
    byte transfer(byte, byte enable_id);

    //Writes the enable pin of 'motor id' to 'state'
    void enable_translater(byte state, byte enable_id);

    //Unmodified
    uint param_handler(byte param, uint value, byte enable_id);

    uint send_and_receive(uint value, byte blen, byte enable_id);

    void set_param(byte param, uint value, byte enable_id);

    uint get_param(byte param, byte enable_id);

    void motor_move(byte dir, uint16 n_step, byte enable_id);

    uint acc_calc(float stepsPerSecPerSec);

    uint dec_calc(float stepsPerSecPerSec);

    uint max_speed_calc(float stepsPerSec);

    uint min_speed_calc(float stepsPerSec);

    uint fs_calc(float stepsPerSec);

    uint get_status(byte enable_id);


#endif
