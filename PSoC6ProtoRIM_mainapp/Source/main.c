/*
* Copyright YOUR COMPANY, THE YEAR
* All Rights Reserved
* UNPUBLISHED, LICENSED SOFTWARE.
*
* CONFIDENTIAL AND PROPRIETARY INFORMATION
* WHICH IS THE PROPERTY OF your company.
*/



#include <stdio.h>
#include "RIM_UI_def.h"
#include "L6470_config.h"
#define CURRENTLY_CONNECTED_MOTORS   0
#define CURRENTLY_CONNECTED_ENCODERS 5

cy_stc_scb_uart_context_t UARTD_context;
cy_stc_sysint_t UARTD_intr_cfg =
{
		.intrSrc = (IRQn_Type) UARTD_IRQ,
		.intrPriority = 6u
};

cy_stc_scb_spi_context_t SPI_context;
cy_stc_sysint_t SPI_intr_cfg =
{
		.intrSrc = (IRQn_Type) SPI_IRQ,
		.intrPriority = 7u
};


uint8 check_busy(byte dev_id);

//For Debugging
uint seeval;
int cur_bit_field = 0;


//Currently used for debugging
uint8 cmd_bytes[3];

uint8 cur_motor_id;

struct motors RIM_Motors[7];
struct encoders RIM_Encoders[5];

uint8 opcode;


