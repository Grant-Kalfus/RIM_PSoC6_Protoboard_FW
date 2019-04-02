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
    RIM_enableWrite(enable_id, 0);
    bufferTx[0] = command;
    Cy_SCB_SPI_Transfer(SPI_HW, bufferTx, bufferRx, 1, &SPI_context);
    while (0UL != (CY_SCB_SPI_TRANSFER_ACTIVE & Cy_SCB_SPI_GetTransferStatus(SPI_HW, &SPI_context)));
    CyDelayUs(20);
    RIM_enableWrite(enable_id, 1);
}

uint16 CUI_read (uint8 enable_id)
{
    uint8_t upper=0,
            lower=0;
    uint16 read_val;
    int i = 0;

    CUI_transfer(CUI_READ_POS, enable_id);

    while(1)
    {
        if(bufferRx[0] == CUI_READ_POS) {break;}

        //if(i < BUFFER_SIZE - 1) {break;}

        wait_for_response();
    }
    upper = wait_for_response(enable_id);
    lower = wait_for_response(enable_id);
    read_val = upper;
    read_val = read_val<<8;
    read_val |= lower;
    return read_val;
}


uint8_t wait_for_response(int enable_id) {

    RIM_enableWrite(enable_id, 0);
    bufferTx[0] = CUI_NOP;
    Cy_SCB_SPI_Transfer(SPI_HW, bufferTx, bufferRx, BUFFER_SIZE, &SPI_context);
    while (0UL != (CY_SCB_SPI_TRANSFER_ACTIVE & Cy_SCB_SPI_GetTransferStatus(SPI_HW, &SPI_context)));
    CyDelayUs(20);
    RIM_enableWrite(enable_id, 1);

    return bufferRx[0];
}

/*
void RIM_enableWrite(uint8 enable_id, uint8 state)
{
    switch(enable_id)
    {
        case 0:
            Cy_GPIO_Write(e0_enable_PORT, e0_enable_NUM, state);
        break;
        /*
        case 1:
            Cy_GPIO_Write(SS02_0_PORT, SS02_0_NUM, state);
        break;

        case 2:
            Cy_GPIO_Write(SS03_0_PORT, SS03_0_NUM, state);
        break;

        case 3:
            Cy_GPIO_Write(SS04_0_PORT, SS04_0_NUM, state);
        break;

        case 4:
            Cy_GPIO_Write(SS04_0_PORT, SS04_0_NUM, state);
        break;

        case 5:
            Cy_GPIO_Write(SS04_0_PORT, SS04_0_NUM, state);
        break;

        case 6:
            Cy_GPIO_Write(SS04_0_PORT, SS04_0_NUM, state);
        break;

        case 7:
            Cy_GPIO_Write(SS04_0_PORT, SS04_0_NUM, state);
        break;

        case 8:
            Cy_GPIO_Write(SS04_0_PORT, SS04_0_NUM, state);
        break;

        case 1:
            Cy_GPIO_Write(SS01_0_PORT, SS01_0_NUM, state);
        break;
           */
//    }
//}


/*
uint16 CUI_get_position(uint8 enable_id)
{
    uint16 result = 0;

    CUI_transfer(CUI_READ_POS, enable_id);

    while(1)
    {
        if(wait_for_response(enable_id) == CUI_READ_POS) {break;}
        CyDelayUs(20);
    }

    //Read 8 upper bytes
    result = CUI_read(enable_id);
    result <<= 8;

    //OR with 8 lower bytes
    result |= CUI_read(enable_id);

    return result;
}
*/
