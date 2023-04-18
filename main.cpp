#include <SDL.h>
#include <iostream>
#include <SDL_image.h>
#include <stdio.h>
#include <string>
#include <vector>
#include <math.h>
#include <stdlib.h>
#include <time.h>
#include <SDL_ttf.h>
#include <fstream>
using namespace std;
float absd(float x)
{
	if (x > 0.0) return x;
	return -x;
}

int lay_ra_o( float x1)
{
    int x = x1 ;
    return (x + 30) / 60  ;
}
int bien_duoi = 415 ,bien_ngang_left = 0, bien_ngang_right = 1200;

int gioihan(int x)
{
	switch (x)
	{
		case 1:
			return 0;
			break;
		case 2:
			return 1;
			break;
		case 3:
			return 1;
			break;
		case 4:
			return 2;
			break;
		case 5:
			return 2;
			break;
		case 6:
			return 3;
			break;
		case 7:
			return 3;
			break;
		case 8:
			return 4;
			break;
		case 9:
			return 4;
			break;
			// case 10 : return 0 ; break ;
	}
}

int soluongchim(int x)
{
	switch (x)
	{
		case 1:
			return 8;
			break;
		case 2:
			return 10;
			break;
		case 3:
			return 6;
			break;
		case 4:
			return 14;
			break;
		case 5:
			return 12;
			break;
		case 6:
			return 10;
			break;
		case 7:
			return 10;
			break;
		case 8:
			return 22;
			break;
		case 9:
			return 10;
			break;
			// case 10 : return 0 ; break ;
	}
}

string doi_so_sang_text(int a)
{
	string ans = "";
	int du;
	while (a > 0)
	{
		du = a % 10;
		char c = du + '0';
		ans = c + ans;
		a = a / 10;
	}

	return ans;
}

int random(int minN, int maxN)
{
	return minN + rand() % (maxN + 1 - minN);
}

const float SCREEN_WIDTH = 1200;
const float SCREEN_HEIGHT = 600;
int vel_chim = 0, so_luong_dan;
SDL_Window *gwindow = NULL;
SDL_Renderer *grenderer = NULL;
SDL_Surface *gsurface = NULL;
TTF_Font *gfont = NULL;

SDL_Event e;
void init()
{
	gwindow = SDL_CreateWindow("SDL Tutorial", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, SCREEN_WIDTH, SCREEN_HEIGHT, SDL_WINDOW_SHOWN);
	grenderer = SDL_CreateRenderer(gwindow, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);

	SDL_SetRenderDrawColor(grenderer, 0xFF, 0xFF, 0xFF, 0xFF);
}

void close()
{
	//Destroy window
	SDL_DestroyRenderer(grenderer);
	SDL_DestroyWindow(gwindow);
	gwindow = NULL;
	grenderer = NULL;
	//Quit SDL subsystems
	IMG_Quit();
	SDL_Quit();
}

/*class Base */
class Base
{
	public:
		Base();
	~Base();
	void free();
	void loadfromfile(string path);
	//Set color modulation
	void setColor(Uint8 red, Uint8 green, Uint8 blue);
	//Set blending
	void setBlendMode(SDL_BlendMode blending);
	//Set alpha modulation
	void setAlpha(Uint8 alpha);

	void setrect(float x, float y);
	int hp;

	void render(SDL_Rect *clip = NULL, float angle = 0.0, SDL_Point *center = NULL, SDL_RendererFlip flip = SDL_FLIP_NONE);
	float get_pos_x()
	{
		return mPosx;
	};

	float get_pos_y()
	{
		return mPosy;
	};

	float get_width()
	{
		return mWidth;
	};

	float get_height()
	{
		return mHeight;
	};

	void set_hp(int x)
	{
		hp = x;
	};

	protected:
		SDL_Texture * mTexture;
	float mWidth, mHeight, mPosx, mPosy;

};

void Base::free()
{
	if (mTexture != NULL)
	{
		SDL_DestroyTexture(mTexture);
		mTexture = NULL;
		mWidth = 0;
		mHeight = 0;
	}
}

Base::Base()
{
	mTexture = NULL;
	mWidth = 0;
	mHeight = 0;
	mPosx = 0;
	mPosy = 0;
}

