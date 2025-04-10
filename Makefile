default:
	cd build && make && ./VulkanTest

test:
	./build/VulkanTest

build:
	mkdir -p build && cd build && cmake .. && make

all: build test
