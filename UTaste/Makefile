# Compiler and flags
CXX := g++
CXXFLAGS := -Wall -Wextra -std=c++20
LDFLAGS := -pthread

# Directories
SRC_DIR := src
BUILD_DIR := build
BIN_DIR := .
OUTPUT := $(BIN_DIR)/UTaste

# Recursive wildcard function to find all .cpp and .hpp files
rwildcard=$(wildcard $1$2) $(foreach d,$(wildcard $1*),$(call rwildcard,$d/,$2))

# Source and header files
SRC_FILES := $(call rwildcard,$(SRC_DIR)/,*.cpp)
HEADER_FILES := $(call rwildcard,$(SRC_DIR)/,*.hpp)
OBJ_FILES := $(patsubst $(SRC_DIR)/%.cpp,$(BUILD_DIR)/%.o,$(SRC_FILES))

# Include directories
INC_DIRS := $(sort $(dir $(HEADER_FILES)))
CXXFLAGS += $(addprefix -I,$(INC_DIRS))

# Default target
.PHONY: all
all: $(OUTPUT)

# Link the final binary
$(OUTPUT): $(OBJ_FILES)
	@mkdir -p $(BIN_DIR)
	$(CXX) $(CXXFLAGS) -o $@ $^ $(LDFLAGS)
	@echo "Linked -> $@"

# Compile source files into object files
$(BUILD_DIR)/%.o: $(SRC_DIR)/%.cpp $(HEADER_FILES)
	@mkdir -p $(@D)
	$(CXX) $(CXXFLAGS) -c $< -o $@
	@echo "Compiled -> $<"

# Clean build files
.PHONY: clean
clean:
	@rm -rf $(BUILD_DIR) $(OUTPUT)
	@echo "Cleaned build files"

# Rebuild everything
.PHONY: rebuild
rebuild: clean all

# Debug target
.PHONY: debug
debug: CXXFLAGS += -g
debug: rebuild
	@echo "Built with debugging symbols"

# Run the server (both frontend and backend)
.PHONY: run
run: $(OUTPUT)
	./$(OUTPUT) 5000 tests/csv/restaurants.csv tests/csv/districts.csv tests/csv/discounts.csv