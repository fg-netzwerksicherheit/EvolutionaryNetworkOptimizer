USER_OBJS :=

CPP_MAINFILES := ../src/main.cpp

CPP_SRCS := $(shell find ../src -name "*.cpp" -not -path "../src/test/*")
CPP_SRCS := $(filter-out $(CPP_MAINFILES), $(CPP_SRCS))
OBJS := $(CPP_SRCS:../%.cpp=./%.o)
CPP_DEPS := $(shell find . -name "*.d")
SUB_DIRS := $(shell cd .. && find ./src -type d -not -path "./src/test" -not -path "./src/test/*")

TARGET := libNetworkOptimizer-debug.so

# dependencies
INCLUDE_PATH := -I"../../Simulation/src/" -I"../../AnyGraph/src/"
LIB_PATH := -L/usr/local/lib
LIBS := -lSimulation-debug -lns3.26-core-debug -lboost_unit_test_framework

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
	g++ -std=c++1y -O0 -g3 -Wall -c -fmessage-length=0 -fPIC -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@)" -o "$@" "$<" $(INCLUDE_PATH)
	@echo 'Finished building: $<'
	@echo ' '


# Linker
$(TARGET): $(SUB_DIRS) $(OBJS) $(USER_OBJS)
	@echo 'Building target: $@'
	@echo 'Invoking: GCC C++ Linker'
	g++ -shared $(LIB_PATH) -o $@ $(OBJS) $(USER_OBJS) $(LIBS)
	@echo 'Finished building target: $@'
	@echo ' '

# Other Targets
clean:
	-$(RM) $(CPP_DEPS)$(OBJS) $(TARGET)
	-@echo ' '

.PHONY: all clean dependents
.SECONDARY:

