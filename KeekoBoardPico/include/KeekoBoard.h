#pragma once

/* Kernel includes. */
#include <FreeRTOS.h> /* Must come first. */
#include "task.h"     /* RTOS task related API prototypes. */
#include "queue.h"    /* RTOS queue related API prototypes. */
#include "timers.h"   /* Software timer related API prototypes. */
#include "semphr.h"   /* Semaphore related API prototypes. */
#include "portable.h"

//Pico includes
#include "pico/stdlib.h"
#include "pico/binary_info.h"
#include "hardware/timer.h"
#include "hardware/structs/uart.h"
#include "tusb.h"
#include "usb_descriptors.h"

#include <stdio.h>
#include <stdarg.h>
#include <string>
#include <string_view>
#include <functional>
#include <cstring>
#include <memory>

//Project includes
#include "Keeko/USBUart.h"
#include "Keeko/BoardUart.h"
#include "Keeko/KeekoMessage.h"
#include "Keeko/DigitalIO.h"
#include "Keeko/BoardDigitalIO.h"

class KeekoBoard
{
public:
    KeekoBoard();
    KeekoBoard(const KeekoBoard&) = delete;

    ~KeekoBoard();

    void blinkTask();
    static void blinkStarter(void* ptr)
    {
        reinterpret_cast<KeekoBoard*>(ptr)->blinkTask();
    };

    void robotCommsTask();
    static void robotCommsStarter(void* ptr)
    {
        reinterpret_cast<KeekoBoard*>(ptr)->robotCommsTask();
    }

    void touchCommsTask();
    static void touchCommsTaskStarter(void* ptr)
    {
        reinterpret_cast<KeekoBoard*>(ptr)->tusbTask();
    }

    void debugCommsTask();
    static void debugCommsStarter(void* ptr)
    {
        reinterpret_cast<KeekoBoard*>(ptr)->debugCommsTask();
    }

    void tusbTask();
    static void tusbTaskStarter(void* ptr)
    {
        reinterpret_cast<KeekoBoard*>(ptr)->tusbTask();
    }

    void controllerTask();
    static void controllerTaskStarter(void* ptr)
    {
        reinterpret_cast<KeekoBoard*>(ptr)->controllerTask();
    }

    void runSceduler() noexcept;

    //Constants
    static constexpr uint8_t ROBOT_COMMS_UART_ID{0};
    static constexpr uint8_t DEBUG_COMMS_UART_ID{1};
    static constexpr uint64_t ROBOT_COMMS_TIMEOUT{pdMS_TO_TICKS(1000)};
    static constexpr uint64_t ROBOT_COMMS_READ_TIMEOUT{1000};//100 microseconds
    static constexpr size_t ROBOT_COMMS_READ_MAX_LENGTH{256};

private:
    //Tasks
    TaskHandle_t blink_task_handle;
    BaseType_t blink_task{0};
    TaskHandle_t robot_comms_task_handle;
    BaseType_t robot_comms_task{0};
    TaskHandle_t debug_comms_task_handle;
    BaseType_t debug_comms_task{0};
    TaskHandle_t tusb_task_handle;
    BaseType_t tusb_task{0};
    TaskHandle_t touch_comms_task_handle;
    BaseType_t touch_comms_task{0};

    TaskHandle_t controller_task_handle;
    BaseType_t controller_task{0};

    std::unique_ptr<Keeko::USBUart> robot_comms_uart;
    std::unique_ptr<Keeko::USBUart> debug_comms_uart;
    std::unique_ptr<Keeko::BoardUart> touch_comms_uart;

    SemaphoreHandle_t pending_touch_message_mutex;
    std::optional<Keeko::KeekoMessage> pending_touch_message;

    SemaphoreHandle_t robot_comms_mutex;
    SemaphoreHandle_t tusb_mutex;

    bool isRobotCommsConnected{true};

    void processRobotMessage(const std::unique_ptr<uint8_t[]>& raw_message, size_t message_size);

    //Controller Config
    struct ControllerButtonInput
    {
        uint8_t controller_button_id;
        std::shared_ptr<Keeko::DigitalIO> gpio_input;
    };
    
    std::array<ControllerButtonInput, 2> CONTROLLER_BUTTON_MAP = 
    {
        ControllerButtonInput{
            0, //Controller button ID
            std::make_shared<Keeko::BoardDigitalIO>
            (
                14, //GPIO pin
                Keeko::DigitalIO::Direction::INPUT, //It is an input
                Keeko::DigitalIO::Pull::DOWN //Add a pull down resistor
            )
        },
        ControllerButtonInput{
            1, //Controller button ID
            std::make_shared<Keeko::BoardDigitalIO>
            (
                16, //GPIO pin
                Keeko::DigitalIO::Direction::INPUT, //It is an input
                Keeko::DigitalIO::Pull::DOWN //Add a pull down resistor
            )
        }
    };
    
};
