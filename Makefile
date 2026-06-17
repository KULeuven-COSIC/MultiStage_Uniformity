FILE ?= main
OUT ?= $(FILE)

# Detect OS
ifeq ($(OS),Windows_NT)
TARGET = $(OUT).exe
RM = del /Q
else
TARGET = $(OUT)
RM = rm -f
UNAME_S := $(shell uname -s)

ifeq ($(UNAME_S),Linux)
PLATFORM = LINUX
endif
ifeq ($(UNAME_S),Darwin)
PLATFORM = MAC
endif
endif

# Compiler
CXX = g++

# Flags
CXXFLAGS = -Wall -std=c++11

# Source files
SRC ?= $(FILE).cpp
OBJ = $(SRC:.cpp=.o)

# Default target
all: $(TARGET)

# Linking step
$(TARGET): $(OBJ)
	$(CXX) $(CXXFLAGS) -o $(TARGET) $(OBJ)

# Compilation step
%.o: %.cpp
	$(CXX) $(CXXFLAGS) -c $< -o $@

# Clean
clean:
	$(RM) $(OBJ) $(TARGET)
