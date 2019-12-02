TARGET_EXEC ?= c1tests

BUILD_DIR ?= ./build
SRC_DIR ?= .
DR_SRC_DIR ?= ../src/DETHRACE
BR_SRC_DIR ?= ../src/BRSRC13

SRCS := $(shell find $(SRC_DIR) -name "*.c")
OBJS := $(SRCS:%=$(BUILD_DIR)/%.o)
OBJS += $(shell find $(BR_SRC_DIR) -name "*.o")
# include all DR objects except the main method to avoid _main symbol collision
OBJS += $(shell find $(DR_SRC_DIR) -name *.o | grep -v pc-dos/main.c.o)

DEPS := $(OBJS:.o=.d)

INC_DIRS := $(shell find $(SRC_DIR) -type d) $(BR_SRC_DIR) $(DR_SRC_DIR)
INC_FLAGS := $(addprefix -I,$(INC_DIRS))

CFLAGS ?= $(INC_FLAGS)
LDFLAGS ?= -lm

.PHONY: clean build run

build: $(BUILD_DIR)/$(TARGET_EXEC)

$(BUILD_DIR)/$(TARGET_EXEC): $(OBJS)
	@$(CC) $(OBJS) -o $@ $(LDFLAGS)

# c source
$(BUILD_DIR)/%.c.o: %.c
	@$(MKDIR_P) $(dir $@)
	@$(CC) $(CFLAGS) -c $< -o $@

clean:
	@$(RM) -r $(BUILD_DIR)

-include $(DEPS)

MKDIR_P ?= mkdir -p
