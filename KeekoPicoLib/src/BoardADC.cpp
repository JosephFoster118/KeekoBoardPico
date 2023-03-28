#include "Keeko/BoardADC.h"


namespace Keeko
{

BoardADC::BoardADC(uint8_t id):
                   id{id},
                   ADC(LIMITS)
{
    adc_init();
    adc_gpio_init(ADC_TO_GPIO[id]);
}

int16_t BoardADC::getValue()
{
    adc_select_input(id);
    return static_cast<uint16_t>(adc_read());
}

}