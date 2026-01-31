# =========================
# Compiler
# =========================
CXX := g++
UNAME_S := $(shell uname -s)

# =========================
# Directories
# =========================
BUILD_DIR := build
SRC_DIR   := .

# =========================
# Sources
# =========================
SRCS := $(wildcard $(SRC_DIR)/*.cxx)
OBJS := $(patsubst %.cxx,$(BUILD_DIR)/%.o,$(notdir $(SRCS)))

# =========================
# Executables
# =========================
MAC_EXEC   := $(BUILD_DIR)/main_mac
LINUX_EXEC := $(BUILD_DIR)/main_linux

# =========================
# Vulkan SDK (macOS)
# =========================
VULKAN_SDK := /home/divyansh-undley/SDKs/vulkansdk-linux-x86_64-1.4.335.0/1.4.335.0/




# =========================
# Flags
# =========================
COMMON_CXXFLAGS := -std=c++2b -Wall -g -O1 \
	-fsanitize=address -fno-omit-frame-pointer \
	-I/usr/include

MAC_CXXFLAGS := $(COMMON_CXXFLAGS) \
	-I$(VULKAN_SDK)/macOS/include

LINUX_CXXFLAGS := $(COMMON_CXXFLAGS)

COMMON_LDFLAGS := -L/opt/homebrew/lib -L./lib -lglfw

MAC_LDFLAGS := $(COMMON_LDFLAGS) \
	-L$(VULKAN_SDK)/macOS/lib \
	-lvulkan -lMoltenVK \
	-framework Cocoa \
	-framework IOKit \
	-framework CoreFoundation \
	-framework QuartzCore \
	-framework Metal \
	-Wl,-rpath,$(VULKAN_SDK)/macOS/lib

LINUX_LDFLAGS := $(COMMON_LDFLAGS) -lvulkan

# =========================
# Colors
# =========================
YELLOW := \033[1;33m
GREEN  := \033[1;32m
RED    := \033[1;31m
BGMAGENTA := \033[45m
RESET  := \033[0m

# =========================
# Targets
# =========================
.PHONY: all clean

all: $(BUILD_DIR)
ifeq ($(UNAME_S),Darwin)
	@echo -e "$(YELLOW)Building for macOS...$(RESET)"
	$(MAKE) $(MAC_EXEC)
	@echo -e "$(GREEN)Running $(BGMAGENTA)$(MAC_EXEC)$(RESET)"
	./$(MAC_EXEC)
else
	@echo -e "$(YELLOW)Building for Linux...$(RESET)"
	$(MAKE) $(LINUX_EXEC)
endif

# =========================
# Build rules
# =========================
$(MAC_EXEC): $(OBJS)
	@echo -e "$(GREEN)Linking $(BGMAGENTA)$@$(RESET)"
	$(CXX) $(OBJS) $(MAC_CXXFLAGS) $(MAC_LDFLAGS) -o $@

$(LINUX_EXEC): $(OBJS)
	@echo -e "$(GREEN)Linking $(BGMAGENTA)$@$(RESET)"
	$(CXX) $(OBJS) $(LINUX_CXXFLAGS) $(LINUX_LDFLAGS) -o $@

$(BUILD_DIR)/%.o: %.cxx
	@mkdir -p $(BUILD_DIR)
	@echo -e "$(YELLOW)Compiling $(BGMAGENTA)$<$(RESET)"
	$(CXX) $(COMMON_CXXFLAGS) -c $< -o $@

# =========================
# Clean
# =========================
clean:
	@echo -e "$(RED)Cleaning build directory...$(RESET)"
	rm -rf $(BUILD_DIR)
	mkdir build