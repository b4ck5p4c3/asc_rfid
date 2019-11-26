#include <port.h>
#include <mbport.h>
#include <periph.h>

extern UART_HandleTypeDef huart1;
uint8_t receive_buf[1];

uint32_t test_var = 0;

BOOL
xMBPortSerialInit(UCHAR ucPORT, ULONG ulBaudRate, UCHAR ucDataBits, eMBParity eParity) {

    return TRUE;
}

void
vMBPortSerialEnable(BOOL xRxEnable, BOOL xTxEnable) {
    if(xRxEnable) {
        HAL_UART_Receive_IT(&huart1, receive_buf, 1);
    }

    if (xRxEnable && !xTxEnable) {
        HAL_GPIO_WritePin(MODBUS_DE_PORT, MODBUS_DE_PIN, GPIO_PIN_RESET);
    }

    if (xTxEnable && !xRxEnable) {
        HAL_GPIO_WritePin(MODBUS_DE_PORT, MODBUS_DE_PIN, GPIO_PIN_SET);
        // HAL_Delay(1);
    }
}

BOOL
xMBPortSerialPutByte( CHAR ucByte ) {
    uint8_t buffer[] = {(uint8_t)ucByte};
    HAL_UART_Transmit_IT(&huart1, buffer, 1);

    return TRUE;
}

BOOL
xMBPortSerialGetByte( CHAR * pucByte ) {
    *pucByte = (CHAR) receive_buf[0];

    return TRUE;
}

void HAL_UART_RxCpltCallback(UART_HandleTypeDef* huart) {
    if (huart == &huart1) {
        // printf("%02X", receive_buf[0]);
        HAL_UART_Receive_IT(&huart1, receive_buf, 1);
        pxMBFrameCBByteReceived();
    }
}

void HAL_UART_ErrorCallback(UART_HandleTypeDef* huart) {

}


void HAL_UART_TxCpltCallback(UART_HandleTypeDef* huart) {
    if (huart == &huart1) {
        pxMBFrameCBTransmitterEmpty();
    }
}
