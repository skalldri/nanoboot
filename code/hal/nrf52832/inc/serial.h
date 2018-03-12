#pragma once

#include <serial_base.h>
#include <nrf_uarte.h>

// TODO: move serial data buffer size into a config variable
#define TX_BUFFER_SIZE 64
#define RX_BUFFER_SIZE 64

// How many buffers in the serial RX ring. Min 3
#define NUM_RX_BUFFER 3

class Serial_impl : public Serial_base {

public:
    Serial_impl(uint32_t device);
    ~Serial_impl();

    void Read(char** data);
    void Write(char* data, uint32_t length);
    void WriteString(const char* data);
    void IRQ_Handler();

private:
    uint32_t deviceIndex;

    char TxBuffer[TX_BUFFER_SIZE];
    uint32_t TxBufferContents;
    char RxBuffer[NUM_RX_BUFFER][RX_BUFFER_SIZE];
    uint32_t RxBufferContents[NUM_RX_BUFFER];
    bool RxBufferInUse[NUM_RX_BUFFER];
    uint32_t RxNextBuffer;
    bool BufferOverlow;
};

typedef Serial_impl Serial;