CXX = g++
CXXFLAGS = -Wall -std=c++11 -Isrc/include

TARGET = bin/main
SOURCES = src/main.cpp src/image.cpp src/quadtree.cpp src/varians.cpp

all:
	$(CXX) $(CXXFLAGS) $(SOURCES) -o $(TARGET)

run: all
	./$(TARGET)

clean:
	rm -f $(TARGET)
