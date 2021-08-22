#include <stdio.h>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "esp_system.h"
#include "esp_spi_flash.h"

TaskHandle_t handle1 = NULL;
TaskHandle_t handle2 = NULL;

void task1(void *param)
{
    for (int i = 0; i < 7; i++)
    {
        printf("Hello from task 1 \n");
        vTaskDelay(1000 / portTICK_RATE_MS);
    }
    vTaskDelete(NULL);
}

void task2(void *param)
{
    for (int i = 0; i < 5; i++)
    {
        printf("Hello from task 2 \n");
        vTaskDelay(1000 / portTICK_RATE_MS);
    }
    vTaskDelete(NULL);
}

void app_main(void)
{
    xTaskCreate(task1, "task1", 1024, NULL, 10, &handle1);
    xTaskCreate(task2, "task2", 1024, NULL, 7, &handle2);
    vTaskStartScheduler();
}