Base::~Base()
{
	free();
}

void Base::setColor(Uint8 red, Uint8 green, Uint8 blue)
{
	//Modulate texture rgb
	SDL_SetTextureColorMod(mTexture, red, green, blue);
}

void Base::setBlendMode(SDL_BlendMode blending)
{
	//Set blending function
	SDL_SetTextureBlendMode(mTexture, blending);
}

void Base::setAlpha(Uint8 alpha)
{
	//Modulate texture alpha
	SDL_SetTextureAlphaMod(mTexture, alpha);
}

void Base::loadfromfile(string path)
{
	free();
	SDL_Surface *image_surface = IMG_Load(path.c_str());
	//SDL_Surface *optimizedSurface = SDL_ConvertSurface(image_surface,image_surface->format,0);
	SDL_SetColorKey(image_surface, SDL_TRUE, SDL_MapRGB(image_surface->format, 0, 0xFF, 0xFF));
	SDL_Texture *image_texture = SDL_CreateTextureFromSurface(grenderer, image_surface);
	mWidth = image_surface->w;
	mHeight = image_surface->h;
	mTexture = image_texture;
	SDL_FreeSurface(image_surface);
}

void Base::setrect(float x, float y)
{
	mPosx = x;
	mPosy = y;
}

void Base::render(SDL_Rect *clip, float angle, SDL_Point *center, SDL_RendererFlip flip)
{
	//Set rendering space and render to screen
	SDL_Rect renderQuad = { mPosx, mPosy, mWidth, mHeight
	};

	//Set clip rendering dimensions
	if (clip != NULL)
	{
		renderQuad.x = 0;
		renderQuad.y = 0;
		renderQuad.w = clip->w;
		renderQuad.h = clip->h;
	}

	//Render to screen
	SDL_RenderCopyEx(grenderer, mTexture, clip, &renderQuad, angle, center, flip);

}

class Map_game
{
public:
    Map_game() ;
    ~Map_game() ;
private:

};
/*CLASS TEXT */
class TextObject: public Base
{
	public: enum TextColor
	{
		RED_TEXT = 0,
			WHITE_TEXT = 1,
			BLACK_TEXT = 2,
	};

	TextObject();~TextObject();
	void Set_Text(string path1);
	void Set_Color(int type);
	void loadFromRenderedText();
	void Show_Text()
	{
		cout << text;
	};

	private: string text;
	SDL_Color text_color;
};

TextObject::TextObject()
{
	mTexture = NULL;
	mWidth = 0;
	mHeight = 0;
	mPosx = 500;
	mPosy = 200;
	text = "";
	text_color.r = 255;
	text_color.g = 255;
	text_color.b = 255;

}

TextObject::~TextObject() {}

void TextObject::loadFromRenderedText()
{
	//Get rid of preexisting texture
	free();
	//Render text surface
	SDL_Surface * textSurface;
	textSurface = TTF_RenderText_Solid(gfont, text.c_str(), text_color);
	if (textSurface != NULL)
	{
		//Create texture from surface pixels
		mTexture = SDL_CreateTextureFromSurface(grenderer, textSurface);
		if (mTexture == NULL)
		{
			printf("Unable to create texture from rendered text! SDL Error: %s\n", SDL_GetError());
		}
		else
		{
			//Get image dimensions
			mWidth = textSurface->w;
			mHeight = textSurface->h;
		}

		//Get rid of old surface
		SDL_FreeSurface(textSurface);
	}
	else cout << "LOI";
}

void TextObject::Set_Text(string path1)
{
	text = path1;
}

void TextObject::Set_Color(int type)
{
	if (type == RED_TEXT)
	{
		SDL_Color color = { 255, 0, 0, 0xFF
		};

		text_color = color;
	}
	else if (type == WHITE_TEXT)
	{
		SDL_Color color = { 255, 255, 255, 0xFF
		};

		text_color = color;
	}
	else
	{
		SDL_Color color = { 0, 0, 0, 0xFF
		};

		text_color = color;
	}
}

TextObject Score;


