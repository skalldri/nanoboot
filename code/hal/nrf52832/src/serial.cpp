#include <serial.h>
#include <string.h>
#include <nrf52.h>

#define UART_RTS_PIN 5
#define UART_TXD_PIN 6
#define UART_CTS_PIN 7
#define UART_RXD_PIN 8

#define UART_PTR NRF_UARTE0

// TODO: 
#define NUM_UART_HW_BLOCKS 1
static Serial* irqReference[NUM_UART_HW_BLOCKS];

extern "C"
{
    void __attribute__((used)) UARTE0_UART0_IRQHandler()
    {
        if (irqReference[0] != nullptr)
        {
            irqReference[0]->IRQ_Handler();
        }
    }
}

void Serial_impl::IRQ_Handler()
{
    if (nrf_uarte_event_check(UART_PTR, NRF_UARTE_EVENT_ENDRX))
    {
        // The contents of this->RxBuffer[RxNextBuffer - 2] contain the most recent read, and
        // this->RxBuffer[RxNextBuffer - 1] has been automatically swapped in

        int mostRecentlyFilled = (RxNextBuffer - 2) % NUM_RX_BUFFER;
        int currentlyInUse = (RxNextBuffer - 1) % NUM_RX_BUFFER;

        // If the buffer we're about to swap in has not been emptied, Buffer Overflow
        if (RxBufferInUse[RxNextBuffer])
        {

        }
        
        nrf_uarte_rx_buffer_set(UART_PTR, (uint8_t*)this->RxBuffer[RxNextBuffer], RX_BUFFER_SIZE);


    }
    else
    {
        // TODO: crash dump
    }
}

// Constructor should initialize implementation specific details
Serial_impl::Serial_impl(uint32_t device)
{
    if (deviceIndex > NUM_UART_HW_BLOCKS)
    {
        // TODO: crash dump
    }

    deviceIndex = 0;
    irqReference[deviceIndex] = this;

    // Prepare the serial ring buffers
    for(int i = 0; i < NUM_RX_BUFFER; i++)
    {
        RxBufferInUse[i] = false;
        RxBufferContents[i] = 0;
    }

    nrf_uarte_txrx_pins_set(UART_PTR, UART_TXD_PIN, UART_RXD_PIN);
    nrf_uarte_hwfc_pins_set(UART_PTR, UART_RTS_PIN, UART_CTS_PIN);

    // TODO: hardware flow control isn't working, not sure why
    nrf_uarte_configure(UART_PTR, NRF_UARTE_PARITY_EXCLUDED, NRF_UARTE_HWFC_DISABLED);

    nrf_uarte_baudrate_set(UART_PTR, NRF_UARTE_BAUDRATE_115200);

    // Enable the ENDRX interrupt
    nrf_uarte_int_enable(UART_PTR, NRF_UARTE_INT_ENDRX_MASK | NRF_UARTE_INT_ERROR_MASK);
    NVIC_ClearPendingIRQ(UARTE0_UART0_IRQn);
    NVIC_SetPriority(UARTE0_UART0_IRQn, 3);
    NVIC_EnableIRQ(UARTE0_UART0_IRQn);

    nrf_uarte_enable(UART_PTR);

    // Start with Buffer 0 as RX active
    nrf_uarte_rx_buffer_set(UART_PTR, (uint8_t*)this->RxBuffer[0], RX_BUFFER_SIZE);
    RxBufferInUse[0] = true;

    // Pend a read on the RX task to kickstart the DMA
    nrf_uarte_task_trigger(UART_PTR, NRF_UARTE_TASK_STARTRX);

    // Setup the next buffer to take over RX duties once we fill this one (the UARTE PTR is double-buffered)
    nrf_uarte_rx_buffer_set(UART_PTR, (uint8_t*)this->RxBuffer[1], RX_BUFFER_SIZE);
    RxBufferInUse[1] = true;

    // Prep the system to use the buffer at index 2 when we need to swap out the next buffer
    RxNextBuffer = 2;
    BufferOverlow = false;
}

Serial_impl::~Serial_impl()
{
    irqReference[deviceIndex] = nullptr;
}

void Serial_impl::Read(char** data)
{
    nrf_uarte_rx_buffer_set(UART_PTR, (uint8_t*)this->RxBuffer, RX_BUFFER_SIZE);
}

void Serial_impl::Write(char* data, uint32_t length)
{
    // TODO: return an error
    if (length > TX_BUFFER_SIZE)
    {
        return;
    }

    memcpy(this->TxBuffer, data, length);
    nrf_uarte_tx_buffer_set(UART_PTR, (uint8_t*)this->TxBuffer, length);
    nrf_uarte_task_trigger(UART_PTR, NRF_UARTE_TASK_STARTTX);
}

void Serial_impl::WriteString(const char* data)
{
    // Add a byte for the trailing \0
    uint32_t length = (strlen(data) + 1);

    // TODO: return an error
    if (length > TX_BUFFER_SIZE)
    {
        return;
    }

    // Copy data into the TX buffer, ensuring that the last character is a null terminator
    memcpy(this->TxBuffer, data, length);
    this->TxBuffer[length - 1] = '\0';

    nrf_uarte_tx_buffer_set(UART_PTR, (uint8_t*)this->TxBuffer, length);
    nrf_uarte_task_trigger(UART_PTR, NRF_UARTE_TASK_STARTTX);
}