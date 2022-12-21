#include "KeekoBoard.h"


KeekoBoard::KeekoBoard()
{
    blink_task = xTaskCreate(KeekoBoard::blinkStarter,
                             "PICO_LED_TASK",
                             128,
                             this,
                             1,
                             &blink_task_handle);

    robot_comms_task = xTaskCreate(KeekoBoard::robotCommsStarter,
                             "ROBOT_COMMS_TASK",
                             128,
                             this,
                             1,
                             &robot_comms_task_handle);

    tusb_task = xTaskCreate(KeekoBoard::tusbTaskStarter,
                             "TUSB_TASK",
                             128,
                             this,
                             1,
                             &tusb_task_handle);
}

KeekoBoard::~KeekoBoard()
{
    
}

void KeekoBoard::blinkTask()
{
    gpio_init(PICO_DEFAULT_LED_PIN);
    gpio_set_dir(PICO_DEFAULT_LED_PIN, GPIO_OUT);
    const uint64_t BLINK_DELAY = pdMS_TO_TICKS(400);
    const uint64_t BLINK_DELAY2 = pdMS_TO_TICKS(100);
    TickType_t xNextWakeTime = xTaskGetTickCount();
    bool state{false};
    for(;;)
    {
        gpio_put(PICO_DEFAULT_LED_PIN, state);
        state = !state;
        vTaskDelay(state ? BLINK_DELAY:BLINK_DELAY2);
        //vTaskDelayUntil( &xNextWakeTime, state ? BLINK_DELAY:BLINK_DELAY2 );
    }
}

void KeekoBoard::robotCommsTask()
{
    const uint64_t BLINK_DELAY = pdMS_TO_TICKS(400);
    for(;;)
    {
        vTaskDelay(BLINK_DELAY);
    }
}

void KeekoBoard::tusbTask()
{
    tusb_init();
    for(;;)
    {
        taskENTER_CRITICAL();
        tud_task(); // tinyusb device task
        taskEXIT_CRITICAL();
        taskYIELD();
    }
}


void KeekoBoard::runSceduler() noexcept
{
    vTaskStartScheduler();
}