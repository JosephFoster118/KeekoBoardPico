#include "Keeko/DigitalIO.h"
namespace Keeko
{

DigitalIO::DigitalIO(DigitalIO::Direction direction):
    direction{direction}
{
    if(direction == Direction::INPUT)
    {
        pull = Pull::NONE;
    }
}

DigitalIO::DigitalIO(DigitalIO::Direction direction, DigitalIO::Pull pull):
    direction{direction},
    pull{pull}
{
}



}