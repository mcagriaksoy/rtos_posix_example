// Mehmet Cagri Aksoy
// RTOS Example - 21.11.2023

// Standard includes
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

// FreeRTOS includes
#include "FreeRTOS.h"
#include "semphr.h"
#include "task.h"

#define MAX_EAT_TIME (1000) // milliseconds
#define TASK_NUMBER (5)

// Global variables
static SemaphoreHandle_t chopstick[TASK_NUMBER];
static int count = 0;

void eat(void *pvParameters)
{
    int number = *(int *)pvParameters;
    printf("Customer %d is waiting for chopstick...\r\n", number);

    // Customer preparing to eat
    vTaskDelay(pdMS_TO_TICKS(500));

    // Take right chopstick
    if (xSemaphoreTake(chopstick[(number + 1) % TASK_NUMBER], MAX_EAT_TIME) != pdTRUE)
    {
        printf("Customer %d cannot get right chopstick...\r\n", number);
    }

    // Take left chopstick
    if (xSemaphoreTake(chopstick[number], MAX_EAT_TIME) != pdTRUE)
    {
        printf("Customer %d cannot get left chopstick...\r\n", number);
    }

    // Eat
    printf("Customer %d is eating...\r\n", number);
    vTaskDelay(pdMS_TO_TICKS(1000));

    // Put right chopstick
    if (xSemaphoreGive(chopstick[(number + 1) % TASK_NUMBER]) != pdTRUE)
    {
        printf("Customer %d cannot put right chopstick...\r\n", number);
    }

    // Put left chopstick
    if (xSemaphoreGive(chopstick[number]) != pdTRUE)
    {
        printf("Customer %d cannot put left chopstick...\r\n", number);
    }

    // Customer finished eating
    printf("Customer %d finished eating...\r\n", number);
    // Notify main task and delete self
    ++count;

    // Wait for all customers to finish eating
    if (count == TASK_NUMBER)
    {
        printf("All customers finished eating...\r\n");
        vTaskEndScheduler();
    }

    vTaskDelete(NULL);
}

int main()
{
    printf("Customer Chopstick example \r\n");

    // Create mutex
    for (int i = 0; i < TASK_NUMBER; i++)
    {
        chopstick[i] = xSemaphoreCreateMutex();
    }

    char task_name[20] = {0};
    for (int i = 0; i < TASK_NUMBER; i++)
    {
        sprintf(task_name, "Customer %i", i);

        // Allocate memory for task parameter
        int *task_param = (int *)pvPortMalloc(sizeof(int));
        *task_param = i;

        if (xTaskCreate(eat, task_name, 1024, task_param, 1, NULL) != pdPASS)
        {
            printf("Failed to create %s task\r\n", task_name);
            free(task_param); // Free allocated memory in case of failure
        }
    }
    vTaskStartScheduler();
}
