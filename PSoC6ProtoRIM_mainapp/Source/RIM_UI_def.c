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
	enable_translator(0, enable_id);
    Cy_SCB_SPI_Transfer(SPI_HW, bufferTx, bufferRx, 1, &SPI_context);
    while (0UL != (CY_SCB_SPI_TRANSFER_ACTIVE & Cy_SCB_SPI_GetTransferStatus(SPI_HW, &SPI_context)));
    CyDelayUs(40);
    enable_translator(1, enable_id);
}

uint16 CUI_read (uint8 enable_id)
{
    uint8_t upper=0,
            lower=0;
    uint16 read_val;
    int timeout = 200;

    CUI_transfer(CUI_READ_POS, enable_id);
    CyDelayUs(20);
    while(1)
    {
        if(bufferRx[0] == CUI_READ_POS) {break;}
        wait_for_response(enable_id);
        CyDelay(1);
        timeout--;

        if(timeout == 0)
        {
        	return 0xFFFF;
        }
    }
    upper = wait_for_response(enable_id);
    CyDelayUs(40);
    lower = wait_for_response(enable_id);
    CyDelayUs(40);
    read_val = upper;
    read_val = read_val<<8;
    read_val |= lower;
    return read_val;
}


uint8_t wait_for_response(int enable_id) {

    bufferTx[0] = CUI_NOP;
	enable_translator(0, enable_id);
    Cy_SCB_SPI_Transfer(SPI_HW, bufferTx, bufferRx, BUFFER_SIZE, &SPI_context);
    while (0UL != (CY_SCB_SPI_TRANSFER_ACTIVE & Cy_SCB_SPI_GetTransferStatus(SPI_HW, &SPI_context)));
    CyDelayUs(40);
    enable_translator(1, enable_id);
    return bufferRx[0];
}
