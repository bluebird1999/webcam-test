################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../main/global.c \
../main/main.c \
../main/manager.c \
../main/timer.c 

OBJS += \
./main/global.o \
./main/main.o \
./main/manager.o \
./main/timer.o 

C_DEPS += \
./main/global.d \
./main/main.d \
./main/manager.d \
./main/timer.d 


# Each subdirectory must supply rules for building sources it contributes
main/%.o: ../main/%.c
	@echo 'Building file: $<'
	@echo 'Invoking: Cross GCC Compiler'
	rsdk-linux-gcc -DVIDEO -DAUDIO -I/home/ning/library-mips/cJSON/include -I/home/ning/library-mips/freetype2/include -I/home/ning/library-mips/mi/include -I/home/ning/library-mips/realtek/include -I/home/ning/library-mips/json-c/include -O0 -g3 -Wall -c -fmessage-length=0 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@)" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


