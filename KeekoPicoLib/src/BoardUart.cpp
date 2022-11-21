#include "BoardUart.h"

namespace Keeko
{

BoardUart::BoardUart(uart_inst_t* uart, uint32_t baud, uint8_t tx_pin, uint8_t rx_pin): Uart(baud)
{
    uart_ptr = uart;
    this->tx_pin = tx_pin;
    this->rx_pin = rx_pin;
    uart_init(uart, baud);
    gpio_set_function(tx_pin, GPIO_FUNC_UART);
    gpio_set_function(rx_pin, GPIO_FUNC_UART);
}

BoardUart::~BoardUart()
{
    uart_deinit(uart_ptr);
    gpio_deinit(tx_pin);
    gpio_deinit(rx_pin);
}

bool BoardUart::isReadable()
{
    return uart_is_readable(uart_ptr);
}

bool BoardUart::isWritable()
{
    return uart_is_writable(uart_ptr);
}

void BoardUart::write(const std::shared_ptr<uint8_t[]> data, size_t size)
{
    //TODOJ: Check is is writable
    uart_write_blocking(uart_ptr, data.get(), size);
}

std::tuple<std::unique_ptr<uint8_t[]>, size_t> BoardUart::read()
{
    std::unique_ptr<uint8_t[]> data{new uint8_t[MAX_READ_SIZE]};
    size_t read_amount{0};
    while(read_amount <= MAX_READ_SIZE && isReadable())
    {
        data[read_amount] = *reinterpret_cast<uint8_t*>(uart_getc(uart_ptr));
        read_amount++;
    }
    return std::make_tuple(std::move(data), read_amount);
}
}
