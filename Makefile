CXX = g++

CXXFLAGS = -std=c++11 -Wall

LDFLAGS = -lSDL2

TARGET = build/gamepad_typer

SRCS = gamepad_typer.cpp

OBJS = $(patsubst %.cpp,build/%.o,$(SRCS))

# Default target
all: build $(TARGET)

build:
	mkdir -p build

$(TARGET): $(OBJS)
	$(CXX) $(CXXFLAGS) -o $@ $^ $(LDFLAGS)

build/%.o: %.cpp
	$(CXX) $(CXXFLAGS) -c $< -o $@

clean:
	rm -rf build

# Phony targets
.PHONY: all clean build
