.PHONY: run dev clean

BUILD_DIR := build
TARGET := Digit_Detection
CMAKE_FLAGS_DEV := -DUSE_SYSTEM_WXWIDGETS=ON -WARNINGS=ON

run: CMakeLists.txt
	@mkdir -p $(BUILD_DIR)
	@cd $(BUILD_DIR) && cmake .. && cmake --build .
	@./$(BUILD_DIR)/$(TARGET)

dev: CMakeLists.txt
	@mkdir -p $(BUILD_DIR)
	@cd $(BUILD_DIR) && cmake $(CMAKE_FLAGS_DEV) .. && cmake --build .
	@./$(BUILD_DIR)/$(TARGET)

clean:
	@rm -rf $(BUILD_DIR)
