#include "lTexture.h"

void lTexture::free()
{
	if (mTexture != NULL)
	{
	SDL_DestroyTexture(mTexture);
	mTexture=NULL;
	mHeight=0;
	mWidth=0;
	}
}
lTexture::lTexture(SDL_Renderer* Renderer)
{
	mRenderer=Renderer;
	mTexture = NULL;
	mWidth = 0;
	mHeight = 0;
}

lTexture::~lTexture()
{
	free();
	mRenderer=NULL;
}

bool lTexture::setFont(TTF_Font* Font)
{
	mFont=NULL;
	mFont=Font;
	return (mFont != NULL);
	
}

bool lTexture::setRenderer(SDL_Renderer* Renderer)
{
	mRenderer=NULL;
	mRenderer=Renderer;
	return (mRenderer != NULL);
}
bool lTexture::loadfromFile(std::string path)
{
	SDL_Texture* newTexture = NULL;
	SDL_Surface* loadedSurface = IMG_Load(path.c_str());
	if (loadedSurface == NULL)
	{
		printf("Bild konnte nicht geladen werden. IMG_Error: %s\n",IMG_GetError());
	}
	else
	{
		newTexture = SDL_CreateTextureFromSurface(mRenderer,loadedSurface);
		if (newTexture == NULL)
		{
			printf("Konnte keine Textur aus %s erstellen. SDL_Error: %s\n",path.c_str(),SDL_GetError());
		}
		else
		{
			mHeight = loadedSurface->h;
			mWidth =  loadedSurface->w;
		}
	}
	SDL_FreeSurface(loadedSurface);
	mTexture=newTexture;
	return (mTexture != NULL);
}
// #ifdef _SLD_TTF_H
bool lTexture::loadfromText(std::string text,SDL_Color textcolor, TTF_Font* font)
{
	free();
	if (font != NULL)
	{mFont=font;
	}
	if (mFont != NULL)
	{
		SDL_Surface* textSurface= TTF_RenderText_Solid(mFont,text.c_str(),textcolor);
		if (textSurface==NULL)
		{
			printf("Text konnte nicht gerendert werden. TTF_Error= %s\n",TTF_GetError());
		}
		else
		{
			mTexture = SDL_CreateTextureFromSurface(mRenderer,textSurface);
			if (mTexture==NULL)
			{
				printf("Konnte keine Textur aus gerendertem Text erstellen. SDL_Error: %s\n",SDL_GetError());
			}
			else
			{
				mHeight=textSurface->h;
				mWidth=textSurface->w;
			}
			SDL_FreeSurface(textSurface);
		}
	}
	else
	{printf("Keine Schriftart angegeben!");
	}
	return (mTexture != NULL);
}
// #endif
lTexture::lTexture(std::string path,SDL_Renderer* Renderer)
{
	mRenderer=Renderer;
	loadfromFile(path);
}

int lTexture::getWidth()
{
	return mWidth;
}
int lTexture::getHeight()
{
	return mHeight;
}

void lTexture::render(SDL_Rect* space, SDL_Rect* clip,double angle,SDL_Point* center,SDL_RendererFlip flip)
{
	SDL_RenderCopyEx(mRenderer,mTexture,clip,space,angle,center,flip);
}

void lTexture::render(int x, int y, SDL_Rect* clip,double angle,SDL_Point* center,SDL_RendererFlip flip)
{
	SDL_Rect renderRect = {x,y,mWidth,mHeight};
	if (clip != NULL)
	{
		renderRect.w=clip->w;
		renderRect.h=clip->h;
	}
	render(&renderRect,clip,angle,center,flip);	
}
void lTexture::setColor(Uint8 red,Uint8 green,Uint8 blue)
{
	SDL_SetTextureColorMod(mTexture,red, green, blue);
	}
void lTexture::setAlpha(Uint8 alpha)
{
	SDL_SetTextureAlphaMod(mTexture,alpha);
}

void lTexture::setBlendMode(SDL_BlendMode blending)
{
	SDL_SetTextureBlendMode(mTexture,blending);
}
