#pragma once

#include <iostream>
#include <string>
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <SDL2/SDL_mixer.h>
#include <SDL2/SDL_ttf.h>
#include <SDL2/SDL_net.h>

bool init();
bool loadMedia();
void close();
SDL_Surface* loadSurface( std::string path);

SDL_Texture* loadTexture( std::string path);