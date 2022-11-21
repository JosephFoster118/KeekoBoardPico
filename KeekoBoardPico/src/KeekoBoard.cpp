#include "KeekoBoard.h"


KeekoBoard::KeekoBoard()
{
    blink_task = xTaskCreate(KeekoBoard::blinkStarter,
                             "PICO_LED_TASK",
                             128,
                             this,
                             1,
                             &blink_task_handle);
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
    bool state{true};
    for(;;)
    {
        gpio_put(PICO_DEFAULT_LED_PIN, state);
        state = !state;
        vTaskDelayUntil( &xNextWakeTime, state ? BLINK_DELAY:BLINK_DELAY2 );
    }
}

void KeekoBoard::runSceduler() noexcept
{
    vTaskStartScheduler();
}