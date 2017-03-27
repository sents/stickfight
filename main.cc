/*
 * =====================================================================================
 *
 *       Filename:  main.cc
 *
 *    Description:  A simple fighting game
 *
 *        Version:  1.0
 *        Created:  21.03.2017 16:39:34
 *       Revision:  none
 *       Compiler:  gcc
 *
 *         Author:  Finn Krein, finnkrein@googlemail.com; Arvid Krein, arvid2000@googlemail.com
 *        Company:  -
 *
 * =====================================================================================
 */

#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <iostream>
#include <stdio.h>
#include <string>
#include <cmath>
#include <vector>
#include "utility.h"
#include "lTexture.h"
#include "physobj.h"
#include "bezier.h"

void drawobjs(SDL_Renderer* Renderer, std::vector<kraftpartikel*> vObj);
int KASTENGROSSE = 5;
const int SCREEN_WIDTH = 800;
const int SCREEN_HEIGHT = 600;

int main (/*int argc, char* args[])*/)
{
	SDL_Window* gWindow;
	SDL_Renderer* gRenderer;
	init(&gWindow,&gRenderer, SCREEN_HEIGHT, SCREEN_WIDTH);
	bool quit=false;
	SDL_Event event;
	simpleTimer timer;
	int mousex;
	int mousey;
	SDL_Rect drawnRect = {SCREEN_WIDTH/2, SCREEN_HEIGHT/2, 4 ,4};
	Worldframe world;
	world.xsize = SCREEN_WIDTH;
	world.ysize = SCREEN_HEIGHT;	
	while (quit!=true)
	{
		while(SDL_PollEvent(&event)!=0)
		{
			if(event.type == SDL_QUIT)
				quit=true;
			if(event.type == SDL_KEYDOWN)
			{
				switch (event.key.keysym.sym)
				{
					case SDLK_p:
						timer.flip();
						break;
					case SDLK_o:
						timer.reset();
						break;
					case SDLK_i:
						std::cout << timer.get() << "\n" << std::flush;
						break;
					case SDLK_n:
						world.vKPartikel.push_back(new kraftpartikel(static_cast<double>(mousex), static_cast<double>(mousey)));
				}	

			}

			if (event.type == SDL_MOUSEBUTTONDOWN)
			{
				SDL_GetMouseState(&mousex, &mousey);
				drawnRect.x=mousex;
				drawnRect.y=mousey;
			}
		}
		if( timer.get() >= (1000/25) )
		{
		world.iterate(0.00001);
		SDL_SetRenderDrawColor(gRenderer, 0xFF, 0xFF, 0xFF, 0xFF);
		SDL_RenderClear(gRenderer);
		SDL_SetRenderDrawColor(gRenderer, 0x00, 0x00, 0x00, 0xFF);
		drawobjs(gRenderer, world.vKPartikel);
		SDL_RenderDrawRect(gRenderer, &drawnRect);
		SDL_RenderPresent(gRenderer);
		timer.reset();

		}
	}
	return 0;
}

void drawobjs(SDL_Renderer* Renderer, std::vector<kraftpartikel*> vObj)
{
	for (auto i : vObj)
	{
		SDL_Rect objRect = {0,0,KASTENGROSSE,KASTENGROSSE};
		objRect.x=i->getx();
		objRect.y=i->gety();
		SDL_SetRenderDrawColor(Renderer,0x00,0x00,0x00,0xFF);
		SDL_RenderFillRect(Renderer,&objRect);
	}
}

