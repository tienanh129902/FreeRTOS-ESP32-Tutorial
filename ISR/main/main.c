#include <stdio.h>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "esp_system.h"
#include "esp_spi_flash.h"
#include "driver/gpio.h"
#include "freertos/timers.h"

#define GPIO_LED 2
#define BUTTON 0
#define ISR_FLAG_DEFAULT 0

TaskHandle_t ISR1 = NULL;
int cnt = 0;
int cur = 0;
bool status = false;
void IRAM_ATTR button_isr_handler()
{
    xTaskResumeFromISR(ISR1);
}

void button_task(void *arg)
{
    while (1)
    {
        vTaskSuspend(ISR1);
        status = !status;
        cnt++;
        gpio_set_level(GPIO_LED, status);
        printf("Button is pressed %d, led status is changed ! \n", cnt);
    }
}

void task1(void *arg)
{
    while (1)
    {
        printf("Hello Deviot\n");
        vTaskDelay(1000 / portTICK_PERIOD_MS);
    }
}

void app_main(void)
{
    gpio_pad_select_gpio(GPIO_LED);
    gpio_pad_select_gpio(BUTTON);
    //set input, output
    gpio_set_direction(GPIO_LED, GPIO_MODE_OUTPUT);
    gpio_set_direction(BUTTON, GPIO_MODE_INPUT);
    //set interrupt edge
    gpio_set_intr_type(BUTTON, GPIO_INTR_NEGEDGE);
    //set default flag
    gpio_install_isr_service(ISR_FLAG_DEFAULT);
    //attach the interrupt service routine
    gpio_isr_handler_add(BUTTON, button_isr_handler, NULL);
    //create task
    xTaskCreate(button_task, "button_task", 4096, NULL, 1, &ISR1);
    xTaskCreate(task1, "task1", 4096, NULL, 1, NULL);
}
