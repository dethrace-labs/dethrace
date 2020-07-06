TARGET_EXEC ?= c1tests

BUILD_DIR ?= ./build
SRC_DIR ?= .
DR_SRC_DIR ?= ../src/DETHRACE
BR_SRC_DIR ?= ../src/BRSRC13
HARNESS_SRC_DIR ?= ../src/harness

OS = $(shell uname -s)
SRCS := $(shell find $(SRC_DIR) -name "*.c")
OBJS := $(SRCS:%=$(BUILD_DIR)/%.o)
OBJS += $(shell find $(BR_SRC_DIR) -name "*.o")
OBJS += $(shell find $(HARNESS_SRC_DIR) -name "*.o")
# include all DR objects except the main method to avoid _main symbol collision
OBJS += $(shell find $(DR_SRC_DIR) -name *.o | grep -v build/main.c.o)

DEPS := $(OBJS:.o=.d)

INC_DIRS := $(shell find $(SRC_DIR) -type d) $(BR_SRC_DIR) $(DR_SRC_DIR) $(HARNESS_SRC_DIR)
INC_FLAGS := $(addprefix -I,$(INC_DIRS))

CFLAGS ?= $(INC_FLAGS) -g -Wno-missing-declarations -Werror=implicit-function-declaration
SDL_LDFLAGS = `sdl2-config --libs`
ifeq ($(OS),Darwin)
	OPENGL_LDFLAGS = -framework OpenGL
else
	OPENGL_LDFLAGS = -lGL
endif
LDFLAGS ?= -lm -fno-pie $(SDL_LDFLAGS) $(OPENGL_LDFLAGS)

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
