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
#include "RIM_UI_def.h"

#define BUFFER_SIZE 1
uint8_t bufferTx[BUFFER_SIZE];
uint8_t bufferRx[BUFFER_SIZE];
//uint16 read_pos();
uint8_t wait_for_response();

void CUI_transfer(uint8 command, uint8 enable_id)
{
    bufferTx[0] = command;
    Encoder_enable_translator(0, enable_id);
    Cy_SCB_SPI_Transfer(SPI_Encoder_HW, bufferTx, bufferRx, 1, &SPI_Encoder_context);
    while (0UL != (CY_SCB_SPI_TRANSFER_ACTIVE & Cy_SCB_SPI_GetTransferStatus(SPI_Encoder_HW, &SPI_Encoder_context)));
    CyDelayUs(20);
    Encoder_enable_translator(1, enable_id);
}

uint16 CUI_read (uint8 enable_id)
{
    uint8_t upper=0,
            lower=0;
    uint16 read_val;

    CUI_transfer(CUI_READ_POS, enable_id);
    CyDelayUs(20);
    while(1)
    {
        if(bufferRx[0] == CUI_READ_POS) {break;}
        wait_for_response(enable_id);
        CyDelayUs(20);
    }
    upper = wait_for_response(enable_id);
    CyDelayUs(20);
    lower = wait_for_response(enable_id);
    read_val = upper;
    read_val = read_val<<8;
    read_val |= lower;
    return read_val;
}


uint8_t wait_for_response(int enable_id) {

    bufferTx[0] = CUI_NOP;
    Encoder_enable_translator(0, enable_id);
    Cy_SCB_SPI_Transfer(SPI_Encoder_HW, bufferTx, bufferRx, BUFFER_SIZE, &SPI_Encoder_context);
    while (0UL != (CY_SCB_SPI_TRANSFER_ACTIVE & Cy_SCB_SPI_GetTransferStatus(SPI_Encoder_HW, &SPI_Encoder_context)));
    CyDelayUs(20);
    Encoder_enable_translator(1, enable_id);
    return bufferRx[0];
}


void Encoder_enable_translator(uint8 state, uint8 enable_id) {
	switch(enable_id)
	{
        case 0:
        	Cy_GPIO_Write(e0_enable_PORT, e0_enable_NUM, state);
        	break;
        case 1:
        	Cy_GPIO_Write(e1_enable_PORT, e1_enable_NUM, state);
        	break;
        case 2:
        	Cy_GPIO_Write(e2_enable_PORT, e2_enable_NUM, state);
        	break;
        case 3:
        	Cy_GPIO_Write(e3_enable_PORT, e3_enable_NUM, state);
        	break;
        case 4:
        	Cy_GPIO_Write(e4_enable_PORT, e4_enable_NUM, state);
        	break;
	}
}
