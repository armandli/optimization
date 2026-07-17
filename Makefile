BUILD_DIR := build

.DEFAULT_GOAL := all

.PHONY: all clean

all: $(BUILD_DIR)/CMakeCache.txt
	cmake --build $(BUILD_DIR)

$(BUILD_DIR)/CMakeCache.txt: CMakeLists.txt
	cmake -B $(BUILD_DIR)

clean:
	rm -rf $(BUILD_DIR)