//ISR for interpreting input from the computer
//Expects three bytes of data
//ISR runs a total of 3 times per full transmission
//void Interrupt_Handler_UART (void);
void Interrupt_Handler_UART(void)
{
//Method 1

    // --mod from PSoC4--
    char received_uart_char = Cy_SCB_UART_Get(UARTD_HW);
    Cy_SCB_ClearRxInterrupt(UARTD_HW, CY_SCB_RX_INTR_NOT_EMPTY);

    cur_bit_field++;
    //Byte 1
    if(cur_bit_field == 1)
    {
        //command = recieved_uart_char;
        cmd_bytes[0] = received_uart_char;

        //Expected bit field:
        //_ _ _ _ |_ _ _ _
        //O O O O | D M M M
        //O = Opcode, D = Direction, M = Motor ID, U = unused

        //Get Opcode
        opcode = received_uart_char & 0xF0;
        switch(opcode)
        {
            //If the incoming command is a motor run command
            case RIM_OP_MOTOR_RUN:
                cur_motor_id = received_uart_char & RIM_MOTOR_ID;

                //Make sure there will be no command overlap
                if(!RIM_Motors[cur_motor_id].received_cmd == CMD_NONE)
                    cur_bit_field = 0;
                else
                {
                    RIM_Motors[cur_motor_id].motor_dir = cmd_bytes[0] >> 3;
                    RIM_Motors[cur_motor_id].command_type = RIM_OP_MOTOR_RUN;
                }
                break;

            case RIM_OP_MOTOR_STOP:
            	cur_motor_id = received_uart_char & RIM_MOTOR_ID;
                RIM_Motors[cur_motor_id].command_type = RIM_OP_MOTOR_STOP;
                break;

            case RIM_OP_MOTOR_STATUS:
                cur_motor_id = received_uart_char & RIM_MOTOR_ID;
                if(!RIM_Motors[cur_motor_id].received_cmd == CMD_NONE)
                    cur_bit_field = 0;
                else
                    RIM_Motors[cur_motor_id].command_type = RIM_OP_MOTOR_STATUS;
                break;

            //Direction bit used for GET / SET
            case RIM_OP_MOTOR_GETSET_PARAM:
            	cur_motor_id = received_uart_char & RIM_MOTOR_ID;
            	if(!RIM_Motors[cur_motor_id].received_cmd == CMD_NONE)
                    cur_bit_field = 0;
                else
                {
                    RIM_Motors[cur_motor_id].motor_dir = (cmd_bytes[0] & GETSET_RECIEVED_ACCESSOR) >> 3;
                    RIM_Motors[cur_motor_id].command_type = RIM_OP_MOTOR_GETSET_PARAM;
                }
            	break;

            case RIM_OP_ENCODER_INFO:
                cur_motor_id = received_uart_char & RIM_MOTOR_ID;
                if(RIM_Encoders[cur_motor_id].is_busy)
                    cur_bit_field = 0;
                else
                    RIM_Encoders[cur_motor_id].command_type = RIM_OP_ENCODER_INFO;
                break;
        }


    }
    //Byte 2
    else if(cur_bit_field == 2)
    {
        //LSB of 16b number of steps to take
        switch(opcode)
        {
            case RIM_OP_MOTOR_RUN:
                RIM_Motors[cur_motor_id].steps |= received_uart_char;
                break;
            //If the LSB of the number is FF, then the UI has issued an ALL_STOP command
            case RIM_OP_MOTOR_STOP:
            	RIM_Motors[cur_motor_id].steps = received_uart_char;
            	break;
            case RIM_OP_MOTOR_STATUS:
                break;
            //Steps is the argument for the parameter
            case RIM_OP_MOTOR_GETSET_PARAM:
            	RIM_Motors[cur_motor_id].steps |= received_uart_char;
            	break;
            case RIM_OP_ENCODER_INFO:
                break;
        }


        cmd_bytes[1] = received_uart_char;
    }

    //Byte 3
    else if(cur_bit_field == 3)
    {
        switch(opcode)
        {
            case RIM_OP_MOTOR_RUN:
                //MSB of 16b number of steps to take
                RIM_Motors[cur_motor_id].steps |= ((uint16)received_uart_char << 8);
                RIM_Motors[cur_motor_id].is_busy = L6470_NOT_BUSY;
                RIM_Motors[cur_motor_id].received_cmd = CMD_QUEUED;
                break;
            case RIM_OP_MOTOR_STOP:
                break;
            case RIM_OP_MOTOR_STATUS:
                RIM_Motors[cur_motor_id].is_busy = L6470_NOT_BUSY;
                RIM_Motors[cur_motor_id].received_cmd = CMD_QUEUED;
                break;
            case RIM_OP_MOTOR_GETSET_PARAM:
            	RIM_Motors[cur_motor_id].steps |= ((uint16)received_uart_char << 8);
                RIM_Motors[cur_motor_id].is_busy = L6470_NOT_BUSY;
                RIM_Motors[cur_motor_id].received_cmd = CMD_QUEUED;
            	break;
            case RIM_OP_ENCODER_INFO:
                RIM_Encoders[cur_motor_id].is_busy = L6470_NOT_BUSY;
                RIM_Encoders[cur_motor_id].received_cmd = CMD_QUEUED;
                break;
        }

        cmd_bytes[2] = received_uart_char;
        Cy_SCB_ClearRxInterrupt(UARTD_HW, CY_SCB_RX_INTR_NOT_EMPTY);
        cur_bit_field = 0;
        //UARTD_ClearRxFifo();
    }

}



void SPI_ISR(void)
{
    Cy_SCB_SPI_Interrupt(SPI_HW, &SPI_context);
}

