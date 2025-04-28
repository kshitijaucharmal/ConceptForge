default:
	cd build && make && ./OpenGL

test:
	./build/OpenGL

build:
	mkdir -p build && cd build && cmake .. && make

all: build test
