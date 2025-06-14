.PHONY: all debug release run test benchmark clean

BUILD_DIR = build

all: release

debug:
	@mkdir -p $(BUILD_DIR)
	@cd $(BUILD_DIR) && cmake  ..
	@cd $(BUILD_DIR) && cmake --build .

release:
	@mkdir -p $(BUILD_DIR)
	@cd $(BUILD_DIR) && cmake -DCMAKE_BUILD_TYPE=Release ..
	@cd $(BUILD_DIR) && cmake --build .

test:
	@./$(BUILD_DIR)/tests/run_tests

benchmark:
	@./$(BUILD_DIR)/benchmark/run_benchmark --benchmark_time_unit=ms

run:
	@./$(BUILD_DIR)/main

clean:
	@rm -rf $(BUILD_DIR)
