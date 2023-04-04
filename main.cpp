#include <SDL.h>
#include<iostream>
#include <SDL_image.h>
#include <stdio.h>
#include <string>
#include<vector>
#include<math.h>
#include <stdlib.h>
#include <time.h>

using namespace std ;


int random(int minN, int maxN){
 return minN + rand() % (maxN + 1 - minN);
}

const double SCREEN_WIDTH = 1200;
const double SCREEN_HEIGHT = 600;
SDL_Window *gwindow = NULL ;
SDL_Renderer *grenderer = NULL ;
SDL_Surface *gsurface = NULL ;
SDL_Event e ;
void init()
{
    gwindow = SDL_CreateWindow("SDL Tutorial", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, SCREEN_WIDTH, SCREEN_HEIGHT, SDL_WINDOW_SHOWN);
    grenderer = SDL_CreateRenderer(gwindow, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC) ;
    SDL_SetRenderDrawColor( grenderer, 0xFF, 0xFF, 0xFF, 0xFF );
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
/* class Base */
class Base
{
public:
    Base() ;
    ~Base() ;
    void free() ;
    void loadfromfile(string path) ;
    //Set color modulation
    void setColor( Uint8 red, Uint8 green, Uint8 blue );
		//Set blending
    void setBlendMode( SDL_BlendMode blending );
		//Set alpha modulation
    void setAlpha( Uint8 alpha );
    void setrect(double x, double y) ;
    void render(SDL_Rect* clip = NULL, double angle = 0.0, SDL_Point* center = NULL, SDL_RendererFlip flip = SDL_FLIP_NONE ) ;
    double get_pos_x(){return mPosx;}; double get_pos_y(){return mPosy;};double get_width(){return mWidth;}; double get_height(){return mHeight;};



protected:
    SDL_Texture *mTexture ;
    double mWidth , mHeight, mPosx, mPosy ;
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
void Base::setColor( Uint8 red, Uint8 green, Uint8 blue )
{
	//Modulate texture rgb
	SDL_SetTextureColorMod( mTexture, red, green, blue );
}
void Base::setBlendMode( SDL_BlendMode blending )
{
	//Set blending function
	SDL_SetTextureBlendMode( mTexture, blending );
}
void Base::setAlpha( Uint8 alpha )
{
	//Modulate texture alpha
	SDL_SetTextureAlphaMod( mTexture, alpha );
}
void Base::loadfromfile(string path)
{
    free() ;
    SDL_Surface *image_surface = IMG_Load(path.c_str());
    //SDL_Surface *optimizedSurface = SDL_ConvertSurface( image_surface,image_surface->format,0 );
    SDL_SetColorKey( image_surface, SDL_TRUE, SDL_MapRGB(image_surface->format, 0, 0xFF, 0xFF ) );
    SDL_Texture *image_texture = SDL_CreateTextureFromSurface(grenderer,image_surface) ;
    mWidth = image_surface->w ;
    mHeight = image_surface -> h ;
    mTexture = image_texture ;
    SDL_FreeSurface(image_surface);
}
void Base::setrect(double x, double y)
{
    mPosx = x ;
    mPosy = y ;
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
/*CLASS BULLET */
class Bullet:public Base
{
public:
    Bullet() ;
    ~Bullet() ;
    bool get_move() ;
    void set_up_or_down(bool l);
    void MouseAction() ;
    void BulletMove() ;
    void SetVel(double x , double y ) ;
private:
    bool ismove, up_or_down ;
    double vel_bullet_x ;
    double vel_bullet_y ;



};
void Bullet::set_up_or_down(bool l)
{
    up_or_down = l ;
}
Bullet::Bullet()
{
    vel_bullet_x = 0 ;
    vel_bullet_y = 0 ;
    mPosx = 1200 ;
    mPosy = 0 ;
    ismove = true  ;

}
Bullet::~Bullet()
{

}
void Bullet::SetVel(double x, double y)
{
    vel_bullet_x = x ;
    vel_bullet_y = y ;
}
bool Bullet::get_move()
{
    return ismove ;
}
void Bullet::MouseAction()
{

}
void Bullet::BulletMove()
{
    if(up_or_down)
    {
        mPosx += vel_bullet_x ;
        mPosy += vel_bullet_y ;
        if(mPosx > 1200 || mPosx <0 || mPosy > 500 || mPosy <0)
        {
            ismove = false ;

        }
    }
    else
    {
        mPosy += 10 ;
        if(mPosx > 1200 || mPosx <0  || mPosy <0 || mPosy > 600)
        {
            ismove = false ;

        }
    }
}

/*CLASS MAINOBJECT */

class MainObject: public Base
{
public:
    MainObject() ;
    ~MainObject() ;

    bool lorr(SDL_Event &e) ;

    double vel = 5 ;
    void HandleAction(SDL_Event &e) ;
    void HandleMove() ;
    vector<Bullet> Bul ;

private:
    double mVelx, mVely ;



};
MainObject::MainObject()
{
    mVelx = 0 ;
    mPosy = 423 ;
    mPosx = 600 ;
    mVely = 0 ;
}
MainObject::~MainObject()
{
}
bool MainObject::lorr(SDL_Event &e)
{
    if( e.type == SDL_KEYDOWN && e.key.repeat == 0 )
    if( e.type == SDL_KEYDOWN )
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

        case SDLK_UP:
            {
                for(double i = 1 ; i <= 100 ; i++)
                {
                    mPosy -= 1 ;
                }
                break;
            }
            //case SDLK_DOWN: mVely += vel; break;
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

            case SDLK_UP:
                {
                    for(double i =1 ; i <= 100 ; i++)
                    {
                        mPosy += 1 ;
                    }
                    break;
                };
            //case SDLK_DOWN: mVely -= vel; break;
            case SDLK_LEFT: mVelx += vel; break;
            case SDLK_RIGHT: mVelx -= vel; break;
        }
    }
    else if(  e.type == SDL_MOUSEBUTTONDOWN )
    {
        int x1, y1;
        SDL_GetMouseState(&x1,&y1) ; double x = x1 , y = y1 ;
        Bullet new_bul ;
        new_bul.setrect(mPosx,mPosy) ;
        new_bul.SetVel((x-mPosx)/abs(x-mPosx)*10  , (y-mPosy)/abs(x-mPosx)*10) ;
        //cout << (x-mPosx)/abs(x-mPosx)*10  << " " <<  (y-mPosy)/abs(x-mPosx)*10 << endl ;

        Bul.push_back(new_bul) ;

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

}
 MainObject human ;
/* ClASS Threast */
class Threat : public Base
{
public:

    Threat() ;
    ~Threat() ;
    void threat_move() ;
    vector<Bullet> Bul ;
    int get_pos_cum() {return pos_cum;}
private:
    double vel_threat_x ;
    double vel_threat_y ;
    int pos_cum ;

};
Threat::Threat()
{
    mPosx = 1200 ;
    mPosy = 200 ;
}
Threat::~Threat()
{

}
void Threat::threat_move()
{

    if(mPosx == 0)
    {
        pos_cum = random(20,1180)/10*10 ;

    }
    mPosx += -10 ;
    mPosy = random(mPosy -3, mPosy +3) ;
    if(mPosx < 0)
    {
        mPosx = 1200 ;
        mPosy = random(20,200) ;
        //ismove = false ;
    }
    if(mPosx == pos_cum)
    {
        Bullet new_bul ;
        new_bul.setrect(mPosx, mPosy) ;
        new_bul.set_up_or_down(false) ;
        Bul.push_back(new_bul) ;
    }

}
bool CheckCollision( Threat object1,  Bullet object2)
{
  double left_a = object1.get_pos_x();
  double right_a = object1.get_pos_x() + object1.get_width();
  double top_a = object1.get_pos_y();
  double bottom_a = object1.get_pos_y() + object1.get_height();

  int left_b = object2.get_pos_x();
  int right_b = object2.get_pos_x() + object2.get_width();
  int top_b = object2.get_pos_y();
  int bottom_b = object2.get_pos_y() + object2.get_height();

  // Case 1: size object 1 < size object 2
  if (left_a > left_b && left_a < right_b)
  {
    if (top_a > top_b && top_a < bottom_b)
    {
      return true;
    }
  }

  if (left_a > left_b && left_a < right_b)
  {
    if (bottom_a > top_b && bottom_a < bottom_b)
    {
      return true;
    }
  }

  if (right_a > left_b && right_a < right_b)
  {
    if (top_a > top_b && top_a < bottom_b)
    {
      return true;
    }
  }

  if (right_a > left_b && right_a < right_b)
  {
    if (bottom_a > top_b && bottom_a < bottom_b)
    {
      return true;
    }
  }

  // Case 2: size object 1 < size object 2
  if (left_b > left_a && left_b < right_a)
  {
    if (top_b > top_a && top_b < bottom_a)
    {
      return true;
    }
  }

  if (left_b > left_a && left_b < right_a)
  {
    if (bottom_b > top_a && bottom_b < bottom_a)
    {
      return true;
    }
  }

  if (right_b > left_a && right_b < right_a)
  {
    if (top_b > top_a && top_b < bottom_a)
    {
      return true;
    }
  }

  if (right_b > left_a && right_b < right_a)
  {
    if (bottom_b > top_a && bottom_b < bottom_a)
    {
      return true;
    }
  }

  // Case 3: size object 1 = size object 2
  if (top_a == top_b && right_a == right_b && bottom_a == bottom_b)
  {
    return true;
  }

  return false;
}
bool CheckCollision1( MainObject object1,  Bullet object2)
{
  double left_a = object1.get_pos_x();
  double right_a = object1.get_pos_x() + object1.get_width();
  double top_a = object1.get_pos_y();
  double bottom_a = object1.get_pos_y() + object1.get_height();

  int left_b = object2.get_pos_x();
  int right_b = object2.get_pos_x() + object2.get_width();
  int top_b = object2.get_pos_y();
  int bottom_b = object2.get_pos_y() + object2.get_height();

  // Case 1: size object 1 < size object 2
  if (left_a > left_b && left_a < right_b)
  {
    if (top_a > top_b && top_a < bottom_b)
    {
      return true;
    }
  }

  if (left_a > left_b && left_a < right_b)
  {
    if (bottom_a > top_b && bottom_a < bottom_b)
    {
      return true;
    }
  }

  if (right_a > left_b && right_a < right_b)
  {
    if (top_a > top_b && top_a < bottom_b)
    {
      return true;
    }
  }

  if (right_a > left_b && right_a < right_b)
  {
    if (bottom_a > top_b && bottom_a < bottom_b)
    {
      return true;
    }
  }

  // Case 2: size object 1 < size object 2
  if (left_b > left_a && left_b < right_a)
  {
    if (top_b > top_a && top_b < bottom_a)
    {
      return true;
    }
  }

  if (left_b > left_a && left_b < right_a)
  {
    if (bottom_b > top_a && bottom_b < bottom_a)
    {
      return true;
    }
  }

  if (right_b > left_a && right_b < right_a)
  {
    if (top_b > top_a && top_b < bottom_a)
    {
      return true;
    }
  }

  if (right_b > left_a && right_b < right_a)
  {
    if (bottom_b > top_a && bottom_b < bottom_a)
    {
      return true;
    }
  }

  // Case 3: size object 1 = size object 2
  if (top_a == top_b && right_a == right_b && bottom_a == bottom_b)
  {
    return true;
  }

  return false;
}


/* Main */
int main(int argc, char* argv[])
{
    Base background ;

    Threat threat[3] ;
    threat[0] = *new Threat ;threat[1] = *new Threat ;threat[2] = *new Threat ;
    for(int i = 0 ; i < 3 ; i ++)
        {
            threat[i].setrect(1200 + i *random(100,400),random(20,100)) ;

        }
    srand(time(NULL));

    init() ;
    background.setrect(0,0) ;
    human.setrect(600,423) ;
    bool quit = false, LorR = false ;
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
        background.loadfromfile("bg600-min_optimized.png") ;
        background.render() ;

        //loadnhanvat
        if(LorR)
        {
            human.loadfromfile("mainnu.png");

        }
        else
        {
            human.loadfromfile("mainnu.png");

        }
        human.render() ;

        for(int i = 0 ; i < 3 ; i ++)
        {
            threat[i].loadfromfile("rsz_conchim.png") ;
            threat[i].threat_move() ;
            threat[i].render() ;


        for(int j = 0 ; j < threat[i].Bul.size() ; j ++ )
        {
            threat[i].Bul[j].loadfromfile("sphere.png") ;
            threat[i].Bul[j].BulletMove() ;
            threat[i].Bul[j].render() ;
            if(CheckCollision1(human,threat[i].Bul[j]))
            {
                SDL_ShowSimpleMessageBox(0,"gameover","gameover",gwindow) ;
            }
        }
        }


        for(double i = 0 ; i < human.Bul.size() ; i++)
        {

            human.Bul[i].loadfromfile("sphere.png") ;
            human.Bul[i].set_up_or_down(true);
            human.Bul[i].BulletMove();
            if(human.Bul[i].get_move())
            human.Bul[i].render() ;
            if (CheckCollision(threat[2],human.Bul[i]))
            {
                //cout << "da va cham" << endl ;
                human.Bul.erase(human.Bul.begin() + i ) ;
            }

            if(!human.Bul[i].get_move())
            {
                 human.Bul.erase(human.Bul.begin() + i ) ;
                 //cout << "daxoa va conlai "<< human.Bul.size() << endl ;
            }


        }
       //render
        SDL_RenderPresent( grenderer );
    }
    close() ;
}
