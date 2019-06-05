################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../src/debug_core.c \
../src/stdafx.c 

OBJS += \
./src/debug_core.o \
./src/stdafx.o 

C_DEPS += \
./src/debug_core.d \
./src/stdafx.d 


# Each subdirectory must supply rules for building sources it contributes
src/%.o: ../src/%.c
	@echo 'Building file: $<'
	@echo 'Invoking: GCC C Compiler'
	gcc -DDEBUG -I"/home/bhart/git/api_core/api_core" -I"/home/bhart/git/common_core/common_core" -I"/home/bhart/git/debug_core/debug_core/include" -O0 -g3 -Wall -c -fmessage-length=0 -fPIC -pthread -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@)" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


