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
#define CURRENTLY_CONNECTED_MOTORS   4
#define CURRENTLY_CONNECTED_ENCODERS 0

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
uint16 seeval;
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
                if(RIM_Motors[cur_motor_id].is_busy)
                    cur_bit_field = -1;
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
                if(RIM_Motors[cur_motor_id].is_busy)
                    cur_bit_field = -1;
                else
                    RIM_Motors[cur_motor_id].command_type = RIM_OP_MOTOR_STATUS;
                break;

            case RIM_OP_ENCODER_INFO:
                cur_motor_id = received_uart_char & RIM_MOTOR_ID;
                if(RIM_Encoders[cur_motor_id].is_busy)
                    cur_bit_field = -1;
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
            	RIM_Motors[cur_motor_id].steps |= received_uart_char;
            	break;
            case RIM_OP_MOTOR_STATUS:
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


    //Reset motor drivers for setup
    Cy_GPIO_Write(rst_0_PORT, rst_0_NUM, 0);
    Cy_GPIO_Write(rst_0_PORT, rst_0_NUM, 1);

    Cy_GPIO_Write(rst_1_PORT, rst_1_NUM, 0);
    Cy_GPIO_Write(rst_1_PORT, rst_1_NUM, 1);

    Cy_GPIO_Write(rst_2_PORT, rst_2_NUM, 0);
    Cy_GPIO_Write(rst_2_PORT, rst_2_NUM, 1);

    Cy_GPIO_Write(rst_3_PORT, rst_3_NUM, 0);
    Cy_GPIO_Write(rst_3_PORT, rst_3_NUM, 1);


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

    seeval = get_param(RIM_CONFIG, RIM_Motors[0].enable_id);
    set_param(STEP_MODE, !SYNC_EN | STEP_SEL_1_2 | SYNC_SEL_1, RIM_Motors[0].enable_id);
    set_param(MAX_SPEED, max_speed_calc(500), RIM_Motors[0].enable_id);
    set_param(FS_SPD, 0x3FF, RIM_Motors[0].enable_id);
    set_param(ACC, acc_calc(100), RIM_Motors[0].enable_id);
    set_param(DECEL, dec_calc(100), RIM_Motors[0].enable_id);
    set_param(OCD_TH, OCD_TH_6000mA, RIM_Motors[0].enable_id);
    set_param(RIM_CONFIG, CONFIG_PWM_DIV_1 | CONFIG_PWM_MUL_2 | CONFIG_SR_530V_us | CONFIG_OC_SD_ENABLE | CONFIG_VS_COMP_DISABLE | CONFIG_SW_HARD_STOP | CONFIG_INT_16MHZ, RIM_Motors[0].enable_id);
    set_param(KVAL_RUN, 0xFF, RIM_Motors[0].enable_id);
    seeval = get_param(RIM_CONFIG, RIM_Motors[0].enable_id);

    seeval = get_param(RIM_CONFIG, RIM_Motors[1].enable_id);
    set_param(STEP_MODE, !SYNC_EN | STEP_SEL_1_4 | SYNC_SEL_1, RIM_Motors[1].enable_id);
    set_param(MAX_SPEED, max_speed_calc(500), RIM_Motors[1].enable_id);
    set_param(FS_SPD, fs_calc(0x3FF), RIM_Motors[1].enable_id);
    set_param(ACC, acc_calc(35), RIM_Motors[1].enable_id);
    set_param(DECEL, dec_calc(100), RIM_Motors[1].enable_id);
    set_param(OCD_TH, OCD_TH_6000mA, RIM_Motors[1].enable_id);
    set_param(RIM_CONFIG, CONFIG_PWM_DIV_1 | CONFIG_PWM_MUL_2 | CONFIG_SR_530V_us | CONFIG_OC_SD_ENABLE | CONFIG_VS_COMP_DISABLE | CONFIG_SW_HARD_STOP | CONFIG_INT_16MHZ, RIM_Motors[1].enable_id);
    set_param(KVAL_RUN, 0x35, RIM_Motors[1].enable_id);
    set_param(KVAL_ACC, 0x35, RIM_Motors[1].enable_id);
    set_param(KVAL_DEC, 0x35, RIM_Motors[1].enable_id);

    seeval = get_param(RIM_CONFIG, RIM_Motors[2].enable_id);
    set_param(STEP_MODE, !SYNC_EN | STEP_SEL_1_2 | SYNC_SEL_1, RIM_Motors[2].enable_id);
    set_param(MAX_SPEED, max_speed_calc(625), RIM_Motors[2].enable_id);
    set_param(FS_SPD,  0x3FF, RIM_Motors[2].enable_id);
    set_param(ACC, acc_calc(125), RIM_Motors[2].enable_id);
    set_param(DECEL, dec_calc(125), RIM_Motors[2].enable_id);
    set_param(OCD_TH, OCD_TH_6000mA, RIM_Motors[2].enable_id);
    set_param(RIM_CONFIG, CONFIG_PWM_DIV_1 | CONFIG_PWM_MUL_2 | CONFIG_SR_530V_us | CONFIG_OC_SD_ENABLE | CONFIG_VS_COMP_DISABLE | CONFIG_SW_HARD_STOP | CONFIG_INT_16MHZ, RIM_Motors[2].enable_id);
    set_param(KVAL_RUN, 0x3A, RIM_Motors[2].enable_id);
    set_param(KVAL_ACC, 0x3A, RIM_Motors[2].enable_id);
    set_param(KVAL_DEC, 0x3A, RIM_Motors[2].enable_id);

    seeval = get_param(RIM_CONFIG, RIM_Motors[3].enable_id);
    set_param(STEP_MODE, !SYNC_EN | STEP_SEL_1_2 | SYNC_SEL_1, RIM_Motors[3].enable_id);
    set_param(MAX_SPEED, max_speed_calc(500), RIM_Motors[3].enable_id);
    set_param(FS_SPD,  0x3FF, RIM_Motors[3].enable_id);
    set_param(ACC, acc_calc(100), RIM_Motors[3].enable_id);
    set_param(DECEL, dec_calc(100), RIM_Motors[3].enable_id);
    set_param(OCD_TH, OCD_TH_6000mA, RIM_Motors[3].enable_id);
    set_param(RIM_CONFIG, CONFIG_PWM_DIV_1 | CONFIG_PWM_MUL_2 | CONFIG_SR_530V_us | CONFIG_OC_SD_ENABLE | CONFIG_VS_COMP_DISABLE | CONFIG_SW_HARD_STOP | CONFIG_INT_16MHZ, RIM_Motors[3].enable_id);
    set_param(KVAL_RUN, 0x84, RIM_Motors[3].enable_id);
    set_param(KVAL_ACC, 0x84, RIM_Motors[3].enable_id);
    set_param(KVAL_DEC, 0x84, RIM_Motors[3].enable_id);


    for(;;)
    {
    	 //RIM_UI_cmd_temp = CUI_read(RIM_E0_ENABLE);
    	       //RIM_UI_cmd_temp = CUI_get_position(RIM_E0_ENABLE);

    	        int i = 0;
    	        //Send update to PC that motor is currently running
	            RIM_Motors[0].is_busy = Cy_GPIO_Read(busy_0_PORT, busy_0_NUM) ^ 0x01;
	            RIM_Motors[1].is_busy = Cy_GPIO_Read(busy_1_PORT, busy_1_NUM) ^ 0x01;
	            RIM_Motors[2].is_busy = Cy_GPIO_Read(busy_2_PORT, busy_2_NUM) ^ 0x01;
	            RIM_Motors[3].is_busy = Cy_GPIO_Read(busy_3_PORT, busy_3_NUM) ^ 0x01;

    	        //RIM_Motors[0].is_busy = Cy_GPIO_ReadOut(busy_0_PORT, busy_0_NUM);

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
    	                default:
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
    }
    return r;
}
