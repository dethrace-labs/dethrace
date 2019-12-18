TARGET_EXEC ?= c1

BUILD_DIR ?= ./build
SRC_DIR ?= .

SRCS := $(shell find $(SRC_DIR) -name "*.c")
OBJS := $(SRCS:%=$(BUILD_DIR)/%.o)
DEPS := $(OBJS:.o=.d)

INC_DIRS := $(shell find $(SRC_DIR) -type d)
INC_FLAGS := $(addprefix -I,$(INC_DIRS))

CFLAGS ?= $(INC_FLAGS) -g -Wno-return-type -Wno-missing-declarations -Werror=implicit-function-declaration

.PHONY: clean build

build: $(OBJS)

# c source
$(BUILD_DIR)/%.c.o: %.c
	@$(MKDIR_P) $(dir $@)
	@$(CC) $(CFLAGS) -c $< -o $@

clean:
	@$(RM) -r $(BUILD_DIR)

-include $(DEPS)

MKDIR_P ?= mkdir -p
