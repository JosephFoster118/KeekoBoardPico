#include "USBUart.h"

namespace Keeko
{

USBUart::USBUart(uart_inst_t* uart, uint32_t baud, uint8_t tx_pin, uint8_t rx_pin): Uart{DEFAULT_USB_UART_BAUD}
{
    this->id = id;
}

USBUart::~USBUart()
{
}

bool USBUart::isReadable()
{
    return uart_is_readable(uart_ptr);
}

bool USBUart::isWritable()
{
    return uart_is_writable(uart_ptr);
}

void USBUart::write(const std::shared_ptr<uint8_t[]> data, size_t size)
{
    //TODOJ: Check is is writable
    uart_write_blocking(uart_ptr, data.get(), size);
}

std::tuple<std::unique_ptr<uint8_t[]>, size_t> USBUart::read()
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
