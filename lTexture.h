#ifndef lTEXTURE
#define lTEXTURE
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <stdio.h>
#include <string>
#include <SDL2/SDL_ttf.h>
class lTexture
{
	public:
	lTexture(SDL_Renderer* Renderer=NULL);
	lTexture(std::string path,SDL_Renderer* Renderer=NULL); //Initialization constructor
	~lTexture();
	bool setRenderer(SDL_Renderer* Renderer);
	bool loadfromFile(std::string path);	
// #ifdef _SDL_TTF_H
	bool loadfromText(std::string text,SDL_Color textcolor = {0,0,0},TTF_Font* Font=NULL);
// #endif
	void free();
	int getHeight();
	int getWidth();
	void render(int x, int y, SDL_Rect* clip=NULL,double angle=0,SDL_Point* center=NULL,SDL_RendererFlip flip=SDL_FLIP_NONE);
	void render(SDL_Rect* space, SDL_Rect* clip=NULL,double angle=0,SDL_Point* center=NULL,SDL_RendererFlip flip=SDL_FLIP_NONE);
	bool setFont(TTF_Font* Font);
	void setColor(Uint8 red,Uint8 green,Uint8 blue);
	void setAlpha(Uint8 alpha);
	void setBlendMode(SDL_BlendMode blending);
	private:
	SDL_Renderer* mRenderer;
	int mWidth;
	int mHeight;
	TTF_Font* mFont;
	SDL_Texture* mTexture;
};
#endif