/* CLASS ITEM */
class Item :public Base
{
public:
    Item() ;
    ~Item(){;} ;
    int ten_item ;
    void item_move() ;
    Uint32 time_ton_tai ;
    //int set_item () ;
private:
    int item_bien ;



};
Item::Item()
{
    item_bien = 425 ;
    ten_item = 0 ;
    time_ton_tai  = 0 ;
}

void Item::item_move()
{
    if (mPosy < item_bien)
    {
        mPosy += 5 ;
    }
}


/*CLASS BULLET */
class Bullet: public Base
{
	public: Bullet();~Bullet();
	bool get_move();
	void set_up_or_down(bool l);
	void MouseAction();
	void BulletMove();
	void SetVel(float x, float y);
	bool da_lay_muctieu;
	void get_muctieu(float x, float y)
	{
		muctieu_x = x;
		muctieu_y = y;
	}

	private: bool ismove, up_or_down;
	float vel_bullet_x;
	float vel_bullet_y;
	float muctieu_x, muctieu_y;

};

void Bullet::set_up_or_down(bool l)
{
	up_or_down = l;
}

Bullet::Bullet()
{
	vel_bullet_x = 0;
	vel_bullet_y = 0;
	mPosx = 1200;
	mPosy = 0;
	ismove = true;
	da_lay_muctieu = false;
	muctieu_x = 0;
	muctieu_y = 0;

}

Bullet::~Bullet() {}

void Bullet::SetVel(float x, float y)
{
	vel_bullet_x = x;
	vel_bullet_y = y;
}

bool Bullet::get_move()
{
	return ismove;
}

void Bullet::MouseAction() {}

void Bullet::BulletMove()
{
	if (up_or_down)
	{
		mPosx += vel_bullet_x;
		mPosy += vel_bullet_y;
		if (mPosx > 1200 || mPosx < 0 || mPosy > 600 || mPosy < 0)
		{
			ismove = false;
		}
	}
	else
	{
		if (mPosx > 1200 || mPosx < 0 || mPosy < 0 || mPosy > 800)
		{
			ismove = false;
		}

		mPosx += vel_bullet_x;
		mPosy += vel_bullet_y;
	}
}
/*claAzdxszsaXC `XX SRRZAQSXDCss hieu ung  */
///
class Hieu_ung: public Base
{
	public: Hieu_ung();

	void render_sprite(float angle = 0.0, SDL_Point *center = NULL, SDL_RendererFlip flip = SDL_FLIP_NONE);
	void set_frame(int f)
	{
		so_frame = f;
	}

	void set_clip();

	private: int so_frame;
	SDL_Rect clip[7];

};

Hieu_ung::Hieu_ung()
{
	clip[0].x = 0;
	clip[0].y = 0;
	clip[0].w = 480 / 8;
	clip[0].h = 64;

	clip[1].x = 480 / 8;
	clip[1].y = 0;
	clip[1].w = 480 / 8;
	clip[1].h = 64;

	clip[2].x = 2 * 480 / 8;
	clip[2].y = 0;
	clip[2].w = 480 / 8;
	clip[2].h = 64;

	clip[3].x = 3 * 480 / 8;
	clip[3].y = 0;
	clip[3].w = 480 / 8;
	clip[3].h = 64;

	clip[4].x = 4 * 480 / 8;
	clip[4].y = 0;
	clip[4].w = 480 / 8;
	clip[4].h = 64;

	clip[5].x = 5 * 480 / 8;
	clip[5].y = 0;
	clip[5].w = 480 / 8;
	clip[5].h = 64;

	clip[6].x = 6 * 480 / 8;
	clip[6].y = 0;
	clip[6].w = 480 / 8;
	clip[6].h = 64;

	clip[7].x = 7 * 480 / 8;
	clip[7].y = 0;
	clip[7].w = 480 / 8;
	clip[7].h = 64;

}

void Hieu_ung::render_sprite(float angle, SDL_Point *center, SDL_RendererFlip flip)
{
	//Set rendering space and render to screen
	SDL_Rect renderQuad = { mPosx, mPosy, mWidth / 8, mHeight
	};

	//Set clip rendering dimensions
	//renderQuad = clip[so_frame] ;

	//Render to screen
	SDL_RenderCopyEx(grenderer, mTexture, &clip[so_frame], &renderQuad, angle, center, flip);

}

