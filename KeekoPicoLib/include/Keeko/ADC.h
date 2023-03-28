#pragma once
#include <utility>
#include <cstdint>

namespace Keeko
{

class ADC
{
public:

    typedef std::pair<int16_t,int16_t> Limits;

    ADC() = delete;
    ADC(Limits limits);

    virtual int16_t getValue() = 0;

protected:

    Limits limits;

};

}