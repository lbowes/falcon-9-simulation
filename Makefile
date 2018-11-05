PROJECT = F9Simulation

GCC = gcc
CXX = g++

LOCAL_OUTPUT_DIR = ./bin/

INCLUDE_DIRS = -I/home/lbowes/dev/Programming/Deps/Includes/ -I/home/lbowes/dev/Programming/Projects/F9\ Simulation/src/
LIB_DIRS = -L/home/lbowes/dev/Programming/Deps/Libs/
BUILD_DIR = ./build/
SOURCE_FILES = $(shell find . -name "*.cpp")
VPATH := $(sort $(dir $(SOURCE_FILES))) 
OBJECTS = $(patsubst %.cpp, $(BUILD_DIR)%.o, $(notdir $(SOURCE_FILES)))

CXXFLAGS = -Wall -Wextra $(INCLUDE_DIRS) -MD -g --std=c++17
LDFLAGS = $(LIB_DIRS) -lGraphicsFramework -lPhysicsFramework $(shell pkg-config --libs --static glfw3) $(shell pkg-config --libs --static glew)

print-%  : ; @echo $* = $($*)

all: $(LOCAL_OUTPUT_DIR)$(PROJECT)

run: $(LOCAL_OUTPUT_DIR)$(PROJECT)
	./$<

$(BUILD_DIR)%.o: %.cpp
	@echo Compiling $(notdir $<)
	@$(CXX) $(CXXFLAGS) -c -o $@ $< $(LDFLAGS)

$(LOCAL_OUTPUT_DIR)$(PROJECT): $(OBJECTS)
	@$(CXX) $(CXXFLAGS) -o $@ $^ $(LDFLAGS)

.PHONY: clean

clean:
	@rm -f $(LOCAL_OUTPUT_DIR)$(PROJECT) $(OBJECTS)
	@rm -f $(OBJECTS:.o=.d)
	@echo Cleaned all.

-include $(OBJECTS:.o=.d)