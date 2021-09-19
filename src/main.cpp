#include "main.hpp"

////////////////////////////////////////////////
// 定数
////////////////////////////////////////////////
const int SCREEN_WIDTH = 640;
const int SCREEN_HEIGHT = 480;
// Button constants
const int BUTTON_WIDTH = 300;
const int BUTTON_HEIGHT = 200;
const int TOTAL_BUTTONS = 4;


////////////////////////////////////////////////
// グローバル変数
////////////////////////////////////////////////
// レンダリング先のウインドウ
SDL_Window* gWindow = NULL;
// ウインドウレンダラー
SDL_Renderer* gRenderer = NULL;
// フォント
TTF_Font *gFont = NULL;
// テキスチャー
LTexture gButtonSpriteSheetTexture;
SDL_Rect gSpriteClips[ TOTAL_BUTTONS ];

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

                // SDL_ttfの初期化
                if( TTF_Init() == -1 ){
                    printf( "SDL_ttf could not initialize! SDL_ttf Error: %s\n", TTF_GetError() );
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

    // スプライトシートテキスチャの読み込み
    if( !gButtonSpriteSheetTexture.loadFromFile( "./media/sprite_sheet.png" ) ){
        printf( "Failed to load walking animation texture!\n" );
        success = false;
    }else{
        // スプライトクリップを設定
        gSpriteClips[ 0 ].x =   0;
        gSpriteClips[ 0 ].y =   0;
        gSpriteClips[ 0 ].w = 128;
        gSpriteClips[ 0 ].h = 128;

        gSpriteClips[ 1 ].x = 128;
        gSpriteClips[ 1 ].y =   0;
        gSpriteClips[ 1 ].w = 128;
        gSpriteClips[ 1 ].h = 128;

        gSpriteClips[ 2 ].x =   0;
        gSpriteClips[ 2 ].y = 128;
        gSpriteClips[ 2 ].w = 128;
        gSpriteClips[ 2 ].h = 128;

        gSpriteClips[ 3 ].x = 128;
        gSpriteClips[ 3 ].y = 128;
        gSpriteClips[ 3 ].w = 128;
        gSpriteClips[ 3 ].h = 128;
        
    }

    /*
    // フォントを開く
    gFont = TTF_OpenFont( "./media/GenShinGothic-Regular.ttf", 28 );
    if( gFont == NULL ){
        printf( "Failed to load lazy font! SDL_ttf Error: %s\n", TTF_GetError() );
        success = false;
    }
    else{
        // テキストをレンダー
        SDL_Color textColor = { 0, 0, 0 };
        if( !gTexture.loadFromRenderedText( "The quick brown fox jumps over the lazy dog", textColor ) ){
            printf( "Failed to render text texture!\n" );
            success = false;
        }
    }
    */
    return success;
}

// メディアを解放し、SDLを閉じる
void close(){
    // 読み込んだ画像を解放する
    gButtonSpriteSheetTexture.free();

    // グローバルフォントの解放
    TTF_CloseFont( gFont );
    gFont = NULL;

    // ウインドウの破棄
    SDL_DestroyRenderer( gRenderer );
    SDL_DestroyWindow( gWindow );
    gRenderer = NULL;
    gWindow = NULL;

    // SDLサブシステムを閉じる
    TTF_Quit();
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

#if defined(SDL_TTF_MAJOR_VERSION)
bool LTexture::loadFromRenderedText( string textureText, SDL_Color textColor ){
    // 既存のテキスチャを消去
    free();

    // テキストサーフェイスをレンダー
    SDL_Surface* textSurface = TTF_RenderText_Solid( gFont, textureText.c_str(), textColor);
    if( textSurface ==NULL ){
        printf( "Unable to render text surface! SDL_ttf Error: %s\n", TTF_GetError() );
    }
    else{
        // サーフェイスピクセルからテキスチャを生成
        mTexture = SDL_CreateTextureFromSurface( gRenderer, textSurface );
        if( mTexture == NULL ){
            printf( "Unable to create texture from rendered text! SDL Error: %s\n", SDL_GetError() );
        }
        else{
            // イメージの次元数を取得
            mWidth = textSurface->w;
            mHeight = textSurface->h;
        }

        // 古いサーフェイスを消去
        SDL_FreeSurface( textSurface );
    }

    // 結果を返す
    return mTexture != NULL;
}
#endif

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

// LButtonの実装
LButton::LButton(){
    mPosition.x = 0;
    mPosition.y = 0;

    mCurrentSprite = BUTTON_SPRITE_MOUSE_OUT;
}

void LButton::setPosition( int x, int y ){
    mPosition.x = x;
    mPosition.y = y;
}

void LButton::handleEvent( SDL_Event* e ){
    // マウスボタンのイベントが発生
    if( e->type == SDL_MOUSEMOTION || e->type == SDL_MOUSEBUTTONDOWN || e->type == SDL_MOUSEBUTTONUP ){
        // マウス位置の取得
        int x, y;
        SDL_GetMouseState( &x, &y );

        // マウスがボタンに含まれるか確認
        bool inside = true;

        // 左のボタンにマウス
        if( x < mPosition.x ){
            inside = false;
        }
        // 右のボタンにマウス
        else if( x > mPosition.x + BUTTON_WIDTH ){
            inside = false;
        }
        // 上のボタンにマウス
        else if( y < mPosition.y ){
            inside = false;
        }
        // 下のボタンにマウス
        else if( y > mPosition.y + BUTTON_HEIGHT ){
            inside = false;
        }

        // 外側にマウス
        if( !inside ){
            mCurrentSprite = BUTTON_SPRITE_MOUSE_OUT;
        }
        // 内側にマウス
        else{
            // スプライトの上にマウス
            switch( e->type ){
                case SDL_MOUSEMOTION:
                    mCurrentSprite = BUTTON_SPRITE_MOUSE_OVER_MOTION;
                    break;
                case SDL_MOUSEBUTTONDOWN:
                    mCurrentSprite = BUTTON_SPRITE_MOUSE_DOWN;
                    break;
                case SDL_MOUSEBUTTONUP:
                    mCurrentSprite = BUTTON_SPRITE_MOUSE_UP;
                    break;
            }
        }
    }
}

void LButton::render(){
    // 現在のボタンスプライトを表示
    gButtonSpriteSheetTexture.render( mPosition.x, mPosition.y, &gSpriteClips[ mCurrentSprite ] );
}

////////////////////////////////////////////////
// メイン関数
////////////////////////////////////////////////
int main(int argc, char ** const args){
    // メインループのフラグ
    bool quit = false;

    // イベントハンドラー
    SDL_Event e;
    LButton gButtons[ TOTAL_BUTTONS ];

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
                                break;
                            case SDLK_d:
                                break;
                            case SDLK_q:
                                break;
                            case SDLK_w:
                                break;
                            case SDLK_e:
                                break;
                        }
                    }
                    else{
                        // ボタンイベントをハンドル
                        for( int i = 0; i < TOTAL_BUTTONS; ++i ){
                            gButtons[ i ].handleEvent( &e );
                        }
                    }
                }
                

                // スクリーンのクリア
                SDL_SetRenderDrawColor( gRenderer, 0xFF, 0xFF, 0xFF, 0xFF );
                SDL_RenderClear( gRenderer );

                // テキスチャーをレンダーする
                for( int i = 0; i < TOTAL_BUTTONS; ++i ){
                    gButtons[ i ].render();
                }

                // スクリーンの更新
                SDL_RenderPresent( gRenderer );
            }
            
        }
    }

    // リソースの解放とSDLを閉じる
    close();

    return 0;
}