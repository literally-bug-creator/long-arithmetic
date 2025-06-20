.PHONY: all debug release test benchmark clean

BUILD_DIR = build
CXX = clang++
JOBS = $(shell nproc)

all: release

debug:
	@mkdir -p $(BUILD_DIR)
	@cd $(BUILD_DIR) && cmake -DCMAKE_CXX_COMPILER=$(CXX) ..
	@cd $(BUILD_DIR) && cmake --build . --parallel $(JOBS)

release:
	@mkdir -p $(BUILD_DIR)
	@cd $(BUILD_DIR) && cmake -DCMAKE_BUILD_TYPE=Release -DCMAKE_CXX_COMPILER=$(CXX) ..
	@cd $(BUILD_DIR) && cmake --build . --parallel $(JOBS)

test:
	@./$(BUILD_DIR)/tests/run_tests

benchmark:
	@./$(BUILD_DIR)/benchmark/run_benchmark --benchmark_time_unit=ms

clean:
	@rm -rf $(BUILD_DIR)
