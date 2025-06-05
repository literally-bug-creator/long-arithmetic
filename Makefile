.PHONY: all build run test benchmark clean

BUILD_DIR = build

all: build

build:
	@mkdir -p $(BUILD_DIR)
	@cd $(BUILD_DIR) && cmake ..
	@cd $(BUILD_DIR) && cmake --build .


run:
	@./$(BUILD_DIR)/main


test:
	@./$(BUILD_DIR)/tests/run_tests


benchmark:
	@./$(BUILD_DIR)/benchmark/run_benchmark --benchmark_time_unit=ms


clean:
	@rm -rf $(BUILD_DIR)


rebuild: clean build
