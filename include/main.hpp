#pragma once

#include <bits/stdc++.h>
#include <iostream>
#include <string>
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <SDL2/SDL_mixer.h>
#include <SDL2/SDL_ttf.h>
#include <SDL2/SDL_net.h>

using namespace std;

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

        // フォント文字から画像を生成
        bool loadFromRenderedText( string textureText, SDL_Color textColor );

        // メモリの解放
        void free();

        // カラー変調の設定
        void setColor( Uint8 red, Uint8 green, Uint8 blue);

        // ブレンディングの設定
        void setBlendMode( SDL_BlendMode blending);

        // アルファ変調
        void setAlpha( Uint8 alpha);

        // 与えられた位置にテキスチャーをレンダリング
        void render(int x, int y, SDL_Rect* clip = NULL, double angle = 0.0, SDL_Point* center = NULL, SDL_RendererFlip flip =  SDL_FLIP_NONE );

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
