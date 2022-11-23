#include "Uart.h"
#include "tusb.h"

namespace Keeko
{

class USBUart: public Uart
{
public:
    USBUart() = delete;
    USBUart(uint8_t id);
    ~USBUart();

    static constexpr size_t MAX_READ_SIZE{32};
    static constexpr uint32_t DEFAULT_USB_UART_BAUD{921600};

    virtual bool isReadable();
    virtual bool isWritable();
    virtual void write(const std::shared_ptr<uint8_t[]> data, size_t size) ;
    virtual std::tuple<std::unique_ptr<uint8_t[]>, size_t> read();

protected:

    uint8_t id;

};

