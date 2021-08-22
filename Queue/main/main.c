#include <stdio.h>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "freertos/queue.h"

TaskHandle_t task1Handle = NULL;
TaskHandle_t task2Handle = NULL;
QueueHandle_t queue1;

void task1(void *param)
{
    char txbuff[50];
    queue1 = xQueueCreate(5, sizeof(txbuff));
    if (queue1 == 0)
    {
        printf("failed to create queue1= %p \n", queue1); // Failed to create the queue.
    }

    sprintf(txbuff, "hello world! 1");
    xQueueSendToBack(queue1, (void *)txbuff, (TickType_t)0);

    sprintf(txbuff, "hello world! 2");
    xQueueSend(queue1, (void *)txbuff, (TickType_t)0);

    sprintf(txbuff, "hello world! 3");
    xQueueSendToFront(queue1, (void *)txbuff, (TickType_t)0);

    sprintf(txbuff, "hello world! 4");
    xQueueSend(queue1, (void *)txbuff, (TickType_t)0);

    sprintf(txbuff, "hello world! 5");
    xQueueSendToFront(queue1, (void *)txbuff, (TickType_t)0);

    while (1)
    {
        printf("data waiting to be read : %d  available spaces: %d \n", uxQueueMessagesWaiting(queue1), uxQueueSpacesAvailable(queue1));
        vTaskDelay(pdMS_TO_TICKS(1000));
    }

    vTaskDelete(NULL);
}

void task2(void *param)
{
    char rxbuff[50];
    while (1)

    {
        if (xQueueReceive(queue1, &(rxbuff), (TickType_t)5))
        {
            printf("got a data from queue 1!  ===  %s \n", rxbuff);
        }

        vTaskDelay(pdMS_TO_TICKS(1000));
    }
    vTaskDelete(NULL);
}

void app_main()
{
    xTaskCreate(task1, "task1", 4096, NULL, 10, task1Handle);
    xTaskCreate(task2, "task2", 4096, NULL, 9, task2Handle);
}