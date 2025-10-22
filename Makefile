# Compiler and Flags
CXX := clang++
CXXFLAGS := -std=c++2b -Wall -g -O0 \
             -fsanitize=address -fno-omit-frame-pointer -fno-optimize-sibling-calls \
             -I/opt/homebrew/Cellar/nlohmann-json/3.12.0/include/ \
             -I/Users/home/VulkanSDK/1.4.321.0/macOS/include \
             -I/opt/homebrew/Cellar/glfw/3.4/include/

LDFLAGS := \
  -L/usr/local/lib \
  -L/opt/homebrew/lib \
  -L./lib \
  -L/Users/home/VulkanSDK/1.4.321.0/macOS/lib \
  -lvulkan -lMoltenVK -lglfw \
  -framework Cocoa -framework IOKit -framework CoreFoundation \
  -framework QuartzCore -framework Metal \
  -Wl,-rpath,/Users/home/VulkanSDK/1.4.321.0/macOS/lib

# Sources and Outputs
SRCS := $(wildcard *.cxx)
OBJS := $(SRCS:.cxx=.o)

MAC_EXEC := main_mac
LINUX_EXEC := main_linux
UNAME_S := $(shell uname -s)

# ANSI Colors
YELLOW := \033[1;33m
GREEN  := \033[1;32m
RED    := \033[1;31m
BGMAGENTA := \033[45m
RESET  := \033[0m

# Targets
.PHONY: all clean

all:
ifeq ($(UNAME_S),Darwin)
	@echo -e "$(YELLOW)Building for macOS...$(RESET)"
	$(MAKE) $(MAC_EXEC)
	@echo -e "$(GREEN)Running $(BGMAGENTA)$(MAC_EXEC)...$(RESET)"
else
	@echo -e "$(YELLOW)Building for Linux...$(RESET)"
	$(MAKE) $(LINUX_EXEC)
endif

# macOS build
$(MAC_EXEC): $(OBJS)
	@echo -e "$(GREEN)Linking $(BGMAGENTA)$@$(RESET)"
	$(CXX) $(CXXFLAGS) $(OBJS) $(LDFLAGS) -o $@

# Linux build
$(LINUX_EXEC): $(OBJS)
	@echo -e "$(GREEN)Linking $(BGMAGENTA)$@$(RESET)"
	$(CXX) $(CXXFLAGS) $(OBJS) $(LDFLAGS) -o $@

# Compile source files
%.o: %.cxx
	@echo -e "$(YELLOW)Compiling $(BGMAGENTA)$<$(RESET)"
	$(CXX) $(CXXFLAGS) -c $< -o $@

# Clean build files
clean:
	@echo -e "$(RED)Cleaning all build files...$(RESET)"
	rm -f $(MAC_EXEC) $(LINUX_EXEC) *.o