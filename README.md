The example implementation of Freertos which runs on Linux based OS.

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

## More information at:

https://www.freertos.org/FreeRTOS-simulator-for-Linux.html
