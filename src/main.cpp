#include "main.hpp"

////////////////////////////////////////////////
// 定数
////////////////////////////////////////////////
const int SCREEN_WIDTH = 640;
const int SCREEN_HEIGHT = 480;
enum KeyPressSurfaces{
    KEY_PRESS_SURFACE_DEFAULT,
    KEY_PRESS_SURFACE_UP,
    KEY_PRESS_SURFACE_DOWN,
    KEY_PRESS_SURFACE_LEFT,
    KEY_PRESS_SURFACE_RIGHT,
    KEY_PRESS_SURFACE_TOTAL
};

////////////////////////////////////////////////
// グローバル変数
////////////////////////////////////////////////
// レンダリング先のウインドウ
SDL_Window* gWindow = NULL;
// ウインドウレンダラー
SDL_Renderer* gRenderer = NULL;
// シーンテキスチャー
LTexture gSpriteTexture;


////////////////////////////////////////////////
// 実装
////////////////////////////////////////////////
// SDLを起動し、ウインドウを作成する
bool init(){
    //初期化処理のフラグ
    bool success = true;

    // SDLの初期化
    if( SDL_Init( SDL_INIT_VIDEO ) < 0 )
    {
        printf( "SDL could not initialize! SDL_Error: %s\n", SDL_GetError() );
        success = false;
    }
    else{
        //Create window
        gWindow = SDL_CreateWindow( "SDL Tutorial", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, SCREEN_WIDTH, SCREEN_HEIGHT, SDL_WINDOW_SHOWN );
        if( gWindow == NULL )
        {
            printf( "Window could not be created! SDL_Error: %s\n", SDL_GetError() );
            success = false;
        }
        else
        {
            // ウインドウレンダラーを作成vsync
            gRenderer = SDL_CreateRenderer( gWindow, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC );
            if( gRenderer == NULL){
                printf( "Renderer could not be created! SDL Error: %s\n", SDL_GetError() );
                success = false;
            }
            else{
                // レンダラーカラーを初期化
                SDL_SetRenderDrawColor( gRenderer, 0xFF, 0xFF, 0xFF, 0xFF );
                // PNGローディングの初期化
                int imgFlags = IMG_INIT_PNG;
                if( !( IMG_Init( imgFlags ) & imgFlags ) ){
                    printf( "SDL_image could not initialize! SDL_image Error: %s\n", IMG_GetError() );
                    success = false;
                }
            }
        }
    }
    return success;
}

// メディアをロードする
bool loadMedia(){
    // ロード完了フラグ
    bool success = true;

    // テキスチャを読み込む
    if( !gSpriteTexture.loadFromFile( "./media/up.png" ) ){
        printf( "Failed to load Foo texture image!\n" );
        success = false;
    }
    else{
    }

    return success;
}

// メディアを解放し、SDLを閉じる
void close(){
    // 読み込んだ画像を解放する
    gSpriteTexture.free();

    // ウインドウの破棄
    SDL_DestroyRenderer( gRenderer );
    SDL_DestroyWindow( gWindow );
    gRenderer = NULL;
    gWindow = NULL;

    // SDLサブシステムを閉じる
    IMG_Quit();
    SDL_Quit();
}

// テキスチャーの読み込み
SDL_Texture* loadTexture( std::string path ){
    // 最終的なテキスチャー
    SDL_Texture* newTexture = NULL;

    // 指定したパスの画像を読み込む
    SDL_Surface* loadedSurface = IMG_Load( path.c_str() );
    if( loadedSurface == NULL){
        printf( "Unable to load image %s! SDL_image Error: %s\n", path.c_str(), IMG_GetError() );
    }
    else{
        // サーファイスピクセルからテキスチャーを作成
        newTexture = SDL_CreateTextureFromSurface( gRenderer, loadedSurface );
        if( newTexture == NULL ){
            printf( "Unable to create texture from %s! SDL Error: %s\n", path.c_str(), SDL_GetError() );
        }

        // 古い読み込まれたサーフェイスを取り除く
        SDL_FreeSurface( loadedSurface );
    }

    return newTexture;
}

// LTextureの実装
LTexture::LTexture(){
    // 初期化
    mTexture = NULL;
    mWidth = 0;
    mHeight = 0;
}

LTexture::~LTexture(){
    // 解放
    free();
}

