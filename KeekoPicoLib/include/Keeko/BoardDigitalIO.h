#pragma once

#include "hardware/gpio.h"
#include "Keeko/DigitalIO.h"

namespace Keeko
{

class BoardDigitalIO : public DigitalIO
{
public:
    BoardDigitalIO() = delete;
    BoardDigitalIO(const BoardDigitalIO&) = delete;
    BoardDigitalIO(uint8_t gpio_id, DigitalIO::Direction direction);
    BoardDigitalIO(uint8_t gpio_id, DigitalIO::Direction direction, DigitalIO::Pull pull);

    virtual ~BoardDigitalIO();

    void setValue(bool value) override;
    bool getValue() override;
protected:
    void initialize();
    uint8_t gpio_id;
};

}
