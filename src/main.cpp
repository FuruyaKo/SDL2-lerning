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
// ウインドウに含まれる表面
SDL_Surface* gScreenSurface = NULL;
// キーに対応した画像
SDL_Surface* gKeyPressSurfaces[ KEY_PRESS_SURFACE_TOTAL];
// 現在表示している画像
SDL_Surface* gCurrentSurface = NULL;
// ウインドウレンダラー
SDL_Renderer* gRenderer = NULL;
// 現在表示しているテキスチャー
SDL_Texture* gTexture = NULL;


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
            // ウインドウレンダラーを作成
            gRenderer = SDL_CreateRenderer( gWindow, -1, SDL_RENDERER_ACCELERATED );
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
                else{
                    //Get window surface
                    gScreenSurface = SDL_GetWindowSurface( gWindow );
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

    /*
    // デフォルトサーフェイスを読み込む
    gKeyPressSurfaces[ KEY_PRESS_SURFACE_DEFAULT ] = loadSurface( "./media/furuya_kou002_3.png" );
    if( gKeyPressSurfaces[ KEY_PRESS_SURFACE_DEFAULT ] == NULL){
        printf( "Failed to load default image!\n" );
        success = false;
    }

    // upサーフェイスを読み込む
    gKeyPressSurfaces[ KEY_PRESS_SURFACE_UP ] = loadSurface( "./media/up.png" );
    if( gKeyPressSurfaces[ KEY_PRESS_SURFACE_UP ] == NULL){
        printf( "Failed to load up image!\n" );
        success = false;
    }

    // downサーフェイスを読み込む
    gKeyPressSurfaces[ KEY_PRESS_SURFACE_DOWN ] = loadSurface( "./media/down.png" );
    if( gKeyPressSurfaces[ KEY_PRESS_SURFACE_DOWN ] == NULL){
        printf( "Failed to load down image!\n" );
        success = false;
    }

    // leftサーフェイスを読み込む
    gKeyPressSurfaces[ KEY_PRESS_SURFACE_LEFT ] = loadSurface( "./media/left.png" );
    if( gKeyPressSurfaces[ KEY_PRESS_SURFACE_LEFT ] == NULL){
        printf( "Failed to load left image!\n" );
        success = false;
    }

    // rightサーフェイスを読み込む
    gKeyPressSurfaces[ KEY_PRESS_SURFACE_RIGHT ] = loadSurface( "./media/right.png" );
    if( gKeyPressSurfaces[ KEY_PRESS_SURFACE_RIGHT ] == NULL){
        printf( "Failed to load right image!\n" );
        success = false;
    }

    // PNGテキスチャを読み込む
    gTexture = loadTexture( "./media/6star.png" );
    if( gTexture == NULL ){
        printf( "Failed to load texture image!\n" );
        success = false;
    }
    */
    return success;
}

// メディアを解放し、SDLを閉じる
void close(){
    /*
    // サーフェイスの解放
    for(int i=0; i<KEY_PRESS_SURFACE_TOTAL; i++){
        SDL_FreeSurface(gKeyPressSurfaces[i]);
        gKeyPressSurfaces[i] = NULL;
    }
    gCurrentSurface = NULL;

    SDL_DestroyTexture( gTexture );
    gTexture = NULL;
    */

    // ウインドウの破棄
    SDL_DestroyRenderer( gRenderer );
    SDL_DestroyWindow( gWindow );
    gRenderer = NULL;
    gWindow = NULL;

    // SDLサブシステムを閉じる
    IMG_Quit();
    SDL_Quit();
}