/*CLASS MAINOBJECT */

class MainObject: public Base
{
	public: MainObject();~MainObject();

	int lorr(SDL_Event & e);

	float vel = 5;
	void HandleAction(SDL_Event & e);
	void HandleMove();
	void set_rong_dai(int x, int y) {mWidth = x; mHeight = y ;}
	vector<Bullet> Bul;
	float mVelx, mVely;
	Hieu_ung hieu_ung_main ;


	private:

};

MainObject::MainObject()
{
	mVelx = 0;
	mPosy = 0;
	mPosx = 600;
	mVely = 0;
	hp = 100;
}

MainObject::~MainObject() {}

int MainObject::lorr(SDL_Event & e)
{
    //int u ;
	//if(e.type == SDL_KEYDOWN && e.key.repeat == 0)
		//Adjust the velocity
		if (e.key.keysym.sym == SDLK_LEFT)
		{
		    //cerr<<e.key.keysym.sym <<endl   ;
			return 97;
		}
		else if (e.key.keysym.sym == SDLK_RIGHT)
		{
		    //cerr << e.key.keysym.sym << endl ;
			return 100;
		}

	//return 999 ;


}

void MainObject::HandleAction(SDL_Event & e)
{
    //cout << "dang_xet" << endl ;

	//If a key was pressed
	if (e.type == SDL_KEYDOWN && e.key.repeat == 0)
	{
		//Adjust the velocity
		 if(e.key.keysym.sym == SDLK_RIGHT){
				mVelx += 10;
				//cout << "thoat_nhan_phim d" << endl ;
				//cerr << e.key.keysym.sym << endl ;
		 }
		else if (e.key.keysym.sym == SDLK_LEFT)
		{


				mVelx -= 10;
				//cout << "thoat_nhan_phim a" << endl ;

				}



			else if(e.key.keysym.sym == SDLK_UP  )
            {
                for(int i = 0 ; i <= 200 ; i ++)
                {
                    mPosy -- ;
                }
            }

	}

	//If a key was released
	else if (e.type == SDL_KEYUP && e.key.repeat == 0)
	{
		//Adjust the velocity
		if (e.key.keysym.sym == SDLK_LEFT)
		{


				mVelx += 10;
				//cout << "thoat_nhan_phim a" << endl ;
				}
        else if(e.key.keysym.sym == SDLK_RIGHT){
				mVelx -= 10;
				//cout << "thoat_nhan_phim d" << endl ;

			}

		}

	else if (e.type == SDL_MOUSEBUTTONDOWN)
	{
		int x1, y1;
		SDL_GetMouseState(&x1, &y1);
		float x = x1, y = y1;
		Bullet new_bul;
		new_bul.setrect(mPosx, mPosy);
		new_bul.SetVel((x - mPosx) / abs(x - mPosx) *10, (y - mPosy) / abs(x - mPosx) *10);
		//cout << (x-mPosx)/abs(x-mPosx)*10  << " " <<  (y-mPosy)/abs(x-mPosx)*10 << endl ;

		Bul.push_back(new_bul);
		so_luong_dan--;
	}
}

void MainObject::HandleMove()
{
	//Move the dot left or right
	mPosx += mVelx;
	//cout << mVelx << " " << mPosx<< endl ;
	//If the dot went too far to the left or right
	if ((mPosx <= bien_ngang_left) )
	{
		//Move back
		mPosx += (bien_ngang_left-mPosx) ;
	}
	if( (mPosx+60 >= bien_ngang_right))
    {
        mPosx += bien_ngang_right - (mPosx+60);
    }
	//Move the dot up or down
	if(mPosy <= bien_duoi)
    {
        mPosy += 10 ;
    }

	//If the dot went too far up or down

}

MainObject human;
/*ClASS Threast */
class Threat: public Base
{
	public:

		Threat();~Threat();
	void threat_move();
	vector<Bullet> Bul;
	void set_vector_move(bool l)
	{
		move_left_or_right = l;
	}

	bool get_vector()
	{
		return move_left_or_right;
	}

