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

class LTexture{
    public:
        // 値の初期化
        LTexture();
        // メモリの解放
        ~LTexture();

        // 指定したパスの画像を読み込む
        bool loadFromFile( std::string path );

        // メモリの解放
        void free();

        // カラー変調のセット
        void setColor( Uint8 red, Uint8 green, Uint8 blue);

        // 与えられた位置にテキスチャーをレンダリング
        void render(int x, int y, SDL_Rect* clip = NULL );

        // 画像の大きさを取得
        int getWidth();
        int getHeight();

    private:
        // 実際のハードウェアテキスチャ
        SDL_Texture* mTexture;

        // 画像の大きさ
        int mWidth;
        int mHeight;
};
