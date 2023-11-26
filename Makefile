# Created by: M. Cagri Aksoy
# Date: 21.11.2023
# Description: Makefile

# RTOS path
RTOS_PATH         = /home/mcagriaksoy/FreeRTOSv202212.01/FreeRTOS
RTOS_PLUS_PATH    = /home/mcagriaksoy/FreeRTOSv202212.01/FreeRTOS-Plus
# .o directory
ODIR            = build
SOURCEDIR	   = $(CURDIR)/src

# Source VPATHS
VPATH           += $(RTOS_PATH)/Source
VPATH	        += $(RTOS_PATH)/Source/portable/MemMang
VPATH	        += $(RTOS_PATH)/Source/portable/ThirdParty/GCC/Posix
VPATH           += $(RTOS_PATH)/Source/portable/ThirdParty/GCC/Posix/utils
VPATH           += $(RTOS_PATH)/Demo/Common
VPATH			+= $(RTOS_PATH)/Project/FileIO
VPATH			+= $(RTOS_PATH)/Project

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

# Our Objects
C_FILES			+= $(SOURCEDIR)/example.c
C_FILES			+= $(SOURCEDIR)/deadlock_example.c

# Include Paths
INCLUDES        += -I$(RTOS_PATH)/Source/include
INCLUDES        += -I$(RTOS_PATH)/Source/portable/ThirdParty/GCC/Posix
INCLUDES        += -I$(RTOS_PATH)/Source/portable/ThirdParty/GCC/Posix/utils
INCLUDES        += -I$(RTOS_PATH)/Project
#INCLUDES        += -I/usr/include/x86_64-linux-gnu/
INCLUDES        += -I/usr/include/
INCLUDES        += -I $(CURDIR)/include

# Generate OBJS names
OBJS = $(patsubst %.c,%.o,$(C_FILES))

######## C Flags ########

# Warnings
CFLAGS = -I -Wall -Wextra -O3 -DOPENSSL_SUPPRESS_DEPRECATED
# Compiler flags
CFLAGS += -m32
CFLAGS += -DDEBUG=1
#CFLAGS += -g -DUSE_STDIO=1 -D__GCC_POSIX__=1
CFLAGS += -g -UUSE_STDIO -D__GCC_POSIX__=1
ifneq ($(shell uname), Darwin)
CFLAGS += -pthread
endif


# Rules
.PHONY : all
all: deadlock_example


# Fix to place .o files in ODIR
_OBJS = $(patsubst %,$(ODIR)/%,$(OBJS))

$(ODIR)/%.o: %.c
	@mkdir -p $(dir $@)
	@echo ">> Compiling $<"
	$(CC) $(CFLAGS) $(INCLUDES) -c -o $@ $<


deadlock_example: $(_OBJS)
	@echo ">> Linking $@..."
	@$(CC) $(CFLAGS) $(INCLUDES) $^ $(LINKFLAGS) $(LIBS) -o $@

	@echo "-------------------------"
	@echo "BUILD COMPLETE: $@"
	@echo "-------------------------"

.PHONY : clean
clean:
	@-rm -rf $(ODIR) example
	@-rm -rf $(ODIR) deadlock_example

	@echo "--------------"
	@echo "CLEAN COMPLETE"
	@echo "--------------"