	//int get_pos_cum() {return pos_cum;}

	private: float vel_threat_x;
	float vel_threat_y;
	int pos_cum1, pos_cum2, pos_cum3;
	bool move_left_or_right;

};

Threat::Threat()
{
	mPosx = 1200;
	mPosy = 200;
	pos_cum1 = 1200;
	pos_cum2 = 1200;
	pos_cum3 = 1200;
}

Threat::~Threat() {}

void Threat::threat_move()
{
	if (move_left_or_right == false)
	{
		if (mPosx == 0)
		{
			pos_cum1 = random(0, 1200) / 5 * 5;
			pos_cum2 = random(0, 1200) / 5 * 5;
			// pos_cum3 = random(0,1200)/5 * 5 ;
		}

		mPosx += -5 + vel_chim;
		mPosy = random(mPosy - 3, mPosy + 3);
		if (mPosx < 0)
		{
			mPosx = 3000;
			mPosy = random(20, 200);
			hp = 20;
			//ismove = false ;
		}

		if (mPosx == pos_cum1 || mPosx == pos_cum2)
		{
			Bullet new_bul;
			new_bul.setrect(mPosx, mPosy);
			new_bul.set_up_or_down(false);
			Bul.push_back(new_bul);
		}
	}
	else
	{
		if (mPosx == 0)
		{
			pos_cum1 = random(0, 1200) / 5 * 5;
			pos_cum2 = random(0, 1200) / 5 * 5;
			//  pos_cum3 = random(0,1200)/5 * 5 ;
		}

		mPosx += 5 + vel_chim;
		mPosy = random(mPosy - 3, mPosy + 3);
		if (mPosx > 1200)
		{
			mPosx = -1800;
			mPosy = random(20, 200);
			hp = 20;
			//ismove = false ;
		}

		if (mPosx == pos_cum1 || mPosx == pos_cum2)
		{
			Bullet new_bul;
			new_bul.setrect(mPosx, mPosy);
			new_bul.set_up_or_down(false);
			Bul.push_back(new_bul);
		}
	}
}

