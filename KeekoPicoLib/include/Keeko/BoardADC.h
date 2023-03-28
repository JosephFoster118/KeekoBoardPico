#pragma once

#include <array>

#include "hardware/adc.h"
#include "Keeko/ADC.h"


namespace Keeko
{

class BoardADC: public ADC
{
public:
    static constexpr ADC::Limits LIMITS{0,4095};

    static constexpr std::array<uint8_t, 3> ADC_TO_GPIO
    {
        26,
        27,
        28
    };

    BoardADC() = delete;
    BoardADC(uint8_t id);

    int16_t getValue();
protected:
    uint8_t id;
};

}