int main(void)
{
    /* Set up the device based on configurator selections */
    init_cycfg_all();

    int i = 0;

    for(i = 0; i < 7; i++){
        RIM_Motors[i].is_busy = 0;
        RIM_Motors[i].received_cmd = CMD_NONE;
        RIM_Motors[i].steps = 0;
    }

    //Assign enable ids
    RIM_Motors[0].enable_id = RIM_M0_ENABLE;
    RIM_Motors[1].enable_id = RIM_M1_ENABLE;
    RIM_Motors[2].enable_id = RIM_M2_ENABLE;
    RIM_Motors[3].enable_id = RIM_M3_ENABLE;
    RIM_Motors[4].enable_id = RIM_M4_ENABLE;

    RIM_Encoders[0].enable_id = RIM_E0_ENABLE;
    RIM_Encoders[1].enable_id = RIM_E1_ENABLE;
    RIM_Encoders[2].enable_id = RIM_E2_ENABLE;
    RIM_Encoders[3].enable_id = RIM_E3_ENABLE;
    RIM_Encoders[4].enable_id = RIM_E4_ENABLE;

    //Reset motor drivers for setup
    Cy_GPIO_Write(rst_0_PORT, rst_0_NUM, 0);
    Cy_GPIO_Write(rst_0_PORT, rst_0_NUM, 1);

    Cy_GPIO_Write(rst_1_PORT, rst_1_NUM, 0);
    Cy_GPIO_Write(rst_1_PORT, rst_1_NUM, 1);

    Cy_GPIO_Write(rst_2_PORT, rst_2_NUM, 0);
    Cy_GPIO_Write(rst_2_PORT, rst_2_NUM, 1);

    Cy_GPIO_Write(rst_3_PORT, rst_3_NUM, 0);
    Cy_GPIO_Write(rst_3_PORT, rst_3_NUM, 1);

    Cy_GPIO_Write(rst_4_PORT, rst_4_NUM, 0);
    Cy_GPIO_Write(rst_4_PORT, rst_4_NUM, 1);

    uint16 RIM_UI_cmd_temp = 0;
    byte cmd_content[2] = {0, 0};

    //-------------------------------------------------
    //Communication Enables
    //-------------------------------------------------
    //UART
    Cy_SCB_UART_Init(UARTD_HW, &UARTD_config, &UARTD_context);
    Cy_SysInt_Init(&UARTD_intr_cfg, &Interrupt_Handler_UART);
    NVIC_EnableIRQ(UARTD_intr_cfg.intrSrc);
    Cy_SCB_UART_Enable(UARTD_HW);

    //SPI
    Cy_SCB_SPI_Init(SPI_HW, &SPI_config, &SPI_context);
    Cy_SysInt_Init(&SPI_intr_cfg, &SPI_ISR);
    NVIC_EnableIRQ(SPI_intr_cfg.intrSrc);
    Cy_SCB_SPI_Enable(SPI_HW);
    //-------------------------------------------------

    __enable_irq();


    CyDelay(1000);
    while(1){
    	seeval = CUI_read(RIM_Encoders[0].enable_id);
    	seeval = CUI_read(RIM_Encoders[1].enable_id);
    	seeval = CUI_read(RIM_Encoders[2].enable_id);
    	seeval = CUI_read(RIM_Encoders[3].enable_id);
    	seeval = CUI_read(RIM_Encoders[4].enable_id);
    	CyDelay(1);
    }


    //Motor Driver Configurations

    seeval = get_param(RIM_CONFIG, RIM_Motors[0].enable_id);
    set_param(STEP_MODE, (!SYNC_EN) | STEP_SEL_1_2 | SYNC_SEL_1, RIM_Motors[0].enable_id);
    set_param(MAX_SPEED, max_speed_calc(500), RIM_Motors[0].enable_id);
    set_param(FS_SPD, fs_calc(0x3FF), RIM_Motors[0].enable_id);
    set_param(ACC, acc_calc(100), RIM_Motors[0].enable_id);
    set_param(DECEL, dec_calc(100), RIM_Motors[0].enable_id);
    set_param(OCD_TH, OCD_TH_6000mA, RIM_Motors[0].enable_id);
    set_param(RIM_CONFIG, CONFIG_PWM_DIV_1 | CONFIG_PWM_MUL_2 | CONFIG_SR_530V_us | CONFIG_OC_SD_ENABLE | CONFIG_VS_COMP_DISABLE | CONFIG_SW_HARD_STOP | CONFIG_INT_16MHZ, RIM_Motors[0].enable_id);
    set_param(KVAL_RUN, 0xFF, RIM_Motors[0].enable_id);

    seeval = get_param(RIM_CONFIG, RIM_Motors[1].enable_id);
    set_param(STEP_MODE, (!SYNC_EN) | STEP_SEL_1_4 | SYNC_SEL_1, RIM_Motors[1].enable_id);
    set_param(MAX_SPEED, max_speed_calc(500), RIM_Motors[1].enable_id);
    set_param(FS_SPD, fs_calc(0x3FF), RIM_Motors[1].enable_id);
    set_param(ACC, acc_calc(35), RIM_Motors[1].enable_id);
    set_param(DECEL, dec_calc(100), RIM_Motors[1].enable_id);
    set_param(OCD_TH, OCD_TH_6000mA, RIM_Motors[1].enable_id);
    set_param(RIM_CONFIG, CONFIG_PWM_DIV_1 | CONFIG_PWM_MUL_2 | CONFIG_SR_530V_us | CONFIG_OC_SD_ENABLE | CONFIG_VS_COMP_DISABLE | CONFIG_SW_HARD_STOP | CONFIG_INT_16MHZ, RIM_Motors[1].enable_id);
    set_param(KVAL_RUN, 0x1B, RIM_Motors[1].enable_id);
    set_param(KVAL_ACC, 0x1B, RIM_Motors[1].enable_id);
    set_param(KVAL_DEC, 0x1B, RIM_Motors[1].enable_id);

    seeval = get_param(RIM_CONFIG, RIM_Motors[2].enable_id);
    set_param(STEP_MODE, !SYNC_EN | STEP_SEL_1_2 | SYNC_SEL_1 , RIM_Motors[2].enable_id);
    set_param(MAX_SPEED, max_speed_calc(625), RIM_Motors[2].enable_id);
    set_param(FS_SPD,  fs_calc(0x3FF), RIM_Motors[2].enable_id);
    set_param(ACC, acc_calc(125), RIM_Motors[2].enable_id);
    set_param(DECEL, dec_calc(125), RIM_Motors[2].enable_id);
    set_param(OCD_TH, OCD_TH_6000mA, RIM_Motors[2].enable_id);
    set_param(RIM_CONFIG, CONFIG_PWM_DIV_1 | CONFIG_PWM_MUL_2 | CONFIG_SR_530V_us | CONFIG_OC_SD_ENABLE | CONFIG_VS_COMP_DISABLE | CONFIG_SW_HARD_STOP | CONFIG_INT_16MHZ, RIM_Motors[2].enable_id);
    set_param(KVAL_RUN, 0x1D, RIM_Motors[2].enable_id);
    set_param(KVAL_ACC, 0x1D, RIM_Motors[2].enable_id);
    set_param(KVAL_DEC, 0x1D, RIM_Motors[2].enable_id);

    seeval = get_param(RIM_CONFIG, RIM_Motors[3].enable_id);
    set_param(STEP_MODE, !SYNC_EN | STEP_SEL_1_2 | SYNC_SEL_1, RIM_Motors[3].enable_id);
    set_param(MAX_SPEED, max_speed_calc(500), RIM_Motors[3].enable_id);
    set_param(FS_SPD,  fs_calc(0x3FF), RIM_Motors[3].enable_id);
    set_param(ACC, acc_calc(100), RIM_Motors[3].enable_id);
    set_param(DECEL, dec_calc(100), RIM_Motors[3].enable_id);
    set_param(OCD_TH, OCD_TH_6000mA, RIM_Motors[3].enable_id);
    set_param(RIM_CONFIG, CONFIG_PWM_DIV_1 | CONFIG_PWM_MUL_2 | CONFIG_SR_530V_us | CONFIG_OC_SD_ENABLE | CONFIG_VS_COMP_DISABLE | CONFIG_SW_HARD_STOP | CONFIG_INT_16MHZ, RIM_Motors[3].enable_id);
    set_param(KVAL_RUN, 0x42, RIM_Motors[3].enable_id);
    set_param(KVAL_ACC, 0x42, RIM_Motors[3].enable_id);
    set_param(KVAL_DEC, 0x42, RIM_Motors[3].enable_id);

    seeval = get_param(RIM_CONFIG, RIM_Motors[4].enable_id);
    set_param(STEP_MODE, (!SYNC_EN) | STEP_SEL_1_4 | SYNC_SEL_1, RIM_Motors[4].enable_id);
    set_param(MAX_SPEED, max_speed_calc(500), RIM_Motors[4].enable_id);
    set_param(FS_SPD,  fs_calc(0x3FF), RIM_Motors[4].enable_id);
    set_param(ACC, acc_calc(100), RIM_Motors[4].enable_id);
    set_param(DECEL, dec_calc(100), RIM_Motors[4].enable_id);
    set_param(OCD_TH, OCD_TH_1500mA, RIM_Motors[4].enable_id);
    set_param(RIM_CONFIG, CONFIG_PWM_DIV_1 | CONFIG_PWM_MUL_2 | CONFIG_SR_530V_us | CONFIG_OC_SD_ENABLE | CONFIG_VS_COMP_DISABLE | CONFIG_SW_HARD_STOP | CONFIG_INT_16MHZ, RIM_Motors[4].enable_id);
    set_param(KVAL_RUN, 0x0B, RIM_Motors[4].enable_id);
    set_param(KVAL_ACC, 0x0B, RIM_Motors[4].enable_id);
    set_param(KVAL_DEC, 0x0B, RIM_Motors[4].enable_id);

    for(;;)
    {

    	        for(i = 0; i < CURRENTLY_CONNECTED_MOTORS; i++) {

    	            if(RIM_Motors[i].received_cmd == CMD_NONE)
    	            {
    	                continue;
    	            }
    	            //seeval = RIM_Motors[0].is_busy = Cy_GPIO_Read(busy_0_0_PORT, busy_0_0_NUM);
    	            RIM_Motors[0].is_busy = Cy_GPIO_Read(busy_0_PORT, busy_0_NUM) ^ 0x01;
    	            RIM_Motors[1].is_busy = Cy_GPIO_Read(busy_1_PORT, busy_1_NUM) ^ 0x01;
    	            RIM_Motors[2].is_busy = Cy_GPIO_Read(busy_2_PORT, busy_2_NUM) ^ 0x01;
    	            RIM_Motors[3].is_busy = Cy_GPIO_Read(busy_3_PORT, busy_3_NUM) ^ 0x01;
    	            RIM_Motors[4].is_busy = Cy_GPIO_Read(busy_4_PORT, busy_4_NUM) ^ 0x01;

    	            switch(RIM_Motors[i].command_type) {
    	                //If received command is a motor run command
    	                case RIM_OP_MOTOR_RUN:
    	                    if(!RIM_Motors[i].is_busy && RIM_Motors[i].received_cmd == CMD_RUNNING)
    	                    {
    	                        transfer(SOFT_STOP, RIM_Motors[i].enable_id);
    	                        while(check_busy(i));


    	                        RIM_Motors[i].is_busy = L6470_NOT_BUSY;
    	                        RIM_Motors[i].received_cmd = CMD_NONE;
    	                        RIM_Motors[i].steps = 0;

    	                        // --mod from PSoC4--
    	                        Cy_SCB_UART_Put(UARTD_HW, RIM_OP_MOTOR_STOP | i);
    	                    }
    	                    else if (RIM_Motors[i].received_cmd == CMD_QUEUED)
    	                    {
    	                        //Start motor movement
    	                        motor_move(RIM_Motors[i].motor_dir ^ 0x1, RIM_Motors[i].steps, RIM_Motors[i].enable_id);
    	                        RIM_Motors[i].received_cmd = CMD_RUNNING;
    	                        //One byte information that tells the PC that a motor 1 is running
    	                        // --mod from PSoC4--
    	                        Cy_SCB_UART_Put(UARTD_HW, RIM_OP_MOTOR_RUN | i);
    	                    }
    	                    break;
    	                case RIM_OP_MOTOR_STOP:
    	                	//If the ALL STOP has been issued:
    	                	if(RIM_Motors[i].steps == ALL_HSTOP || RIM_Motors[i].steps == ALL_SSTOP)
    	                	{
    	                		int j = 0;

    	                		for(j = 0; j < CURRENTLY_CONNECTED_MOTORS; j++)
    	                		{
        	                		if(RIM_Motors[i].steps == ALL_HSTOP)
        	                			transfer(HARD_STOP, RIM_Motors[j].enable_id);
        	                		else
        	                			transfer(SOFT_STOP, RIM_Motors[j].enable_id);
    	                		}

    	                		for(j = 0; j < CURRENTLY_CONNECTED_MOTORS; j++)
    	                		{
    	                			while(check_busy(j));
        	                		RIM_Motors[j].is_busy = L6470_NOT_BUSY;
        	                		RIM_Motors[j].received_cmd = CMD_NONE;
        	                		RIM_Motors[j].steps = 0;
        	                		Cy_SCB_UART_Put(UARTD_HW, RIM_OP_MOTOR_STOP | j);
    	                		}
    	                	}
    	                	else
    	                	{
    	                		if(RIM_Motors[i].steps == ONE_HSTOP)
    	                			transfer(HARD_STOP, RIM_Motors[i].enable_id);
    	                		else
    	                			transfer(SOFT_STOP, RIM_Motors[i].enable_id);

    	                		while(check_busy(i));
    	                		RIM_Motors[i].is_busy = L6470_NOT_BUSY;
    	                		RIM_Motors[i].received_cmd = CMD_NONE;
    	                		RIM_Motors[i].steps = 0;
    	                		Cy_SCB_UART_Put(UARTD_HW, RIM_OP_MOTOR_STOP | i);
    	                	}
    	                	break;

    	                //If command is asking for the motor status register
    	                case RIM_OP_MOTOR_STATUS:
    	                    if (RIM_Motors[i].received_cmd == CMD_QUEUED)
    	                    {
    	                        RIM_Motors[i].received_cmd = CMD_RUNNING;
    	                        //One byte information that tells the PC that a motor 1 is running
    	                        // --mod from PSoC4--
    	                        Cy_SCB_UART_Put(UARTD_HW, RIM_OP_MOTOR_STATUS | i);
    	                        RIM_UI_cmd_temp = get_status(RIM_Motors[i].enable_id);
    	                        cmd_content[0] = RIM_UI_cmd_temp;
    	                        cmd_content[1] = RIM_UI_cmd_temp >> 8;
    	                        // --mod from PSoC4--
    	                        Cy_SCB_UART_Put(UARTD_HW, cmd_content[0]);
    	                        // --mod from PSoC4--
    	                        Cy_SCB_UART_Put(UARTD_HW, cmd_content[1]);
    	                        RIM_Motors[i].received_cmd = CMD_NONE;
    	                    }
    	                    break;

    	                case RIM_OP_MOTOR_GETSET_PARAM:

    	                	if (RIM_Motors[i].received_cmd == CMD_QUEUED)
    	                    {
    	                        RIM_Motors[i].received_cmd = CMD_RUNNING;
								byte cmd_type         = RIM_Motors[i].steps & GETSET_RECIEVED_PARAM_TYPE;
    	                        uint16 cmd_params     = (RIM_Motors[i].steps & GETSET_RECIEVED_PARAM_DATA) >> 5;

    	                        if(RIM_Motors[i].motor_dir == GETSET_GET_PARAM)
    	                        {
    	                        	RIM_UI_cmd_temp = get_param(cmd_type, RIM_Motors[i].enable_id);

    	                        	cmd_content[0] = (RIM_UI_cmd_temp & 0x07) << 5;
    	                        	cmd_content[0] |= cmd_type;
        	                        cmd_content[1] = RIM_UI_cmd_temp >> 3;

        	                        Cy_SCB_UART_Put(UARTD_HW, RIM_OP_MOTOR_GETSET_PARAM | GETSET_GET_PARAM | i);
        	                        Cy_SCB_UART_Put(UARTD_HW, cmd_content[0]);
        	                        Cy_SCB_UART_Put(UARTD_HW, cmd_content[1]);

    	                        }
    	                        else if(RIM_Motors[i].motor_dir == GETSET_SET_PARAM)
    	                        {
    	                        	uint temp = 0;
    	                        	switch(cmd_type)
    	                        	{
    	                        		case ACC:
    	                        			temp = acc_calc(cmd_params);
    	                        			set_param(cmd_type, temp, RIM_Motors[i].enable_id);
    	                        			break;
    	                        		case DECEL:
    	                        			temp = dec_calc(cmd_params);
    	                        			set_param(cmd_type, temp, RIM_Motors[i].enable_id);
    	                        			break;
    	                        		case MAX_SPEED:
    	                        			temp = max_speed_calc(cmd_params);
											set_param(cmd_type, temp, RIM_Motors[i].enable_id);
    	                        			break;
    	                        		case STEP_MODE:
    	                        			set_param(cmd_type, (!SYNC_EN) | cmd_params | SYNC_SEL_1 , RIM_Motors[i].enable_id);
    	                        			break;
    	                        		default:
    	                        			set_param(cmd_type, cmd_params, RIM_Motors[i].enable_id);
    	                        			break;
    	                        	}
    	                        	Cy_SCB_UART_Put(UARTD_HW, RIM_OP_MOTOR_GETSET_PARAM | (GETSET_SET_PARAM << 3) | i);
    	                        	Cy_SCB_UART_Put(UARTD_HW, (((temp << 6) | cmd_type) & 0x00FF));
    	                        	Cy_SCB_UART_Put(UARTD_HW,  ((temp << 6) | cmd_type) >> 8);
    	                        }
	                        	RIM_Motors[i].is_busy = L6470_NOT_BUSY;
    	                		RIM_Motors[i].received_cmd = CMD_NONE;
    	                		RIM_Motors[i].steps = 0;
    	                    }

    	                	break;
    	                default:
    	                    break;
    	            }

    	        }
    	        for(i = 0; i < CURRENTLY_CONNECTED_ENCODERS; i++) {
    	        	if(RIM_Encoders[i].received_cmd == CMD_NONE)
    	        		continue;
    	        	switch(RIM_Encoders[i].command_type)
    	        	{
    	        	case RIM_OP_ENCODER_INFO:
    	        		if(RIM_Encoders[i].received_cmd == CMD_QUEUED)
    	        		{
    	        			 RIM_Encoders[i].received_cmd = CMD_RUNNING;
    	        			 Cy_SCB_UART_Put(UARTD_HW, RIM_OP_ENCODER_INFO | i);
    	        			 RIM_UI_cmd_temp = CUI_read(RIM_Encoders[i].enable_id);
    	        			 cmd_content[0] = RIM_UI_cmd_temp;
    	        			 cmd_content[1] = RIM_UI_cmd_temp >> 8;
    	        			 Cy_SCB_UART_Put(UARTD_HW, cmd_content[0]);
    	        			 Cy_SCB_UART_Put(UARTD_HW, cmd_content[1]);
    	        		 	 RIM_Encoders[i].received_cmd = CMD_NONE;
    	        		}
    	        		break;

    	        	}
    	        }


    }
}


uint8 check_busy(byte dev_id)
{
    uint8 r = 0;

    switch(dev_id)
    {
        case 0:
            RIM_Motors[0].is_busy = Cy_GPIO_Read(busy_0_PORT, busy_0_NUM) ^ 0x01;
            r = RIM_Motors[0].is_busy;
            break;
        case 1:
            RIM_Motors[1].is_busy = Cy_GPIO_Read(busy_1_PORT, busy_1_NUM) ^ 0x01;
            r = RIM_Motors[1].is_busy;
            break;
        case 2:
            RIM_Motors[2].is_busy = Cy_GPIO_Read(busy_2_PORT, busy_2_NUM) ^ 0x01;
            r = RIM_Motors[2].is_busy;
            break;
        case 3:
            RIM_Motors[3].is_busy = Cy_GPIO_Read(busy_3_PORT, busy_3_NUM) ^ 0x01;
            r = RIM_Motors[3].is_busy;
            break;
        case 4:
            RIM_Motors[4].is_busy = Cy_GPIO_Read(busy_4_PORT, busy_4_NUM) ^ 0x01;
            r = RIM_Motors[4].is_busy;
    }
    return r;
}
