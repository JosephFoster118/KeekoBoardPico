#pragma once

#include <memory>
#include <tuple>

namespace Keeko
{

class Uart
{
public:
    Uart() = delete;
    Uart(uint32_t baud);
    ~Uart(){};
    
    virtual bool isReadable() = 0;
    virtual bool isWritable() = 0;
    virtual void write(const std::shared_ptr<uint8_t[]>& data, size_t size) = 0;
    virtual std::tuple<std::unique_ptr<uint8_t[]>, size_t> read() = 0;

protected:
    uint32_t baud;

};

}

