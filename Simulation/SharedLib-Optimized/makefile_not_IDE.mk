USER_OBJS :=

CPP_SRCS := $(shell find ../src -name "*.cpp" -not -path "../src/test/*")
OBJS := $(CPP_SRCS:../%.cpp=./%.o)
CPP_DEPS := $(shell find . -name "*.d")
SUB_DIRS := $(shell cd .. && find ./src -type d -not -path "./src/test" -not -path "./src/test/*")

TARGET := libSimulation-optimized.so

# dependencies
INCLUDE_PATH := -I"../../AnyGraph/src/"
LIB_PATH := -L/usr/local/lib
LIBS := -lAnyGraph-optimized -lboost_unit_test_framework -lns3.26-core-optimized -lns3.26-internet-optimized -lns3.26-applications-optimized -lns3.26-point-to-point-optimized -lns3.26-network-optimized -lns3.26-wifi-optimized -lns3.26-mobility-optimized -lns3.26-nix-vector-routing-optimized

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
	g++ -std=c++0x -O3 -Wall -c -fmessage-length=0 -fPIC -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@)" -o "$@" "$<" $(INCLUDE_PATH)
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

