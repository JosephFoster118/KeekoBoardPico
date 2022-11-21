#pragma once
#include <memory>

class BoardMessage
{
public:
    BoardMessage(const std::unique_ptr<uint8_t[]>& raw);

    BoardMessage() = delete;
    BoardMessage(const BoardMessage&) = delete;
protected:

private:
};

