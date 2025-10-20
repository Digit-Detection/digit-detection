.PHONY: run dev clean

BUILD_DIR := build
TARGET := Digit_Detection

run: CMakeLists.txt
	@mkdir -p $(BUILD_DIR)
	@cd $(BUILD_DIR) && cmake -DUSE_SYSTEM_WXWIDGETS=OFF .. && cmake --build .
	@./$(BUILD_DIR)/$(TARGET)

dev: CMakeLists.txt
	@mkdir -p $(BUILD_DIR)
	@cd $(BUILD_DIR) && cmake -DUSE_SYSTEM_WXWIDGETS=ON .. && cmake --build .
	@./$(BUILD_DIR)/$(TARGET)

clean:
	@rm -rf $(BUILD_DIR)
