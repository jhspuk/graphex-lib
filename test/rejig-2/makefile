# Define the compiler and the flags
CXX = g++
#CXX = clang++
CXXFLAGS = -Wall -Wextra -std=c++20 -ggdb #-O3 #-flto #-ggdb -Wpadded

# Define the target executable
TARGET = main.run

# Find all the .cpp files in the current directory
SRCS = $(wildcard *.cpp)

# Convert .cpp files to .o files
OBJS = $(SRCS:.cpp=.o)

# Default target: build the program
all: $(TARGET)

# Link object files to create the executable
$(TARGET): $(OBJS)
	$(CXX) $(CXXFLAGS) -o $(TARGET) $(OBJS)

# Compile .cpp files to .o files
%.o: %.cpp
	$(CXX) $(CXXFLAGS) -c $< -o $@

# Remove generated files
clean:
	rm -f $(TARGET) $(OBJS)

# Run the program
run: $(TARGET)
	./$(TARGET)

# Phony targets to avoid conflicts with files of the same name
.PHONY: all clean run

