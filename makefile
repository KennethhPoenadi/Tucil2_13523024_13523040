CXX = g++
CXXFLAGS = -Wall -std=c++17 -Isrc/include

TARGET = bin/main.exe
SOURCES = src/main.cpp src/image.cpp src/quadtree.cpp src/utils.cpp src/bonus.cpp src/gifer.cpp

all:
	$(CXX) $(CXXFLAGS) $(SOURCES) -o $(TARGET)

run: all
ifeq ($(OS),Windows_NT)
	$(TARGET)
else
	./$(TARGET)
endif

clean:
	rm -f $(TARGET)