/*check va cham */
bool CheckCollision(Threat object1, Bullet object2)
{
	float left_a = object1.get_pos_x();
	float right_a = object1.get_pos_x() + object1.get_width();
	float top_a = object1.get_pos_y();
	float bottom_a = object1.get_pos_y() + object1.get_height();

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

bool CheckCollision1(MainObject object1, Bullet object2)
{
	float left_a = object1.get_pos_x();
	float right_a = object1.get_pos_x() + object1.get_width();
	float top_a = object1.get_pos_y();
	float bottom_a = object1.get_pos_y() + object1.get_height();

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



Base pause;
int map_index[20][10] ;
TextObject text_level, text_hp, text_task, text_dan; // text
bool win_task = false, pause_game = false;   // trang thai game
int level, so_luong_chim, gioi_han_tren, kiem_soat_gioi_han, task;  // thong so level
Hieu_ung vu_no, test_frame;
vector<Item> list_item ;
vector<string> list_chim
{
	"chim_nau.png", "chim_vang.png", "chim_bom.png", "chim_xanh.png", "pig_fly.png" };

/*Main Main Main Main Main Main Main Main Main Main Mian Main Main Main Main Mian Main Main Main Main Mian Main Main Main Main Mian Main */
int main(int argc, char *argv[])
{
	/*xu ly level */
	ifstream ip("du_lieu_game.txt");
	ifstream ip_map("map_text.txt") ;
	ofstream op("du_lieu_game1.txt");
	if (ip)
	{
		cout << " success ";
	}
	int frame_right = 0 , frame_left = 0  ;

	ip >> level;
	for(int i = 8 ; i < 10 ; i ++ )
    {
        for(int j = 0 ; j < 20 ; j++ )
        {
            ip_map >> map_index[j][i] ;
        }
    }
    for(int i = 0 ; i< 20 ; i++)
    {
        for(int j = 0 ; j<10 ; j++)
        {
            cout << map_index[i][j] << " " ;
        }
        cout << endl ;
    }
    cout << map_index[9][7] << endl  ;
	/*kiem_soat_chim */
	if (level % 2 == 0)
	{
		kiem_soat_gioi_han = 0;
	}
	else
	{
		kiem_soat_gioi_han = 1;
	}

	task = 5 + level * 5;
	human.set_rong_dai(60, 60) ;
	gioi_han_tren = gioihan(level);
	so_luong_chim = soluongchim(level);
	so_luong_dan = 50;

	string chim_at_level[so_luong_chim];

	TTF_Init();
	gfont = TTF_OpenFont("Xerox Sans Serif Wide Bold.ttf", 28);

	bool vao_game = false;
	Base background;

	Threat threat[so_luong_chim];

	srand(time(NULL));

	init();
	background.setrect(0, 0);
	human.setrect(600, -100);
	bool quit = false ;
	int LorR = 97 ;

	/*loop */
	while (quit != true)
	{
		while (SDL_PollEvent(&e) != 0)
		{
			if (e.type == SDL_QUIT)
			{
				quit = true;
			}

			if (vao_game == true)
			{
				human.HandleAction(e);
				//cout << "imhere" << endl ;
						//human.HandleMove();
				if (e.type == SDL_KEYDOWN && (e.key.keysym.sym == SDLK_LEFT || e.key.keysym.sym == SDLK_RIGHT))
					LorR = human.lorr(e);
					//cout << LorR << endl ;

				if (pause_game == false)
				{
					if (e.type == SDL_MOUSEBUTTONDOWN)
					{
						int x, y;
						SDL_GetMouseState(&x, &y);
						{
							if (1054 <= x && 1200 >= x && 0 <= y && 80 >= y)
							{
								pause_game = true;
							}
						}
					}
				}
				else // if pause_game = true
				{
					if (e.type == SDL_MOUSEBUTTONDOWN)
					{
						int x, y;
						SDL_GetMouseState(&x, &y);
						{
							if (400 <= x && 600 >= x && 30 <= y && 80 >= y)
							{
								pause_game = false;
							}
							else if (400 <= x && 600 >= x && 200 <= y && 300 <= y)
							{
								quit = true;
							}
						}
					}
				}

			}

			// xu ly vao game hay khong
			else if (e.type == SDL_MOUSEBUTTONDOWN)
			{
				int x, y;
				SDL_GetMouseState(&x, &y);
				if (500 <= x && 700 >= x && 50 <= y && 150 >= y)
				{
					vao_game = true;
					human.hp = 100;
					human.setrect(600,0) ;
					so_luong_dan = 200;
					for (int i = 0; i < so_luong_chim; i++)
					{
						if (i % 2 == 0)
						{
							chim_at_level[i] = list_chim[gioi_han_tren - 1 + kiem_soat_gioi_han];
						}
						else
						{
							chim_at_level[i] = list_chim[gioi_han_tren];
						}
					}

					for (int i = 0; i < so_luong_chim; i++)
					{
						if (i % 2 == 0)
						{
							threat[i].set_vector_move(false);
							threat[i].setrect(1200 + i* random(100, 400), random(20, 100));
						}
						else
						{
							threat[i].set_vector_move(true);
							threat[i].setrect(0 - i* random(100, 400), random(20, 100));
						}

						threat[i].set_hp(20);
					}
				}
			}
		}
		human.HandleMove();




		if (vao_game == false)
		{
			background.loadfromfile("menu1-min.png");
			background.setrect(0,0) ;
			background.render();
		}

		if (vao_game == true)
		{
			//clear screen


			SDL_SetRenderDrawColor(grenderer, 0xFF, 0xFF, 0xFF, 0xFF);
			SDL_RenderClear(grenderer);

			//loadbackground
			background.loadfromfile("back_ground_buoi_chieu.png");
				background.setrect(0,0) ;
				background.render();

				for(int o = 0 ; o <= 19 ; o ++ )
                {
                    //int tambo,m ;
                    for( int m = 8 ; m <= 9 ; m ++ )
                    {
                        if(map_index[o][m] == 1)
                        {
                            background.loadfromfile("2.png");
                            background.setrect(o*60,m*60) ;
                            background.render() ;
                        }

                    }
                }

			// pause game
			pause.loadfromfile("pause1.png");
			pause.setrect(1000, 0);
			pause.render();

			// level ;
			text_level.Set_Color(1);
			text_level.Set_Text("LEVEL " + doi_so_sang_text(level));
			text_level.setrect(20, 20);
			text_level.loadFromRenderedText();
			text_level.render();

			if (pause_game == true )
			{
				background.loadfromfile("bg600.png");
				background.setrect(0,0) ;
				background.render();
				background.loadfromfile("2.png");
				for(int o = 0 ; o <= 1140 ; o += 60 )
                {
                    for(int m = 480 ; m <= 540 ; m += 60 )
                    {
                        background.setrect(o,m) ;
                        background.render() ;

                    }
                }

			}
			else if (win_task == false && human.hp > 0 && so_luong_dan > 0)
			{
			 	//loadnhanvat
			 	int x =  lay_ra_o(human.get_pos_x()) ;
			 	int y = lay_ra_o(human.get_pos_y() + 10 )  ;


			 	if(map_index[x][8] == 0 )
			 	{
			 	    bien_duoi = 475;

			 	}
                else bien_duoi = 415 ;


                if(map_index[x-1][y] == 1 )
                {
                    bien_ngang_left = 60 * (x) ;
                }
                else
                {
                    bien_ngang_left = 0;
                }
                if(map_index[x+1][y] == 1)
                {
                    bien_ngang_right  = 60 * (x + 1) ;

                }
                else
                {
                    bien_ngang_right = 1200 ;
                }


			 	//cout << x << " " << y << " " << map_index[x-1][y] << " " << map_index[x+1][y]<< " " << bien_ngang_left << " " << bien_ngang_right << " " << human.get_pos_x()<< " " << human.get_pos_y() <<  endl ;

			 	//human.HandleMove();

				if (LorR == 100)
				{
				   // /*
					human.hieu_ung_main.loadfromfile("player_right.png");
					human.hieu_ung_main.setrect(human.get_pos_x(),human.get_pos_y()) ;
					human.hieu_ung_main.set_frame(frame_right) ;
					//SDL_Delay(50) ;
					human.hieu_ung_main.render_sprite() ;
					if(human.mVelx !=0) frame_right ++ ;
					else frame_right = 0 ;
					if(frame_right == 7) frame_right = 0 ;
					//cout << frame_right << endl ;
				//	*/

					//human.loadfromfile("guy.png") ;

				}
				else if(LorR == 97)
				{
				    // /*
					human.hieu_ung_main.loadfromfile("player_left.png");
					human.hieu_ung_main.setrect(human.get_pos_x(),human.get_pos_y()) ;
					human.hieu_ung_main.set_frame(frame_left) ;
					human.hieu_ung_main.render_sprite() ;
					if(human.mVelx !=0) frame_left ++ ;
					else frame_left = 0 ;
					if(frame_left == 7) frame_left = 0 ;
                    //    */

                       // human.loadfromfile("guy_flip.png") ;
				}

				//cout << LorR << endl ;
				//human.render();
				human.set_rong_dai(60, 60) ;

				for (int i = 0; i < so_luong_chim; i++)
				{
					threat[i].loadfromfile(chim_at_level[i]);
					threat[i].threat_move();
					threat[i].render();

					for (int j = 0; j < threat[i].Bul.size(); j++)
					{
						threat[i].Bul[j].loadfromfile("egg.png");
						if (threat[i].Bul[j].da_lay_muctieu == false)
						{
							threat[i].Bul[j].get_muctieu(human.get_pos_x(), human.get_pos_y());
							threat[i].Bul[j].SetVel((human.get_pos_x() - threat[i].Bul[j].get_pos_x()) / absd(human.get_pos_x() - threat[i].Bul[j].get_pos_x()) *5, (human.get_pos_y() - threat[i].Bul[j].get_pos_y()) / absd(human.get_pos_x() - threat[i].Bul[j].get_pos_x()) *5);
							threat[i].Bul[j].da_lay_muctieu = true;
						}

						threat[i].Bul[j].BulletMove();

						threat[i].Bul[j].render();
						if (CheckCollision1(human, threat[i].Bul[j]) == true || threat[i].Bul[j].get_move() == false)
						{
							if (CheckCollision1(human, threat[i].Bul[j]) == true)
							{
								human.hp -= 1;
							}

							threat[i].Bul.erase(threat[i].Bul.begin() + j);
						}
					}
				}

				for (int k = 0; k < human.Bul.size(); k++)
				{
					human.Bul[k].loadfromfile("sphere.png");
					human.Bul[k].set_up_or_down(true);
					human.Bul[k].BulletMove();
					human.Bul[k].set_hp(10);
					if (human.Bul[k].get_move())
						human.Bul[k].render();
					for (int i = 0; i < so_luong_chim; i++)
					{
						if (CheckCollision(threat[i], human.Bul[k]))
						{
							human.Bul.erase(human.Bul.begin() + k);
							threat[i].hp -= human.Bul[k].hp;
							if (threat[i].hp == 0)
							{
							    Item new_item ;
							    new_item.ten_item = random(1,3) ;
							    new_item.time_ton_tai = SDL_GetTicks() ;
							    new_item.setrect(threat[i].get_pos_x(),threat[i].get_pos_y()) ;
							    list_item.push_back(new_item) ;

							    				{ 						for (int l = 0; l <= 4; l++)
									{
										vu_no.setrect(threat[i].get_pos_x() + 3 *l, threat[i].get_pos_y() + 3 *l);
										vu_no.loadfromfile("exp.png");
										vu_no.set_frame(l);
										vu_no.render_sprite();
										//SDL_Delay(100) ;
										//SDL_RenderPresent(grenderer) ;
										//SDL_Delay(100) ;
									}

									//SDL_Delay(150) ;
								}

								threat[i].hp = 20;
								if (threat[i].get_vector() == false)
									threat[i].setrect(3000, random(20, 100));
								else threat[i].setrect(-1800, random(20, 100));
								task--;
								if (task == 0)
								{
									win_task = true;
								}
							}
						}
					}

					if (!human.Bul[k].get_move())
					{
						human.Bul.erase(human.Bul.begin() + k);
						//cout << "daxoa va conlai "<< human.Bul.size() << endl ;
					}
				}
				for(int li = 0 ; li < list_item.size() ; li ++)
                {

                    if(SDL_GetTicks() - list_item[li].time_ton_tai >= 8000.0 )
                    {
                        list_item.erase(list_item.begin() + li) ;
                        //continue ;
                    }

                    //cout << list_item[li].time_ton_tai << endl ;
                    switch(list_item[li].ten_item)
                    {
                        case 1:list_item[li].loadfromfile("trai_tim_be_nho.png") ; break ;
                        case 2:list_item[li].loadfromfile("hao_quang_vip.png") ; break ;
                        case 3:list_item[li].loadfromfile("hao_quang_vip.png") ; break ;
                    }
                    list_item[li].item_move() ;
                    list_item[li].render() ;
                }

				if (so_luong_dan == 0)
				{
					win_task = false;
				}

				// show score ;
				text_hp.Set_Color(1);
				text_hp.Set_Text("HP " + doi_so_sang_text(human.hp));
				text_hp.setrect(20, 60);
				text_hp.loadFromRenderedText();
				text_hp.render();

				text_task.Set_Color(1);
				text_task.Set_Text("TASK REMAIN " + doi_so_sang_text(task));
				text_task.setrect(20, 90);
				text_task.loadFromRenderedText();
				text_task.render();

				text_dan.Set_Color(1);
				text_dan.Set_Text("Bullet " + doi_so_sang_text(so_luong_dan));
				text_dan.setrect(20, 120);
				text_dan.loadFromRenderedText();
				text_dan.render();
			}
			else if (win_task == false && (human.hp <= 0 || so_luong_dan <= 0))
			{
				vao_game = false;
			}
		}

		//render
		//cout << vao_game<< endl ;
		SDL_RenderPresent(grenderer);

		SDL_RenderClear(grenderer);
	}


	SDL_GetError() ;
	close();
	op << level;


	ifstream ip1("du_lieu_game1.txt");
	ofstream op1("du_lieu_game.txt");
	//level ++ ;
	ip1 >> level;
	op1 << level;
	return 0;
}
