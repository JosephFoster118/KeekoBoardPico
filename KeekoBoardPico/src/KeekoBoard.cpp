#include "KeekoBoard.h"

#include "Keeko/Utility.h"

KeekoBoard::KeekoBoard()
{
    tusb_init();
    robot_comms_uart = std::unique_ptr<std::decay<decltype(*robot_comms_uart)>::type>(new Keeko::USBUart(ROBOT_COMMS_UART_ID));
    debug_comms_uart = std::unique_ptr<std::decay<decltype(*debug_comms_uart)>::type>(new Keeko::USBUart(DEBUG_COMMS_UART_ID));
    touch_comms_uart = std::unique_ptr<std::decay<decltype(*touch_comms_uart)>::type>(new Keeko::BoardUart(uart0, 576000, 16, 17));

    robot_comms_mutex = xSemaphoreCreateBinary();
    xSemaphoreGive(robot_comms_mutex);//Mutex starts empty

    pending_touch_message_mutex = xSemaphoreCreateBinary();
    xSemaphoreGive(pending_touch_message_mutex);//Mutex starts empty

    tusb_mutex = xSemaphoreCreateBinary();
    xSemaphoreGive(tusb_mutex);//Mutex starts empty



    blink_task = xTaskCreate(KeekoBoard::blinkStarter,
                             "PICO_LED_TASK",
                             32*3,
                             this,
                             1,
                             &blink_task_handle);

    robot_comms_task = xTaskCreate(KeekoBoard::robotCommsStarter,
                             "ROBOT_COMMS_TASK",
                             128*4,
                             this,
                             1,
                             &robot_comms_task_handle);
    
    controller_task = xTaskCreate(KeekoBoard::controllerTaskStarter,
                             "CONTROLLER_TASK",
                             128*4,
                             this,
                             1,
                             &controller_task_handle);

    // debug_comms_task = xTaskCreate(KeekoBoard::debugCommsStarter,
    //                          "DEBUG_COMMS_TASK",
    //                          128,
    //                          this,
    //                          1,
    //                          &debug_comms_task_handle);

    tusb_task = xTaskCreate(KeekoBoard::tusbTaskStarter,
                             "TUSB_TASK",
                             128,
                             this,
                             1,
                             &tusb_task_handle);

    // tusb_task = xTaskCreate(KeekoBoard::tusbTaskStarter,
    //                          "TUSB_TASK",
    //                          128,
    //                          this,
    //                          1,
    //                          &tusb_task_handle);
}

KeekoBoard::~KeekoBoard()
{
    
}

void KeekoBoard::blinkTask()
{
    gpio_init(PICO_DEFAULT_LED_PIN);
    gpio_set_dir(PICO_DEFAULT_LED_PIN, GPIO_OUT);
    const uint64_t BLINK_DELAY = pdMS_TO_TICKS(450);
    const uint64_t BLINK_DELAY2 = pdMS_TO_TICKS(50);
    TickType_t xNextWakeTime = xTaskGetTickCount();
    bool state{false};
    Keeko::BoardDigitalIO test_led{15,
                                   Keeko::DigitalIO::Direction::OUTPUT};
    for(;;)
    {
        xSemaphoreTake(robot_comms_mutex, portMAX_DELAY);
        if(isRobotCommsConnected)
        {
            gpio_put(PICO_DEFAULT_LED_PIN, ((time_us_64()%200000) > 100000));
            test_led.setValue(!((time_us_64()%200000) > 100000));
        }
        else
        {
            gpio_put(PICO_DEFAULT_LED_PIN, ((time_us_64()%500000) > 250000));
            test_led.setValue(!((time_us_64()%500000) > 250000));
        }
        //state = !state;
        xSemaphoreGive(robot_comms_mutex);
        vTaskDelayUntil( &xNextWakeTime, pdMS_TO_TICKS(50));
    }
}

