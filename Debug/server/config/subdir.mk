################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../server/config/config.c \
../server/config/config_audio.c \
../server/config/config_kernel.c \
../server/config/config_miio.c \
../server/config/config_miss.c \
../server/config/config_player.c \
../server/config/config_recorder.c \
../server/config/config_video.c \
../server/config/rwio.c 

OBJS += \
./server/config/config.o \
./server/config/config_audio.o \
./server/config/config_kernel.o \
./server/config/config_miio.o \
./server/config/config_miss.o \
./server/config/config_player.o \
./server/config/config_recorder.o \
./server/config/config_video.o \
./server/config/rwio.o 

C_DEPS += \
./server/config/config.d \
./server/config/config_audio.d \
./server/config/config_kernel.d \
./server/config/config_miio.d \
./server/config/config_miss.d \
./server/config/config_player.d \
./server/config/config_recorder.d \
./server/config/config_video.d \
./server/config/rwio.d 


# Each subdirectory must supply rules for building sources it contributes
server/config/%.o: ../server/config/%.c
	@echo 'Building file: $<'
	@echo 'Invoking: Cross GCC Compiler'
	rsdk-linux-gcc -DVIDEO -DAUDIO -I/home/ning/library-mips/cJSON/include -I/home/ning/library-mips/mp4v2/include -I/home/ning/library-mips/freetype2/include -I/home/ning/library-mips/mi/include -I/home/ning/library-mips/realtek/include -I/home/ning/library-mips/json-c/include -O0 -g3 -Wall -c -fmessage-length=0 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@)" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


