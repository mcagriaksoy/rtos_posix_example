// Mehmet Cagri Aksoy
// RTOS Example for Audivo GmbH
// 21.11.2023

#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#include "FreeRTOS.h"
#include "queue.h"
#include "semphr.h"
#include "task.h"

#define ELEMENT_SIZE (200)    // 200 elements
#define TASK_DELAY_TIME (750) // milliseconds

static QueueHandle_t pos_queue = NULL;
static SemaphoreHandle_t mutex;

void producer(void *pvParameters)
{
    uint32_t *ptr = (uint32_t *)pvParameters;
    while (1)
    {
        // Task 2.3 The producer should randomly generate a number and
        // write to the allocated memory(position of memory also randomly).
        uint32_t random_number = rand() % ELEMENT_SIZE;
        uint32_t random_position = rand() % ELEMENT_SIZE;

        if (xSemaphoreTake(mutex, 0) == pdTRUE)
        {
            ptr[random_position] = random_number;
            xSemaphoreGive(mutex);
        }
        else
        {
            printf("[Producer Thread] Mutex is not available.\r\n");
        }

        // Task 2.4 The producer should print out the position and number.
        printf("[Producer Thread] Position: %d, Number: %d\r\n", random_position, random_number);

        // Task 2.5 The producer signals the position he wrote to consumer by
        // some kind of inter-thread communication. (I prefer Queue in this scenario)
        if (xQueueSend(pos_queue, &random_position, 0) != pdPASS)
        {
            printf("[Producer Thread] Queue is full.\r\n");
        }

        vTaskDelay(pdMS_TO_TICKS(TASK_DELAY_TIME));
    }
}

void consumer(void *pvParameters)
{
    uint32_t *ptr = (uint32_t *)pvParameters;
    while (1)
    {
        // Task 2.6 The consumer should start to read the position which was changed by the producer.
        uint32_t written_position;
        if (xQueueReceive(pos_queue, (void *)&written_position, 0) != pdPASS)
        {
            printf("[Consumer Thread] Queue is empty.\r\n");
        }
        else
        {

            // Task 2.7 The consumer should print out the position and number.
            printf("[Consumer Thread] Position: %d, Number: %d\r\n", written_position, ptr[written_position]);

            if (xSemaphoreTake(mutex, 0) == pdTRUE)
            {
                // Task 2.8 The consumer should clean (set 0) to the position, which was read
                ptr[written_position] = 0;

                // Task 2.9 The consumer should notice the producer, that operation is finished,
                // and the producer should continue to write (at Task 2.2)
                xSemaphoreGive(mutex);
            }
            else
            {
                printf("[Consumer Thread] Mutex is not available.\r\n");
            }
        }

        vTaskDelay(pdMS_TO_TICKS(TASK_DELAY_TIME));
    }
}

int main(void)
{
    srand(time(NULL));
    // Task 2.1 Allocate memory for 200 elements, type: uint32_t
    uint32_t *ptr = (uint32_t *)pvPortMalloc(ELEMENT_SIZE * sizeof(uint32_t));
    if (ptr == NULL)
    {
        printf("[Main] Memory allocation failed.\r\n");
        return -1;
    }

    // Create a mutex for mutual exclusion.
    mutex = xSemaphoreCreateMutex();

    // Task 2.2 Create two threads (producer and consumer).
    if (xTaskCreate(&producer, "Producer thread", 1024, &ptr, 1, NULL) != pdPASS)
    {
        printf("[Main] Producer thread creation failed.\r\n");
        return -1;
    }

    if (xTaskCreate(&consumer, "Consumer thread", 1024, &ptr, 1, NULL) != pdPASS)
    {
        printf("[Main] Consumer thread creation failed.\r\n");
        return -1;
    }

    pos_queue = xQueueCreate(ELEMENT_SIZE, sizeof(uint32_t));
    if (pos_queue == NULL)
    {
        printf("[Main] Queue creation failed.\r\n");
        return -1;
    }

    vTaskStartScheduler();
    return 0;
}