void KeekoBoard::robotCommsTask()
{
    uint64_t last_message{0};
    std::unique_ptr<uint8_t[]> received_message{};
    size_t message_size = 0;
    for(;;)
    {
        TickType_t current_time = xTaskGetTickCount();
        xSemaphoreTake(robot_comms_mutex, portMAX_DELAY);
        if(robot_comms_uart->isReadable())
        {
            isRobotCommsConnected = true;
            std::unique_ptr<uint8_t[]> read_data = std::unique_ptr<uint8_t[]>(new uint8_t[ROBOT_COMMS_READ_MAX_LENGTH]);
            size_t read_data_size{0};
            uint64_t read_timeout = time_us_64() + ROBOT_COMMS_READ_TIMEOUT;
            while(time_us_64() < read_timeout)//We know a message is done being sent if there are ROBOT_COMMS_READ_MAX_LENGTH microseconds since the last byte was recieved
            {
                if(robot_comms_uart->isReadable())
                {
                    auto [data, size] = robot_comms_uart->read();
                    if(size > 0)
                    {
                        read_timeout = time_us_64() + ROBOT_COMMS_READ_TIMEOUT;
                    }
                    if(size + read_data_size < ROBOT_COMMS_READ_MAX_LENGTH)//If the user sends to much data, do not copy to buffer so we wont crash
                    {
                        for(uint16_t i = 0; i < size; i++)
                        {
                            read_data[i + read_data_size] = data[i];
                        }
                    }
                    read_data_size += size;
                    taskYIELD();
                }
            }
            char msg[16];
            auto msg_size = snprintf(msg, 16,"%d\n", read_data_size);
            last_message = current_time;
            received_message = std::move(read_data);
            message_size = read_data_size;
        }
        else
        {
            if(isRobotCommsConnected)
            {
                if(current_time >= (last_message + ROBOT_COMMS_TIMEOUT))
                {
                    isRobotCommsConnected = false;
                }
            }
        }
        if(received_message)
        {
            processRobotMessage(received_message, message_size);
            received_message.reset();
            message_size = 0;
        }
        xSemaphoreGive(robot_comms_mutex);
        taskYIELD();
    }
}

void KeekoBoard::processRobotMessage(const std::unique_ptr<uint8_t[]>& raw_message, size_t message_size)
{
    bool valid;
    std::optional<Keeko::KeekoMessageError> error;
    std::tie(valid, error) = Keeko::KeekoMessage::validateRawData(raw_message, message_size);
    Keeko::KeekoMessage resp;
    if(valid)
    {
        Keeko::KeekoMessage robot_message{raw_message, message_size};
        resp.addElement(Keeko::CRC::calculateCRC32("Ack"), uint8_t{1}, 0);
    }
    else
    {
        resp.addElement(Keeko::CRC::calculateCRC32("Nack"), std::string{error.value().error_string}, 0);
    }
    std::unique_ptr<uint8_t[]> resp_raw;
    size_t resp_size;
    resp.getRaw(resp_raw, resp_size);
    uint16_t current_pos = 0;
    while(current_pos < resp_size)//Reply
    {
        uint16_t available = robot_comms_uart->availableWrite();
        while(available == 0)
        {
            taskYIELD();//Need to wait for next time we can write, switch tasks
            available = robot_comms_uart->availableWrite();
        }
        if(available >= resp_size - current_pos - 1)
        {
            robot_comms_uart->write(resp_raw.get() + current_pos, resp_size - current_pos);
        }
        else
        {
            robot_comms_uart->write(resp_raw.get() + current_pos, available);
        }
        current_pos += available;
        taskYIELD();//We have sent data, switch tasks so the USB task can start sending the data
    }
}

void KeekoBoard::touchCommsTask()
{
    for(;;)
    {
        taskYIELD();
    }
}

void KeekoBoard::debugCommsTask()
{
    const uint64_t BLINK_DELAY = pdMS_TO_TICKS(400);
    for(;;)
    {
        if(!debug_comms_uart->isReadable())
        {
            taskYIELD();
            continue;
        }
        auto [read_data,read_size] = debug_comms_uart->read();
        debug_comms_uart->write(read_data.get(), read_size);
    }
}

void KeekoBoard::tusbTask()
{
    for(;;)
    {
        taskENTER_CRITICAL();
        xSemaphoreTake(tusb_mutex, portMAX_DELAY);
        tud_task(); // tinyusb device task
        //controllerTask();
        xSemaphoreGive(tusb_mutex);
        taskEXIT_CRITICAL();
        taskYIELD();
    }
}


void KeekoBoard::runSceduler() noexcept
{
    vTaskStartScheduler();
}

void KeekoBoard::controllerTask()
{
    //TODO: figure out how to remove delay
    vTaskDelay(pdMS_TO_TICKS(1000));//Let everything boot up
    auto last_wake_time = xTaskGetTickCount();
    auto period = pdMS_TO_TICKS(50);
    Keeko::BoardADC test_adc(0);
    uint32_t counter = 0;
    for(;;)
    {
        vTaskDelayUntil(&last_wake_time, period);
        taskENTER_CRITICAL();
        xSemaphoreTake(tusb_mutex, portMAX_DELAY);
        hid_gamepad_report_t report{0,};
        report.x = static_cast<int8_t>(static_cast<int16_t>(test_adc.getValue() >> 4) - 128);
        report.buttons = collectControllerButtons();
        tud_hid_report(REPORT_ID_GAMEPAD, &report, sizeof(report));
        xSemaphoreGive(tusb_mutex);
        taskEXIT_CRITICAL();
        counter++;
    }
}

uint32_t KeekoBoard::collectControllerButtons()
{
    uint32_t result = 0;
    for(const ControllerButtonInput& button_input: CONTROLLER_BUTTON_MAP)
    {
        if(!button_input.gpio_input->getValue())
        {
            result |= Keeko::highBitByIndex<decltype(result)>(button_input.controller_button_id);
        }
    }
    return result;
}
