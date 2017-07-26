USER_OBJS :=

CPP_SRCS := $(shell find ../src -name "*.cpp")
OBJS := $(CPP_SRCS:../%.cpp=./%.o)
CPP_DEPS := $(shell find . -name "*.d")
SUB_DIRS := $(shell cd .. && find ./src -type d)

TARGET = AnyGraph-debug

LIB_PATH := -L/usr/local/lib
LIBS := -ltinyxml2 -ljsoncpp -lboost_unit_test_framework

# Include all dependency files
ifneq ($(strip $(CPP_DEPS)),)
-include $(CPP_DEPS)
endif

test:
	@echo $(CPP_SRCS)
	@echo $(OBJS)
	@echo $(CPP_DEPS)
	@echo $(SUB_DIRS)

# All Target
all: $(TARGET)

# Sub directory
$(SUB_DIRS):
	@echo 'Creating directory: $@'
	@-mkdir $(@)
	@echo ' '

# Compiler
$(OBJS): ./%.o: ../%.cpp
	@echo 'Building file: $<'
	@echo 'Invoking: GCC C++ Compiler'
	g++ -std=c++0x -O0 -g3 -Wall -c -fmessage-length=0 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@)" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


# Linker
$(TARGET): $(SUB_DIRS) $(OBJS) $(USER_OBJS)
	@echo 'Building target: $@'
	@echo 'Invoking: GCC C++ Linker'
	g++ $(LIB_PATH) -o $@ $(OBJS) $(USER_OBJS) $(LIBS)
	@echo 'Finished building target: $@'
	@echo ' '
	
# Other Targets
clean:
	-$(RM) $(CPP_DEPS)$(OBJS) $(TARGET)
	-@echo ' '

.PHONY: all clean dependents
.SECONDARY:
