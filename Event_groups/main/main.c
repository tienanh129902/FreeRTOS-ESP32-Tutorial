#include <stdio.h>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "esp_system.h"
#include "esp_spi_flash.h"
#include "freertos/event_groups.h"

#define TASK1_BIT (1UL << 0UL) // zero shift for bit0
#define TASK2_BIT (1UL << 1UL) // 1 shift for flag  bit 1
#define TASK3_BIT (1UL << 2UL) // 2 shift for flag bit 2

//  declare a event grounp handler variable
EventGroupHandle_t xEventGroup;

// A constant to define 1s delay
const TickType_t xMyDelay = pdMS_TO_TICKS(1000);

// defintion of input and output tasks
void inputTask1(void *arg)
{
    while (1)
    {
        // set flag bit TASK1_BIT
        xEventGroupSetBits(xEventGroup, TASK1_BIT);
        //delay this task for 1s
        vTaskDelay(xMyDelay);
    }
}

void inputTask2(void *arg)
{
    while (1)
    {
        // set flag bit TASK2_BIT
        xEventGroupSetBits(xEventGroup, TASK2_BIT);
        //delay this task for 1s
        vTaskDelay(xMyDelay);
    }
}

void inputTask3(void *arg)
{
    while (1)
    {
        // set flag bit TASK2_BIT
        xEventGroupSetBits(xEventGroup, TASK3_BIT);
        //delay this task for 1s
        vTaskDelay(xMyDelay);
    }
}


void outputTask(void *pvParameters)
{
    // define a variable which holds the state of events
    const EventBits_t xBitsToWaitFor = (TASK1_BIT | TASK2_BIT | TASK3_BIT);
    EventBits_t xEventGroupValue;
    while (1)
    {
        xEventGroupValue = xEventGroupWaitBits(xEventGroup,
                                               xBitsToWaitFor,
                                               pdTRUE,
                                               pdTRUE,
                                               portMAX_DELAY);
        if ((xEventGroupValue & TASK1_BIT) != 0)
        {
            printf("Task 1 event occured\n");
        }
        if ((xEventGroupValue & TASK2_BIT) != 0)
        {
            printf("Task 2 event occured\n");
        }
        if ((xEventGroupValue & TASK3_BIT) != 0)
        {
            printf("Task 3 event occured\n");
            
        }
    }
    vTaskDelay(xMyDelay);
    xEventGroupClearBits(xEventGroup,TASK1_BIT);
    xEventGroupClearBits(xEventGroup,TASK2_BIT);
    xEventGroupClearBits(xEventGroup,TASK3_BIT);
}

void app_main(void)
{
    //create event group and assign it a earlier created referene handler
    xEventGroup = xEventGroupCreate();
    // Create three tasks which are used to release event with
    // the help of above defined event flags
    xTaskCreate(inputTask1, "Input Task 1", 1024, NULL, 1, NULL);
    xTaskCreate(inputTask2, "Input Task 2", 1024, NULL, 1, NULL);
    xTaskCreate(inputTask3, "Input Task 3", 1024, NULL, 1, NULL);
    //Create output task that will execute only when all three events occured
    xTaskCreate(outputTask, "Output Task", 1024, NULL, 1, NULL);
}

