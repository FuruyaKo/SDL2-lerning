#!/bin/bash
g++ -g -o ./bin/$1 ./src/$1.cpp `sdl2-config --cflags --libs` -lSDL2 -lSDL2_mixer -lSDL2_image -lSDL2_ttf -lSDL2_net && ./bin/$1