TARGET_EXEC ?= erapGBC

BUILD_DIR ?= build
SRC_DIRS ?= ERapGBC

SRCS := $(shell find $(SRC_DIRS) -name *.cpp -or -name *.c -or -name *.s)
OBJS := $(SRCS:%=$(BUILD_DIR)/%.o)
DEPS := $(OBJS:.o=.d)

INC_DIRS := $(SRC_DIRS)
INC_FLAGS := $(addprefix -I,$(INC_DIRS))

DEFS ?= -DLINUX
CPPFLAGS ?= $(INC_FLAGS) -MMD
LDFLAGS ?= -lsfml-graphics -lsfml-window -lsfml-system

$(BUILD_DIR)/$(TARGET_EXEC): $(OBJS)
	$(CXX) $(DEFS) $(OBJS) -o $@ $(LDFLAGS)

# assembly
$(BUILD_DIR)/%.s.o: %.s
	$(MKDIR_P) $(dir $@)
	$(AS) $(DEFS) $(ASFLAGS) -c $< -o $@

# c source
$(BUILD_DIR)/%.c.o: %.c
	$(MKDIR_P) $(dir $@)
	$(CC) $(DEFS) $(CPPFLAGS) $(CFLAGS) -c $< -o $@

# c++ source
$(BUILD_DIR)/%.cpp.o: %.cpp
	$(MKDIR_P) $(dir $@)
	$(CXX) $(DEFS) $(CPPFLAGS) $(CXXFLAGS) -c $< -o $@


.PHONY: clean

clean:
	$(RM) -r $(BUILD_DIR)

-include $(DEPS)

MKDIR_P ?= mkdir -p