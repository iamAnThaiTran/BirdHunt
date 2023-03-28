#include <SDL.h>
#include <SDL_image.h>
#include <stdio.h>
#include <string>
using namespace std ;

const int SCREEN_WIDTH = 1200;
const int SCREEN_HEIGHT = 600;

SDL_Window *gwindow = NULL ;
SDL_Renderer *grenderer = NULL ;
SDL_Event e ;
void init()
{
    gwindow = SDL_CreateWindow("SDL Tutorial", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, SCREEN_WIDTH, SCREEN_HEIGHT, SDL_WINDOW_SHOWN);
    grenderer = SDL_CreateRenderer(gwindow, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC) ;
}
void close()
{

	//Destroy window
	SDL_DestroyRenderer( grenderer );
	SDL_DestroyWindow( gwindow );
	gwindow = NULL;
	grenderer = NULL;

	//Quit SDL subsystems
	IMG_Quit();
	SDL_Quit();
}

class Base
{
public:
    Base() ;
    ~Base() ;
    void free() ;
    void loadfromfile(string path) ;
    void render(SDL_Rect* clip = NULL, double angle = 0.0, SDL_Point* center = NULL, SDL_RendererFlip flip = SDL_FLIP_NONE ) ;
protected:
    SDL_Texture *mTexture ;
    int mWidth , mHeight, mPosx, mPosy ;

};
void Base::free()
{
    if( mTexture != NULL )
	{
		SDL_DestroyTexture( mTexture );
		mTexture = NULL;
		mWidth = 0;
		mHeight = 0;
	}
}
Base::Base()
{
    mTexture = NULL ;
    mWidth = 0 ;
    mHeight = 0 ;
    mPosx = 0 ;
    mPosy = 0 ;
}
Base::~Base()
{
    free() ;
}
void Base::loadfromfile(string path)
{
    SDL_Surface *image_surface = IMG_Load(path.c_str());
    SDL_SetColorKey( image_surface, SDL_TRUE, SDL_MapRGB( image_surface->format, 0, 0xFF, 0xFF ) );
    SDL_Texture *image_texture = SDL_CreateTextureFromSurface(grenderer,image_surface) ;
    mWidth = image_surface->w ;
    mHeight = image_surface -> h ;
    mTexture = image_texture ;
}
void Base::render( SDL_Rect* clip, double angle, SDL_Point* center, SDL_RendererFlip flip )
{
	//Set rendering space and render to screen
	SDL_Rect renderQuad = { mPosx, mPosy, mWidth, mHeight };

	//Set clip rendering dimensions
	if( clip != NULL )
	{
		renderQuad.w = clip->w;
		renderQuad.h = clip->h;
	}

	//Render to screen
	SDL_RenderCopyEx( grenderer, mTexture, clip, &renderQuad, angle, center, flip );
}

class MainObject: public Base
{
public:
    MainObject() ;
    ~MainObject() ;
    bool lorr(SDL_Event &e) ;
    int vel = 10 ;
    void HandleAction(SDL_Event &e) ;
    void HandleMove() ;
private:
    int mVelx, mVely ;


};
MainObject::MainObject()
{
    mVelx = 0 ;
    mVely = 0 ;
}
MainObject::~MainObject()
{

}
bool MainObject::lorr(SDL_Event &e)
{
    if( e.type == SDL_KEYDOWN && e.key.repeat == 0 )
    {
        //Adjust the velocity
        switch( e.key.keysym.sym )
        {

            case SDLK_LEFT: return false; break;
            case SDLK_RIGHT: return true; break;
        }
    }
}
void MainObject::HandleAction( SDL_Event &e )
{
    //If a key was pressed
	if( e.type == SDL_KEYDOWN && e.key.repeat == 0 )
    {
        //Adjust the velocity
        switch( e.key.keysym.sym )
        {
            case SDLK_UP: mVely -= vel; break;
            case SDLK_DOWN: mVely += vel; break;
            case SDLK_LEFT: mVelx -= vel; break;
            case SDLK_RIGHT: mVelx += vel; break;
        }
    }
    //If a key was released
    else if( e.type == SDL_KEYUP && e.key.repeat == 0 )
    {
        //Adjust the velocity
        switch( e.key.keysym.sym )
        {
            case SDLK_UP: mVely += vel; break;
            case SDLK_DOWN: mVely -= vel; break;
            case SDLK_LEFT: mVelx += vel; break;
            case SDLK_RIGHT: mVelx -= vel; break;
        }
    }
}

void MainObject::HandleMove()
{
    //Move the dot left or right
    mPosx += mVelx;

    //If the dot went too far to the left or right
    if( ( mPosx < 0 ) || ( mPosx + mWidth > SCREEN_WIDTH ) )
    {
        //Move back
        mPosx -= mVelx;
    }

    //Move the dot up or down
    mPosy += mVely;

    //If the dot went too far up or down
    if( ( mPosy < 0 ) || ( mPosy + mHeight > SCREEN_HEIGHT ) )
    {
        //Move back
        mPosy -= mVely;
    }
}


int main(int argc, char* argv[])
{
    Base background ;
    MainObject human ;

    init() ;
    bool quit = false, LorR ;
    while(quit != true)
    {
        while(SDL_PollEvent(&e) != 0)
        {
            if(e.type == SDL_QUIT)
            {
                quit = true ;
            }
            LorR = human.lorr(e) ;
            human.HandleAction(e) ;
        }
        human.HandleMove() ;
        //clear screen
        SDL_SetRenderDrawColor( grenderer, 0xFF, 0xFF, 0xFF, 0xFF );
        SDL_RenderClear( grenderer );

        //loadbackground
        background.loadfromfile("bg4800.png") ;
        background.render() ;

        //loadnhanvat
        if(LorR)
        {
            human.loadfromfile("human64x91.png");
        }
        else
        {
            human.loadfromfile("human64x91flip.png");
        }
        human.render() ;

        //render
        SDL_RenderPresent( grenderer );

    }
    close() ;
}
