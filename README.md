<a href="https://github.com/mcagriaksoy/rtos_posix_example" title="Go to GitHub repo"><img src="https://img.shields.io/static/v1?label=mcagriaksoy&message=rtos_posix_example&color=blue&logo=github" alt="mcagriaksoy - rtos_posix_example"></a>
<a href="#license"><img src="https://img.shields.io/badge/License-MIT-blue" alt="License"></a>
<a href="https://github.com/mcagriaksoy/rtos_posix_example/issues"><img src="https://img.shields.io/github/issues/mcagriaksoy/rtos_posix_example" alt="issues - rtos_posix_example"></a>
[![OS - Linux](https://img.shields.io/badge/OS-Linux-blue?logo=linux&logoColor=white)](https://www.linux.org/ "Go to Linux homepage")

# Introduction

In this repository, I would like to explain how to use freertos on posix based OS. (Linux distros for example).
I want to answer some questions before explaining the code and configuration.

## What is freertos?

FreeRTOS is a real-time operating system kernel for embedded devices. It's designed to be small and simple, making it ideal for constrained devices such as microcontrollers.

FreeRTOS provides the core real-time scheduling functionality, inter-task communication, timing and synchronization primitives. It's highly configurable, allowing developers to include only the features they need.

Despite its small size, FreeRTOS supports multiple architectures and development environments, making it a versatile choice for a wide range of projects. It's also provided under a modified GPL license that allows its use in commercial applications.

## Why is so popular?

Portability, Small Footprint, Configurability, Active community and great documentation, and the last but not the least one, the license!

## It is RTOS but how can I use it on non-rtos environment?

Yes, there are some ports for Windows OS, or Linux, Unix distros to use RTOS. You can test your code before really use that on real hardware.

Remember, even though you're using FreeRTOS in a non-real-time environment, it's still a multitasking system. Be aware of potential issues such as task priority inversion, deadlock, and race conditions.

## I know Windows, MacOS, Linux. But what is posix?

POSIX stands for Portable Operating System Interface. It's a family of standards specified by the IEEE for maintaining compatibility between operating systems. POSIX defines the application programming interface (API), along with command line shells and utility interfaces, for software compatibility with variants of Unix and other operating systems.

`The main aim of POSIX is to provide a standard way of using operating system functions. This allows software developers to write applications that can be ported to various operating systems without having to modify the code.`

POSIX standards include definitions for system functions, system calls, libraries, and interfaces, including those for file handling, process management, inter-process communication (IPC), and more.

POSIX-compliant operating systems include Unix, Linux, and macOS, among others. Windows is not POSIX supported OS.

# Let's start:

First of all, we need to download the freertos code from its repository.

Repo URL : https://github.com/FreeRTOS/FreeRTOS
.zip URL : https://github.com/FreeRTOS/FreeRTOS/releases

- Please download the latest LTS release and unzip it to the location you want.

# Step - 2

We need to create a makefile for our project and show the required freertos file locations.

The file `Makefile` can be found the repository main location.

Please do not forget to add the Posix related source files to the path.

```
INCLUDES        += -I$(RTOS_PATH)/Source/include
INCLUDES        += -I$(RTOS_PATH)/Source/portable/ThirdParty/GCC/Posix
INCLUDES        += -I$(RTOS_PATH)/Source/portable/ThirdParty/GCC/Posix/utils
```

Also, do not forget to add following compiler flag:

```
CFLAGS += -m32
```

The -m32 compiler flag instructs the GCC (GNU Compiler Collection) to generate code that will run on a 32-bit environment.

When you use this flag, the compiler will produce code that is compatible with the i386 (or later) architecture. This is useful if you're working on a 64-bit system but need to compile your code for a 32-bit system.

If you want to use or test the multithreading feature of code:

```
CFLAGS += -pthread
```

If your system is not MacOS, also you need to add the pthread feature.
The -pthread option tells the GCC compiler to add support for multithreading with the pthreads library. This is necessary for programs that use functions from the pthreads library to create and manage threads.

If you're compiling a program that uses pthreads, you should include -pthread in your CFLAGS. This will ensure that the pthreads library is correctly linked during the compilation process.

Also another important point is adding the rtos related .c files to the project makefile.

```
# FreeRTOS Objects
C_FILES			= croutine.c \
				event_groups.c \
				list.c \
				queue.c \
				tasks.c \
				timers.c \
# portable Objects
C_FILES			+= heap_3.c \
				port.c \
				wait_for_event.c \
```

For example in my code, I used heap_3, port, queue, croutine, event_groups features. Whichever you want, you can include the .c files.
The files are located under: FreeRTOS_VERSION_YOU_DOWNLOADED/FreeRTOS/Source/\*.c

# Step - 3

To create working program, we need a `FreeRTOSConfig.h` file to define some missing definitions. You can find the file on include path. The config file is environment dependent and changes on different hardwares and so on.

Addiationally, we need to call the header `#include "FreeRTOS.h"` which is in downloaded rtos folder. Also if you want to use queue for example, you need to call `queue.h` as well.

# Step - 4

Unlike C, task definition must be made in rtos. Tasks are also called threads in rtos world. If we want to create two task, we need to create the functions below:

```
void task1(void *pvParameters);
void task2(void *pvParameters);
```

It returns void and takes a void pointer parameter, you do not have to pass the parameter to the task but, for example if you have allocated memory and you want to use it on different task (thread) you can use the `pvParameters` parameter.

But the creation of thread not enough. We call the functions via:

```
xTaskCreate(&task1, "thread1", 1024, NULL, 1, NULL)
xTaskCreate(&task2, "thread2", 1024, NULL, 1, NULL)
```

&task1: This is a pointer to the task function that will be executed when the task is scheduled. The function must have the prototype void vTaskFunction( void \* pvParameters ).

"thread1": This is a descriptive name for the task, mainly used for debugging purposes. The kernel does not use this name itself.

1024: This is the stack depth for the task. The size is in words, not bytes, so on a 32-bit system, 1024 would allocate 4096 bytes (1024 words \* 4 bytes/word).

NULL: This is a pointer that will be passed as the parameter to the task function. If you don't need to pass a parameter, you can pass NULL.

1: This is the priority of the task. In FreeRTOS, higher numbers denote higher priority. The maximum priority that can be assigned is defined by configMAX_PRIORITIES in FreeRTOSConfig.h.

NULL: This is a pointer to a variable that will hold the task's handle. This can be used to reference the task in API calls. If you don't need the task handle, you can pass NULL.

Okay, we created tasks but! We need to run these as well via:

`vTaskStartScheduler();`
That's it!

## Data operations on RTOS

### How can we allocate memory?

`uint32_t *ptr = (uint32_t *)pvPortMalloc(ELEMENT_SIZE * sizeof(uint32_t));`
This line of code is allocating dynamic memory in FreeRTOS. Here's a breakdown of what it does:

uint32_t \*ptr: This declares a pointer to a uint32_t (an unsigned 32-bit integer). This pointer will be used to access the memory that's allocated.

pvPortMalloc(ELEMENT_SIZE \* sizeof(uint32_t)): This calls the pvPortMalloc function, which is the FreeRTOS equivalent of the standard C malloc function. It allocates a block of memory from the FreeRTOS heap.

ELEMENT_SIZE \* sizeof(uint32_t): This is the size of the memory block to allocate. It's calculated as the number of elements (ELEMENT_SIZE) times the size of each element (sizeof(uint32_t)). This will allocate enough memory to hold ELEMENT_SIZE number of uint32_t elements.

(uint32_t \*): This is a cast that tells the compiler to treat the returned pointer as a pointer to uint32_t. This is necessary because pvPortMalloc returns a void pointer, which can't be directly assigned to ptr without a cast.

So, in summary, this line of code is allocating a block of memory large enough to hold ELEMENT_SIZE number of uint32_t elements, and assigning the address of this memory block to ptr.

## How can I pass the data between tasks?

In FreeRTOS, there are several ways to pass data between tasks. It is also called inter-task or inter-thread communication ways.

### Queues:

Queues are a common way to send data from one task to another. You can create a queue with xQueueCreate(), send data to the queue with xQueueSend() or xQueueSendToFront(), and receive data from the queue with xQueueReceive().

```
QueueHandle_t myQueue = xQueueCreate(ELEMENT_SIZE, sizeof(int));
int valueToSend = 123;
xQueueSend(myQueue, &valueToSend, portMAX_DELAY);
```

### Task Notifications:

Task notifications are a lightweight and fast method to send data or events from one task to another. You can send a notification with xTaskNotify() and receive a notification with xTaskNotifyWait().

```
TaskHandle_t myTaskHandle = NULL; // Assume this is initialized with a task handle
uint32_t valueToSend = 123;
xTaskNotify(myTaskHandle, valueToSend, eSetValueWithOverwrite);
```

### Shared Memory:

Tasks can share data by accessing shared memory. However, you must use a mutex or a semaphore to ensure that tasks do not access the shared memory simultaneously, which could lead to data inconsistency or corruption.
Remember, when passing data between tasks, you must consider synchronization and data consistency issues. Always protect shared resources and ensure that tasks do not overwrite each other's data.

```
SemaphoreHandle_t myMutex = xSemaphoreCreateMutex(); // Create a mutex
int sharedVariable = 0; // This variable is shared between tasks

// In a task
xSemaphoreTake(myMutex, portMAX_DELAY); // Take the mutex before accessing shared memory
sharedVariable = 123; // Modify the shared variable
xSemaphoreGive(myMutex); // Give the mutex after accessing shared memory
```

Note: portMAX_DELAY is a constant in FreeRTOS that represents the maximum delay time that a task can block for. It's often used as a parameter in FreeRTOS API functions that have a timeout or delay.

#### What is difference between mutex and semaphore?

We use a mutex when you need to protect a single resource from concurrent access, and use a semaphore when you need to control access to a pool of resources or to synchronize tasks.

## More information at:

https://www.freertos.org/FreeRTOS-simulator-for-Linux.html
https://www.freertos.org/FreeRTOS-simulator-for-Linux.html#build_instructions_1
