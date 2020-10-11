################################################################################
# Automatically-generated file. Do not edit!
################################################################################

-include ../makefile.init

RM := rm -rf

# All of the sources participating in the build are defined here
-include sources.mk
-include tools/json/subdir.mk
-include tools/cJSON/subdir.mk
-include tools/buffer/subdir.mk
-include tools/subdir.mk
-include server/video/subdir.mk
-include server/recorder/subdir.mk
-include server/realtek/subdir.mk
-include server/miss/subdir.mk
-include server/miio/subdir.mk
-include server/config/subdir.mk
-include server/audio/subdir.mk
-include manager/subdir.mk
-include subdir.mk
-include objects.mk

ifneq ($(MAKECMDGOALS),clean)
ifneq ($(strip $(C_DEPS)),)
-include $(C_DEPS)
endif
endif

-include ../makefile.defs

# Add inputs and outputs from these tool invocations to the build variables 

# All Target
all: webcam

# Tool invocations
webcam: $(OBJS) $(USER_OBJS)
	@echo 'Building target: $@'
	@echo 'Invoking: Cross GCC Linker'
	rsdk-linux-gcc -L/home/ning/library-mips/cJSON/lib -L/home/ning/library-mips/mp4v2/lib -L/home/ning/library-mips/sodium/lib -L/home/ning/library-mips/freetype2/lib -L/home/ning/library-mips/json-c/lib -L/home/ning/library-mips/mi/lib -L/home/ning/library-mips/realtek/lib -o "webcam" $(OBJS) $(USER_OBJS) $(LIBS)
	@echo 'Finished building target: $@'
	@echo ' '
	$(MAKE) --no-print-directory post-build

# Other Targets
clean:
	-$(RM) $(EXECUTABLES)$(OBJS)$(C_DEPS) webcam
	-@echo ' '

post-build:
	-cp -f webcam /home/ning/nfs-root/bin/webcam
	-@echo ' '

.PHONY: all clean dependents post-build

-include ../makefile.targets
