/* Hello World Example

   This example code is in the Public Domain (or CC0 licensed, at your option.)

   Unless required by applicable law or agreed to in writing, this
   software is distributed on an "AS IS" BASIS, WITHOUT WARRANTIES OR
   CONDITIONS OF ANY KIND, either express or implied.
*/
#include <stdio.h>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "freertos/timers.h"
#include "esp_system.h"
#include "esp_spi_flash.h"

static TimerHandle_t one_shot_timer = NULL;
static TimerHandle_t auto_reload_timer = NULL;

// Called when one of the timers expires
void myTimerCallback(TimerHandle_t xTimer)
{

    // Print message if timer 0 expired
    if ((uint32_t)pvTimerGetTimerID(xTimer) == 0)
    {
        printf("One-shot timer expired\n");
    }

    // Print message if timer 1 expired
    if ((uint32_t)pvTimerGetTimerID(xTimer) == 1)
    {
        printf("Auto-reload timer expired\n",);
    }
}

xTimerStop

void app_main(void)
{
    vTaskDelay(1000 / portTICK_PERIOD_MS);
    printf("FreeRTOS Timer Demo\n");

    // Create a one-shot timer
    one_shot_timer = xTimerCreate(
        "One-shot timer",          // Name of timer
        2000 / portTICK_PERIOD_MS, // Period of timer (in ticks)
        pdFALSE,                   // no Auto-reload
        (void *)0,                 // Timer ID
        myTimerCallback);          // Callback function

    // Create an auto-reload timer
    auto_reload_timer = xTimerCreate(
        "Auto-reload timer",       // Name of timer
        1000 / portTICK_PERIOD_MS, // Period of timer (in ticks)
        pdTRUE,                    // Auto-reload
        (void *)1,                 // Timer ID
        myTimerCallback);          // Callback function

    // Check to make sure timers were created
    if (one_shot_timer == NULL || auto_reload_timer == NULL)
    {
        printf("Could not create one of the timers\n");
    }
    else
    {

        // Wait and then print out a message that we're starting the timers
        vTaskDelay(1000 / portTICK_PERIOD_MS);
        printf("Starting timers...\n");

        // Start timers (max block time if command queue is full)
        xTimerStart(one_shot_timer, portMAX_DELAY);
        xTimerStart(auto_reload_timer, portMAX_DELAY);
    }

    // Delete self task to show that timers will work with no user tasks
    vTaskDelete(NULL);
}
