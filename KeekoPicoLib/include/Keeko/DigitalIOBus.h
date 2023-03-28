#pragma once

#include <functional>

namespace Keeko
{

class DigitalIOBus
{
public:
    std::function<void(bool)> lockResource;

};

}