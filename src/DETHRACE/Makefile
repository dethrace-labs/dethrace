TARGET_EXEC ?= c1

BUILD_DIR ?= ./build
SRC_DIR ?= .
BR_SRC_DIR ?= ../BRSRC13

SRCS := $(shell find $(SRC_DIR) -name *.c)
OBJS := $(SRCS:%=$(BUILD_DIR)/%.o) $(shell find $(BR_SRC_DIR) -name *.o)
DEPS := $(OBJS:.o=.d)

INC_DIRS := $(shell find $(SRC_DIR) -type d) ../BRSRC13
INC_FLAGS := $(addprefix -I,$(INC_DIRS))

CFLAGS ?= $(INC_FLAGS) -Wno-return-type -Wno-missing-declarations -Werror=implicit-function-declaration

.PHONY: clean build test

build: $(BUILD_DIR)/$(TARGET_EXEC)

brender: $(BR_OBJS)
	$(MAKE) -C BRSRC13

$(BUILD_DIR)/$(TARGET_EXEC): $(OBJS)
	@$(CC) $(OBJS) -o $@ $(LDFLAGS)

# c source
$(BUILD_DIR)/%.c.o: %.c
	@$(MKDIR_P) $(dir $@)
	@$(CC) $(CFLAGS) -c $< -o $@

clean:
	$(RM) -r $(BUILD_DIR)

test: build
	$(BUILD_DIR)/$(TARGET_EXEC) -test


-include $(DEPS)

MKDIR_P ?= mkdir -p
