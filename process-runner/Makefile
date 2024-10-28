CXX=g++
CXXFLAGS= -Wall -Wextra -Werror -std=c++17
SOURCES = main.cpp
OBJECTS = $(SOURCES:.cpp=.o)
TARGET = process-runner

all: $(TARGET)

$(TARGET): $(OBJECTS)
	$(CXX) $(CXXFLAGS) -o $@ $^

%.o: %.cpp
	$(CXX) $(CXXFLAGS) -c $< -o $@

clean:
	rm -f $(OBJECTS) $(TARGET)

.PHONY: all clean