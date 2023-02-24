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
    return tud_cdc_n_write_available(id) >= CFG_TUD_CDC_TX_BUFSIZE;//TODO: Correct
}

uint32_t USBUart::availableWrite()
{
    return tud_cdc_n_write_available(id);
}

void USBUart::write(const std::shared_ptr<uint8_t[]>& data, size_t size)
{
    //TODOJ: Check is is writable
    tud_cdc_n_write(id, data.get(), size);
    tud_cdc_n_write_flush(id);
}

void USBUart::write(const void* data, size_t size)
{
    tud_cdc_n_write(id, data, size);
    tud_cdc_n_write_flush(id);
}

void USBUart::write(const std::string& data)
{
    Uart::write(data);
}

std::tuple<std::unique_ptr<uint8_t[]>, size_t> USBUart::read()
{
    std::unique_ptr<uint8_t[]> data{new uint8_t[MAX_READ_SIZE]};
    size_t read_amount = tud_cdc_n_read(id, data.get(), MAX_READ_SIZE);
    return std::make_tuple(std::move(data), read_amount);
}
}
