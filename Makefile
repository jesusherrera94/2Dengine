.PHONY: build run clean
build:
	mkdir -p build
	g++ -Wall -std=c++17 src/*.cpp -lSDL2 -llua -o ./build/gameengine;
run:
	./build/gameengine;
clean:
	rm -rf ./build;
