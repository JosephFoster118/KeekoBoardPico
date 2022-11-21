#pragma once

/* Kernel includes. */
#include <FreeRTOS.h> /* Must come first. */
#include "task.h"     /* RTOS task related API prototypes. */
#include "queue.h"    /* RTOS queue related API prototypes. */
#include "timers.h"   /* Software timer related API prototypes. */
#include "semphr.h"   /* Semaphore related API prototypes. */

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
    void runSceduler() noexcept;

private:
    TaskHandle_t blink_task_handle;
    BaseType_t blink_task{0};
};
