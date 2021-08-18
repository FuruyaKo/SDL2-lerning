#include "main.hpp"

////////////////////////////////////////////////
// 定数
////////////////////////////////////////////////
const int SCREEN_WIDTH = 640;
const int SCREEN_HEIGHT = 480;

////////////////////////////////////////////////
// グローバル変数
////////////////////////////////////////////////
// レンダリング先のウインドウ
SDL_Window* gWindow = NULL;
// ウインドウに含まれる表面
SDL_Surface* gScreenSurface = NULL;
// 画像をロードしてスクリーンに表示する
SDL_Surface* gHelloWorld = NULL;

////////////////////////////////////////////////
// 関数
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
            //Get window surface
            gScreenSurface = SDL_GetWindowSurface( gWindow );
        }
    }
    return success;
}

// メディアをロードする
bool loadMedia(){
    // ロード完了フラグ
    bool success = true;

    // スプラッシュイメージをロードする
    gHelloWorld = IMG_Load( "./media/furuya_kou002_3.png" );
    if( gHelloWorld == NULL){
        printf( "Unable to load image %s! SDL Error: %s\n", "02_getting_an_image_on_the_screen/hello_world.bmp", SDL_GetError() );
        success = false;
    }

    return success;
}

// メディアを解放し、SDLを閉じる
void close(){
    // サーフェイスの解放
    SDL_FreeSurface(gHelloWorld);
    gHelloWorld = NULL;

    // ウインドウの破棄
    SDL_DestroyWindow( gWindow );
    gWindow = NULL;

    // SDLサブシステムを閉じる
    SDL_Quit();
}

////////////////////////////////////////////////
// メイン関数
////////////////////////////////////////////////
int main(int argc, char ** const args){

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
            // 画像を表示する
            SDL_BlitSurface( gHelloWorld, NULL, gScreenSurface, NULL);

            // サーフェイスの更新
            SDL_UpdateWindowSurface( gWindow );

            // 2秒間待つ
            SDL_Delay( 2000 );
        }
    }

    // リソースの解放とSDLを閉じる
    close();

    return 0;
}