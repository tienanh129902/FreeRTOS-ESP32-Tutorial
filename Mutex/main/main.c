#include <stdio.h>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "esp_system.h"
#include "esp_spi_flash.h"
#include "freertos/semphr.h"

SemaphoreHandle_t xMutex;

void task1(void *arg)
{
    if (xSemaphoreTake(xMutex, portMAX_DELAY) == pdTRUE)
    {
        for (int i = 0; i < 3; i++)
        {
            printf("Hello from task 1\n");
            vTaskDelay(1000 / portTICK_PERIOD_MS);
        }
        xSemaphoreGive(xMutex);
    }
    vTaskDelete(NULL);
}

void task2(void *arg)
{
    if (xSemaphoreTake(xMutex, portMAX_DELAY) == pdTRUE)
    {
        for (int i = 0; i < 5; i++)
        {
            printf("Hello from task 2\n");
            vTaskDelay(1000 / portTICK_PERIOD_MS);
        }
        xSemaphoreGive(xMutex);
    }
    vTaskDelete(NULL);
}

void app_main(void)
{
    xMutex = xSemaphoreCreateMutex();
    xTaskCreate(task1, "task1", 1024, NULL, 1, NULL);
    xTaskCreate(task2, "task2", 1024, NULL, 1, NULL);
}
