################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../server/audio/audio.c 

OBJS += \
./server/audio/audio.o 

C_DEPS += \
./server/audio/audio.d 


# Each subdirectory must supply rules for building sources it contributes
server/audio/%.o: ../server/audio/%.c
	@echo 'Building file: $<'
	@echo 'Invoking: Cross GCC Compiler'
	rsdk-linux-gcc -DVIDEO -DAUDIO -I/home/ning/library-mips/cJSON/include -I/home/ning/library-mips/mp4v2/include -I/home/ning/library-mips/freetype2/include -I/home/ning/library-mips/mi/include -I/home/ning/library-mips/realtek/include -I/home/ning/library-mips/json-c/include -O0 -g3 -Wall -c -fmessage-length=0 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@)" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


