################################################################################
# Automatically-generated file. Do not edit!
################################################################################

-include ../makefile.init

RM := rm -rf

# All of the sources participating in the build are defined here
-include sources.mk
-include src/subdir.mk
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
all: libdebug_core.so

dependents:
	-cd /home/bhart/git/common_core/common_core/Debug && $(MAKE) all
	-cd /home/bhart/git/api_core/api_core/Debug && $(MAKE) all

# Tool invocations
libdebug_core.so: $(OBJS) $(USER_OBJS) /home/bhart/git/common_core/common_core/Debug/libcommon_core.so /home/bhart/git/api_core/api_core/Debug/libapi_core.so
	@echo 'Building target: $@'
	@echo 'Invoking: GCC C Linker'
	gcc -L"/home/bhart/git/api_core/api_core/Debug" -L"/home/bhart/git/common_core/common_core/Debug" -shared -o "libdebug_core.so" $(OBJS) $(USER_OBJS) $(LIBS)
	@echo 'Finished building target: $@'
	@echo ' '
	$(MAKE) --no-print-directory post-build

# Other Targets
clean:
	-$(RM) $(LIBRARIES)$(OBJS)$(C_DEPS) libdebug_core.so
	-@echo ' '

post-build:
	-cp -f libdebug_core.so "/home/bhart/lib/Debug"; rm -rf /home/bhart/git/debug_core/docs; doxygen /home/bhart/git/debug_core/debug_core/docs/Doxyfile
	-@echo ' '

.PHONY: all clean dependents post-build
/home/bhart/git/common_core/common_core/Debug/libcommon_core.so:
/home/bhart/git/api_core/api_core/Debug/libapi_core.so:

-include ../makefile.targets
