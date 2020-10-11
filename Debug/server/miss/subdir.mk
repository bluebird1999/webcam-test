################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../server/miss/miss.c \
../server/miss/miss_porting.c \
../server/miss/miss_session.c \
../server/miss/miss_session_list.c 

OBJS += \
./server/miss/miss.o \
./server/miss/miss_porting.o \
./server/miss/miss_session.o \
./server/miss/miss_session_list.o 

C_DEPS += \
./server/miss/miss.d \
./server/miss/miss_porting.d \
./server/miss/miss_session.d \
./server/miss/miss_session_list.d 


# Each subdirectory must supply rules for building sources it contributes
server/miss/%.o: ../server/miss/%.c
	@echo 'Building file: $<'
	@echo 'Invoking: Cross GCC Compiler'
	rsdk-linux-gcc -DVIDEO -DAUDIO -I/home/ning/library-mips/cJSON/include -I/home/ning/library-mips/mp4v2/include -I/home/ning/library-mips/freetype2/include -I/home/ning/library-mips/mi/include -I/home/ning/library-mips/realtek/include -I/home/ning/library-mips/json-c/include -O0 -g3 -Wall -c -fmessage-length=0 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@)" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


