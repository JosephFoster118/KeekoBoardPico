#pragma once

#include <optional>

namespace Keeko
{

class DigitalIO
{
public:
    enum class Direction: uint8_t
    {
        INPUT,
        OUTPUT
    };

    enum class Pull: uint8_t
    {
        NONE,
        UP,
        DOWN
    };

    DigitalIO() = delete;
    DigitalIO(const DigitalIO&) = delete;
    DigitalIO(DigitalIO::Direction direction);
    DigitalIO(DigitalIO::Direction direction, DigitalIO::Pull pull);

    inline virtual ~DigitalIO(){};

    virtual void setValue(bool value) = 0;
    virtual bool getValue() = 0;

protected:
    Direction direction;
    std::optional<Pull> pull;
    bool output_value{false};
};

}