// 個々の画像の読み込み
SDL_Surface* loadSurface( std::string path){
    // 最終適応画像
    SDL_Surface* optimizedSurface = NULL;

    // 指定したパスの画像を読み込む
    SDL_Surface* loadedSurface = IMG_Load( path.c_str() );
    if( loadedSurface == NULL){
        printf( "Unable to load image %s! SDL Error: %s\n", path.c_str(), SDL_GetError() );
    }
    else{
        // サーフェイスをスクリーンフォーマットにコンバート
        optimizedSurface = SDL_ConvertSurface( loadedSurface, gScreenSurface->format, 0 );
        if( optimizedSurface == NULL){
            printf( "Unable to optimize image %s! SDL Error: %s\n", path.c_str(), SDL_GetError() );
        }

        // 古い読み込まれたサーフェイスを取り除く
        SDL_FreeSurface( loadedSurface );
    }
    return optimizedSurface;
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

////////////////////////////////////////////////
// メイン関数
////////////////////////////////////////////////
int main(int argc, char ** const args){
    // メインループのフラグ
    bool quit = false;

    // イベントハンドラー
    SDL_Event e;

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
            // 現在のサーフェイスをデフォルトに設定
            gCurrentSurface = gKeyPressSurfaces[ KEY_PRESS_SURFACE_DEFAULT ];

            // アプリケーションが起動している間
            while( !quit ){
                // キューのイベントをハンドル
                while( SDL_PollEvent( &e ) != 0){
                    // ユーザーが閉じるを要求
                    if( e.type == SDL_QUIT ){
                        quit = true;
                    }
                    /*
                    // ユーザーがキーを押したとき
                    else if( e.type == SDL_KEYDOWN){
                        // キー押下を基にサーフェイスを選択する
                        switch( e.key.keysym.sym ){
                            case SDLK_UP:
                                gCurrentSurface = gKeyPressSurfaces[ KEY_PRESS_SURFACE_UP ];
                                break;
                            case SDLK_DOWN:
                                gCurrentSurface = gKeyPressSurfaces[ KEY_PRESS_SURFACE_DOWN ];
                                break;
                            case SDLK_LEFT:
                                gCurrentSurface = gKeyPressSurfaces[ KEY_PRESS_SURFACE_LEFT ];
                                break;
                            case SDLK_RIGHT:
                                gCurrentSurface = gKeyPressSurfaces[ KEY_PRESS_SURFACE_RIGHT ];
                                break;
                            default:
                                gCurrentSurface = gKeyPressSurfaces[ KEY_PRESS_SURFACE_DEFAULT ];
                                break;
                        }
                    }*/
                }

                // スクリーンのクリア
                SDL_SetRenderDrawColor( gRenderer, 0xFF, 0xFF, 0xFF, 0xFF );
                SDL_RenderClear( gRenderer );

                // 赤く塗りつぶされた四角
                SDL_Rect fillRect = { SCREEN_WIDTH/4, SCREEN_HEIGHT/4, SCREEN_WIDTH/2, SCREEN_HEIGHT/2 };
                SDL_SetRenderDrawColor( gRenderer, 0xFF, 0x00, 0x00, 0xFF);
                SDL_RenderFillRect( gRenderer, &fillRect );

                // 緑色の縁の四角
                SDL_Rect outlineRect = { SCREEN_WIDTH/6, SCREEN_HEIGHT/6, SCREEN_WIDTH*2/3, SCREEN_HEIGHT*2/3 };
                SDL_SetRenderDrawColor( gRenderer, 0x00, 0xFF, 0x00, 0xFF);
                SDL_RenderDrawRect( gRenderer, &outlineRect );

                // 青い水平線を描画
                SDL_SetRenderDrawColor( gRenderer, 0x00, 0x00, 0xFF, 0xFF );
                SDL_RenderDrawLine( gRenderer, 0, SCREEN_HEIGHT/2, SCREEN_WIDTH, SCREEN_HEIGHT/2);

                // 黄色いドットを縦線として描画
                SDL_SetRenderDrawColor( gRenderer, 0xFF, 0xFF, 0x00, 0xFF);
                for( int i = 0; i < SCREEN_HEIGHT; i += 4){
                    SDL_RenderDrawPoint( gRenderer, SCREEN_WIDTH/2, i);
                }

                // テキスチャーをスクリーンにレンダー
                // SDL_RenderCopy( gRenderer, gTexture, NULL, NULL );

                // スクリーンの更新
                SDL_RenderPresent( gRenderer );
                /*
                // 画像のサイズ変更を適用する
                SDL_Rect stretchRect;
                stretchRect.x = 0;
                stretchRect.y = 0;
                stretchRect.w = SCREEN_WIDTH;
                stretchRect.h = SCREEN_HEIGHT;
                SDL_BlitScaled( gCurrentSurface, NULL, gScreenSurface, &stretchRect );


                // 画像を表示する
                // SDL_BlitSurface( gCurrentSurface, NULL, gScreenSurface, NULL);

                // サーフェイスの更新
                SDL_UpdateWindowSurface( gWindow );
                */
            }
            
        }
    }

    // リソースの解放とSDLを閉じる
    close();

    return 0;
}