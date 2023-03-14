#include<SDL.h>
#include<SDL_image.h>
#include<string>

const int scr_width = 1200 ;
const int scr_height = 600 ;

class Ltexture
{
public:
    // constructor
    Ltexture() ;
    ~Ltexture() ;

    // dien vao cac muc o private
    void loadfromfile(std::string image_name) ;
    void free() ;
    void render(int x, int y ) ;
    int getwidth() ;
    int getheight() ;
private:
    SDL_Texture *mtexture ;
    int mwidth ;
    int mheight ;

};
class creating
{
public:
    Ltexture xulyanh ;
};
SDL_Window *gwindow = NULL ;
SDL_Renderer *grenderer = NULL ;
SDL_Event e ;
//Ltexture object ;
Ltexture background ;
creating object ;


Ltexture::Ltexture()
{
    mtexture = NULL ;
    mwidth = 0 ;
    mheight = 0 ;
}
void Ltexture ::free()
{
    if(mtexture != NULL )
    {
        SDL_DestroyTexture(mtexture) ;
        mtexture = 0 ;
        mwidth = 0 ;
        mheight = 0 ;
    }
}
Ltexture::~Ltexture()
{
    free() ;
}
void Ltexture::loadfromfile(std::string image_name)
{
    free() ;
    SDL_Surface *image_surface = IMG_Load(image_name.c_str()) ; // lay picture anh
    SDL_SetColorKey(image_surface, SDL_TRUE, SDL_MapRGB(image_surface -> format,0,255,255)) ;
    SDL_Texture *image_texture = SDL_CreateTextureFromSurface(grenderer, image_surface) ;
    mwidth = image_surface ->w ;
    mheight = image_surface ->h ;
    SDL_FreeSurface(image_surface) ;
    mtexture = image_texture ;

}
void Ltexture::render(int x, int y)
{
    SDL_Rect renderQuad{x, y, mwidth, mheight} ;
    SDL_RenderCopy(grenderer, mtexture, NULL, &renderQuad) ;
}

bool init()
{
    gwindow = SDL_CreateWindow("game",SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, scr_width, scr_height,SDL_WINDOW_SHOWN) ;
    grenderer = SDL_CreateRenderer(gwindow, -1, SDL_RENDERER_ACCELERATED) ;
    SDL_SetRenderDrawColor(grenderer, 255, 255, 255, 255) ;
}

int main(int argc, char* argv[])
{
    init() ;
    background.loadfromfile("bg600.png");
    object.xulyanh.loadfromfile("human64x91.png");
    bool quit = false ;
    while(quit != true)
    {
        while(SDL_PollEvent(&e) != 0 )
        {
            if(e.type == SDL_QUIT)
            {
                quit = true ;
            }
        }
        SDL_SetRenderDrawColor(grenderer,0xFF,0xFF,0xFF,0xFF) ;
        SDL_RenderClear(grenderer);
        background.render(0,0) ;
        object.xulyanh.render(240,300) ;
        SDL_RenderPresent(grenderer) ;

    }


}

