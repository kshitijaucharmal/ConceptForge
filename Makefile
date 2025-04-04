CFLAGS = -std=c++17 -O2
LDFLAGS = -lglfw -lvulkan -ldl -lpthread -lX11 -lXxf86vm -lXrandr -lXi

SRC_FILES = src/*.cpp
BINARY_LOC = bin/VulkanTest

VulkanTest: $(SRC_FILES)
	g++ $(CFLAGS) -o $(BINARY_LOC) $(SRC_FILES) $(LDFLAGS)

.PHONY: test clean

test: VulkanTest
	./$(BINARY_LOC)

clean:
	rm -f $(BINARY_LOC)
