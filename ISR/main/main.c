#include <stdio.h>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "esp_system.h"
#include "esp_spi_flash.h"
#include "driver/gpio.h"

#define GPIO_LED 2
#define BUTTON 0
#define ISR_FLAG_DEFAULT 0

TaskHandle_t ISR1 = NULL;
int cnt = 0;

void IRAM_ATTR button_isr_handler()
{
    xTaskResumeFromISR(ISR1);
}

void button_task(void *arg)
{
    bool status = false;
    while(1)
    {
        vTaskSuspend(ISR1);
        status = !status;
        cnt++;
        gpio_set_level(GPIO_LED,status);
        printf("Button is pressed %d, led status is changed ! \n",cnt);
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
    gpio_set_intr_type(BUTTON,GPIO_INTR_NEGEDGE);
    //set default flag
    gpio_install_isr_service(ISR_FLAG_DEFAULT);
    //attach the interrupt service routine
    gpio_isr_handler_add(BUTTON,button_isr_handler,NULL);
    //create task
    xTaskCreate(button_task,"button_task",4096,NULL,1,&ISR1);
}
