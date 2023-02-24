#include "Keeko/Uart.h"

namespace Keeko
{

Uart::Uart(uint32_t baud)
{
    this->baud = baud;
}

void Uart::write(const std::string& data)
{
    write(reinterpret_cast<const void*>(data.c_str()), data.length());
}

}
