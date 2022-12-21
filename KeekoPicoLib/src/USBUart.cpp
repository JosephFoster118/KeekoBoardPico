#include "Keeko/USBUart.h"

namespace Keeko
{

USBUart::USBUart(uint8_t id): Uart{DEFAULT_USB_UART_BAUD}
{
    this->id = id;
}

USBUart::~USBUart()
{
}

bool USBUart::isReadable()
{
    return tud_cdc_n_available(id);
}

bool USBUart::isWritable()
{
    return tud_cdc_n_write_available(id);
}

void USBUart::write(const std::shared_ptr<uint8_t[]> data, size_t size)
{
    //TODOJ: Check is is writable
    tud_cdc_n_write(id, data.get(), size);
}

std::tuple<std::unique_ptr<uint8_t[]>, size_t> USBUart::read()
{
    std::unique_ptr<uint8_t[]> data{new uint8_t[MAX_READ_SIZE]};
    size_t read_amount = tud_cdc_n_read(id, data.get(), MAX_READ_SIZE);
    return std::make_tuple(std::move(data), read_amount);
}
}
