.PHONY: run dev clean

# MAKE FILE VARIABLES
BUILD_DIR := build
TARGET := Digit_Detection
CMAKE_FLAGS_DEV := -DUSE_SYSTEM_WXWIDGETS=ON -WARNINGS=ON

# Basic run (Does not use warnings and utlizes submodule)
run: CMakeLists.txt
	@mkdir -p $(BUILD_DIR)
	@cd $(BUILD_DIR) && cmake .. && cmake --build .
	@./$(BUILD_DIR)/$(TARGET)

# Developer run (Uses warnings and utilizes local wxWidget for faster execution)
dev: CMakeLists.txt
	@mkdir -p $(BUILD_DIR)
	@cd $(BUILD_DIR) && cmake $(CMAKE_FLAGS_DEV) .. && cmake --build .
	@./$(BUILD_DIR)/$(TARGET)

# Removed build folder on complete
clean:
	@rm -rf $(BUILD_DIR)
