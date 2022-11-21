#include "main.h"

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

#include "KeekoBoard.h"
#include "KeekoMessage.h"

void UARTPrintf(uart_inst_t * port,const std::string_view& format, ...);

void vApplicationIdleHook( void )
{
}
void vApplicationTickHook( void )
{
}

const uint LED_PIN = PICO_DEFAULT_LED_PIN;

void setupOnBoardLED()
{
    gpio_init(LED_PIN);
    gpio_set_dir(LED_PIN, GPIO_OUT);
}

static void blinkTask(void *pvParameters)
{
    UARTPrintf(uart0, "Doing thingy 2\r\n");
    const uint64_t BLINK_DELAY = pdMS_TO_TICKS(400);
    const uint64_t BLINK_DELAY2 = pdMS_TO_TICKS(100);
    TickType_t xNextWakeTime = xTaskGetTickCount();
    bool state{true};
    for(;;)
    {
        gpio_put(LED_PIN, state);
        state = !state;
        vTaskDelayUntil( &xNextWakeTime, state ? BLINK_DELAY:BLINK_DELAY2 );
    }
}

void UARTPrintf(uart_inst_t * port,const std::string_view& format, ...)
{
    char buffer[128];
    va_list args;
    va_start(args,format);
    auto count = vsnprintf(buffer,sizeof(buffer), std::string{format}.c_str(), args);
    va_end(args);
    uart_write_blocking(port, reinterpret_cast<uint8_t*>(buffer), count);
}

static void usbTask(void *pvParameters)
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

int main(void)
{
    // uart_init(uart0, 115200);
    // gpio_set_function(0, GPIO_FUNC_UART);
    // gpio_set_function(1, GPIO_FUNC_UART);
    
    
    // try
    // {
    //     throw std::string{"ah"};
    // }
    // catch(const std::string& ex)
    // {
        
    // }
	

    // setupOnBoardLED();
    // gpio_put(LED_PIN, true);
    // TaskHandle_t blink_task_hable = nullptr;
    // TaskHandle_t usb_task_handle = nullptr;
    // BaseType_t blink_task = xTaskCreate(blinkTask, "PICO_LED_TASK",  128, NULL, 1, &blink_task_hable);
    // BaseType_t usb_task = xTaskCreate(usbTask, "USB_TASK",  128, NULL, 1, &usb_task_handle);
    Keeko::KeekoMessage msg{};
    msg.addElement(123, std::string{"hello"});
    auto [foo, size] = msg.getRaw();
    KeekoBoard board{};
    board.runSceduler();
    //If we get here things have gone very wrong
    return 0;
}

uint16_t tud_hid_get_report_cb(uint8_t instance, uint8_t report_id, hid_report_type_t report_type, uint8_t* buffer, uint16_t reqlen)
{
  // TODO not Implemented
  (void) instance;
  (void) report_id;
  (void) report_type;
  (void) buffer;
  (void) reqlen;

  return 0;
}

void tud_hid_set_report_cb(uint8_t instance, uint8_t report_id, hid_report_type_t report_type, uint8_t const* buffer, uint16_t bufsize)
{
    // TODO set LED based on CAPLOCK, NUMLOCK etc...
    // printf("DEBUG: tud_hid_set_report_cb triggered\n");
    // printf("DEBUG: report_id: %X\n", report_id);
    // printf("DEBUG: report_type: %X\n", report_type);
    // printf("DEBUG: bufsize: %d\n", bufsize);

    // printf("DEBUG: buffer content:\n");
    // for (int i = 0; i < bufsize; i++) {
    //     printf("%02X ", buffer[i]);
    // }
    // printf(" - End \n");
    (void) report_id;
    (void) report_type;
    (void) buffer;
    (void) bufsize;
}
