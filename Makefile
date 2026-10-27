.PHONY: build run clean

# Declare makefile variables
CCX = g++
LANG_STD = -std=c++17
FLAGS = -Wall -Wfatal-errors
INCLUDE_PATHS = -I"./libs"
SRC_FILES = src/*.cpp \
			src/game/*.cpp \
			src/logger/*.cpp \
			src/ecs/*.cpp
LINKER_FLAGS = `sdl2-config --libs --cflags` -lSDL2_image -lSDL2_ttf -lSDL2_mixer -llua
OBJ_NAME = gameengine

# rules
build:
	mkdir -p build
	$(CCX) $(FLAGS) $(LANG_STD) $(INCLUDE_PATHS) $(SRC_FILES) $(LINKER_FLAGS) -o ./build/$(OBJ_NAME);
run:
	./build/$(OBJ_NAME);
clean:
	rm -rf ./build;
 