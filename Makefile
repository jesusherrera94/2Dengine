.PHONY: build run clean
build:
	mkdir -p build
	g++ -Wall -std=c++17 -I"./libs/" src/*.cpp `sdl2-config --libs --cflags` -lSDL2_image -lSDL2_ttf -lSDL2_mixer -llua -o ./build/gameengine;
run:
	./build/gameengine;
clean:
	rm -rf ./build;
 