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

#ifndef __RIM_UI_DEF
#define __RIM_UI_DEF

	#include "cy_sysint.h"
	#include "cy_device_headers.h"
	#include "cycfg.h"
	#include "L6470_config.h"

	extern  cy_stc_scb_uart_context_t UARTD_context;
	extern cy_stc_sysint_t SPI_intr_cfg;

    //Definitions for RIM packets
    #define RIM_DIRECTION 0x08
    #define RIM_MOTOR_ID 0x07

    #define RIM_FW 0x00
    #define RIM_BW 0x01

    #define CMD_NONE 0x00
    #define CMD_QUEUED 0x01
    #define CMD_RUNNING 0x02




    #define L6470_NOT_BUSY 0x00
    #define L6470_BUSY 0x01

    //SPI Enable Pin Values
	#define RIM_M0_ENABLE 0x00
	#define RIM_M1_ENABLE 0x01
	#define RIM_M2_ENABLE 0x02
	#define RIM_M3_ENABLE 0x03
	#define RIM_M4_ENABLE 0x04


    //#define RIM_E0_ENABLE 0x00
    //#define RIM_E1_ENABLE 0x03


    //RIM OpCodes
    #define RIM_OP_MOTOR_RUN           0x00
    #define RIM_OP_MOTOR_STOP          0x10
    #define RIM_OP_MOTOR_GETSET_PARAM  0x20
    #define RIM_OP_MOTOR_STATUS        0x30
    #define RIM_OP_ENCODER_INFO        0x40
    #define RIM_OP_ERROR               0x50

    #define RIM_OP_MOTOR_EXTENDED_STEP 0x80

	//RIM Opcode Specials

	//For RIM_OP_MOTOR_STOP
	#define ONE_SSTOP 0x00
	#define ONE_HSTOP 0x01
	#define ALL_SSTOP 0x02
	#define ALL_HSTOP 0x03

	//For RIM_GETSET_PARAM
	#define GETSET_GET_PARAM 0x00
	#define GETSET_SET_PARAM 0x01

	#define GETSET_RECIEVED_PARAM_DATA 0xFFE0
	#define GETSET_RECIEVED_PARAM_TYPE 0x001F
	#define GETSET_RECIEVED_ACCESSOR 0x08

    //CUI Encoder Constants
    #define CUI_NOP 0x00
    #define CUI_NO_DATA 0xA5
    #define CUI_READ_POS 0x10
    #define CUI_SET_ZERO_POS 0x70

    //CUI Encoder Functions
    void CUI_transfer(uint8 command, uint8 enable_id);

    uint16 CUI_read(uint8 enable_id);

    uint16 CUI_get_position(uint8 enable_id);

    void RIM_enableWrite(uint8 enable_id, uint8 state);

    struct encoders {
        //Stores which enable pin needs to be pulled down for SPI communications
        //Will be loaded into the enable control register component
        uint8 enable_id;


        uint8 received_cmd;
        uint8 command_type;
        uint8 is_busy;
    };


    struct motors {
        //Stores which enable pin needs to be pulled down for SPI communications

        uint8 enable_id;

        //Stores number of steps
        uint16 steps;

        //Stores motor direction
        uint8 motor_dir;

        //True false
        uint8 received_cmd;

        //Type of command received
        uint8 command_type;

        //Keeps track if the device is busy
        uint32_t is_busy;
    };


#endif
