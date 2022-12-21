#pragma once

#include "Uart.h"
#include "pico/stdlib.h"
#include "hardware/uart.h"

namespace Keeko
{

class BoardUart: public Uart
{
public:
    BoardUart() = delete;
    BoardUart(uart_inst_t* uart, uint32_t baud, uint8_t tx_pin, uint8_t rx_pin);
    ~BoardUart();

    static constexpr size_t MAX_READ_SIZE{16};

    virtual bool isReadable();
    virtual bool isWritable();
    virtual void write(const std::shared_ptr<uint8_t[]> data, size_t size) ;
    virtual std::tuple<std::unique_ptr<uint8_t[]>, size_t> read();

protected:

    uart_inst_t* uart_ptr;
    uint8_t tx_pin;
    uint8_t rx_pin;

};

}