bool LTexture::loadFromFile( std::string path ){
    // 既存のテキスチャーを取り除く
    free();

    // 最終的なテキスチャー
    SDL_Texture* newTexture = NULL;

    // 指定したパスの画像を読み込む
    SDL_Surface* loadedSurface = IMG_Load( path.c_str() );
    if( loadedSurface == NULL){
        printf( "Unable to load image %s! SDL_image Error: %s\n", path.c_str(), IMG_GetError() );
    }
    else{
        // カラーキーイメージ
        SDL_SetColorKey( loadedSurface, SDL_TRUE, SDL_MapRGB(loadedSurface->format, 0x0A, 0x0A, 0xF5 ) );
        // サーファイスピクセルからテキスチャーを作成
        newTexture = SDL_CreateTextureFromSurface( gRenderer, loadedSurface );
        if( newTexture == NULL ){
            printf( "Unable to create texture from %s! SDL Error: %s\n", path.c_str(), SDL_GetError() );
        }
        else{
            // 画像のサイズを取得
            mWidth = loadedSurface->w;
            mHeight = loadedSurface->h;
        }

        // 古い読み込まれたサーフェイスを取り除く
        SDL_FreeSurface( loadedSurface );
    }

    // 成功を返す
    mTexture = newTexture;
    return mTexture != NULL;
}

void LTexture::free(){
    // 存在しているならテキスチャーを解放する
    if( mTexture != NULL ){
        SDL_DestroyTexture( mTexture );
        mTexture = NULL;
        mWidth = 0;
        mHeight = 0;
    }
}

void LTexture::setColor( Uint8 red, Uint8 green, Uint8 blue){
    SDL_SetTextureColorMod( mTexture, red, green, blue);
}

void LTexture::setBlendMode( SDL_BlendMode blending ){
    // ブレンド関数を設定
    SDL_SetTextureBlendMode( mTexture, blending );
}

void LTexture::setAlpha( Uint8 alpha ){
    // テキスチャのアルファ値を変調
    SDL_SetTextureAlphaMod( mTexture, alpha );
}

void LTexture::render( int x, int y, SDL_Rect* clip, double angle, SDL_Point* center, SDL_RendererFlip flip ){
    // レンダリング位置を設定してレンダーする
    SDL_Rect renderQuad = { x, y, mWidth, mHeight};

    if( clip != NULL ){
        renderQuad.w = clip->w;
        renderQuad.h = clip->h;
    }

    SDL_RenderCopyEx( gRenderer, mTexture, clip, &renderQuad, angle, center, flip );
}

int LTexture::getWidth(){
    return mWidth;
}

int LTexture::getHeight(){
    return mHeight;
}

////////////////////////////////////////////////
// メイン関数
////////////////////////////////////////////////
int main(int argc, char ** const args){
    // メインループのフラグ
    bool quit = false;

    // イベントハンドラー
    SDL_Event e;

    // 回転角度
    double degrees = 0;

    //  フリップタイプ
    SDL_RendererFlip flipType = SDL_FLIP_NONE;

    // SDLの起動とウインドウの作成
    if( !init() )
    {
        printf( "Failed to initialize!\n" );
    }
    else{
        // メディアのロード
        if( !loadMedia() ){
            printf( "Failed to load media!\n" );
        }
        else{

            // アプリケーションが起動している間
            while( !quit ){
                // キューのイベントをハンドル
                while( SDL_PollEvent( &e ) != 0){
                    // ユーザーが閉じるを要求
                    if( e.type == SDL_QUIT ){
                        quit = true;
                    }
                    // ユーザーがキーを押したとき
                    else if( e.type == SDL_KEYDOWN){
                        // キー押下
                        switch( e.key.keysym.sym ){
                            case SDLK_ESCAPE:
                                quit = true;
                                break;
                            case SDLK_a:
                                degrees -= 60;
                                break;
                            case SDLK_d:
                                degrees += 60;
                                break;
                            case SDLK_q:
                                flipType = SDL_FLIP_HORIZONTAL;
                                break;
                            case SDLK_w:
                                flipType = SDL_FLIP_NONE;
                                break;
                            case SDLK_e:
                                flipType = SDL_FLIP_VERTICAL;
                                break;
                        }
                    }
                }
                

                // スクリーンのクリア
                SDL_SetRenderDrawColor( gRenderer, 0xFF, 0xFF, 0xFF, 0xFF );
                SDL_RenderClear( gRenderer );

                // テキスチャーをレンダーする
                gSpriteTexture.render( ( SCREEN_WIDTH - gSpriteTexture.getWidth() ) / 2, (SCREEN_HEIGHT - gSpriteTexture.getHeight() ) / 2, NULL, degrees, NULL, flipType );

                // スクリーンの更新
                SDL_RenderPresent( gRenderer );
            }
            
        }
    }

    // リソースの解放とSDLを閉じる
    close();

    return 0;
}