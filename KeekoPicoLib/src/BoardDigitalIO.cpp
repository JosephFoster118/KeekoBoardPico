#include "Keeko/BoardDigitalIO.h"

namespace Keeko
{

BoardDigitalIO::BoardDigitalIO(uint8_t gpio_id, DigitalIO::Direction direction):
    DigitalIO{direction},
    gpio_id{gpio_id}
{
    initialize();
}

BoardDigitalIO::BoardDigitalIO(uint8_t gpio_id, DigitalIO::Direction direction, DigitalIO::Pull pull):
    DigitalIO{direction, pull},
    gpio_id{gpio_id}
{
    initialize();
}

BoardDigitalIO::~BoardDigitalIO()
{
    gpio_deinit(gpio_id);
}

void BoardDigitalIO::initialize()
{
    gpio_init(gpio_id);
    if(direction == Direction::OUTPUT)
    {
        gpio_set_dir(gpio_id, GPIO_OUT);
    }
    else
    {
        gpio_set_dir(gpio_id, GPIO_IN);
    }
    if(pull.has_value())
    {
        switch(pull.value())
        {
            case Pull::NONE:
            {
                gpio_disable_pulls(gpio_id);
            }break;
            case Pull::UP:
            {
                gpio_pull_up(gpio_id);
            }break;
            case Pull::DOWN:
            {
                gpio_pull_down(gpio_id);
            }break;
        }
    }
}

void BoardDigitalIO::setValue(bool value)
{
    //Only set the value if the direction is output, else ignore
    if(direction == Direction::OUTPUT)
    {
        output_value = value;//Store value for fast recall
        gpio_put(gpio_id, output_value);
    }
}

bool BoardDigitalIO::getValue()
{
    if(direction == Direction::OUTPUT)
    {
        return output_value;//Return value stored for fast recall
    }
    return gpio_get(gpio_id);
}

}
