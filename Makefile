run: 
	@mkdir build
	@cd build && cmake -DUSE_SYSTEM_WXWIDGETS=ON ..
	@cd build && cmake --build .
	@./build/Digit_Detection
	@$(MAKE) clean
clean:
	@rm -rf build