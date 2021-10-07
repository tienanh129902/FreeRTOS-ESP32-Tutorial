#include <stdio.h>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "esp_system.h"
#include "esp_spi_flash.h"
#include "freertos/semphr.h"

SemaphoreHandle_t xSemaphore;

void task1(void *arg)
{
    while (1)
    {
        vTaskDelay(2000 / portTICK_RATE_MS);
        printf("Hello task 1\n");
        xSemaphoreGive(xSemaphore);
    }
}

void task2(void *arg)
{
    while (1)
    {
        vTaskDelay(2000 / portTICK_RATE_MS);
        printf("Hello task 2\n");
        xSemaphoreGive(xSemaphore);
    }
}

void task3(void *arg)
{
    while (1)
    {
        xSemaphoreTake(xSemaphore, 2000);
        xSemaphoreTake(xSemaphore, 2000);
        printf("Hello task 3\n");
    }
}

void app_main(void)
{
    xSemaphore = xSemaphoreCreateCounting(2, 0);
    xTaskCreate(task1, "task1", 1024, NULL, 10, NULL);
    xTaskCreate(task2, "task2", 1024, NULL, 10, NULL);
    xTaskCreate(task3, "task3", 1024, NULL, 10, NULL);
    xSemaphoreGive(xSemaphore);
}
