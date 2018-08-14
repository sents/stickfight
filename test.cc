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
#include "vec2.h"
#include "stickman.h"

int KASTENGROSSE = 5;
const int FRAMERATE = 25;

SDL_Point SDP(vec2 Point)
{
	return {static_cast<int>(std::round(Point.X)),static_cast<int>(std::round(Point.Y))};

}


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
	vec2 P1 = {100,100};
	vec2 P2 = {110,100};
	vec2 P3 = {110,110};
	vec2 P4 = {100,110};
	std::array<vec2,4> Ps = {P1,P2,P3,P4};
	std::vector<vec2> PHull = hull(Ps);
	vec2 K1 = {100,100};
	vec2 K2 = {110,100};
	vec2 K3 = {110,110};
	vec2 K4 = {100,110};
	std::array<vec2,4> Ks = {K1,K2,K3,K4};
	std::vector<vec2> KHull = hull(Ks);

	SDL_Rect PRect;
	SDL_Rect drawnRect = {SCREEN_WIDTH/2, SCREEN_HEIGHT/2, 4 ,4};
	Bezpath Path;
	Path.pushNode({static_cast<float>(SCREEN_WIDTH)/2,static_cast<float>(SCREEN_HEIGHT/2),0,0,60});
	Path.pushNode({static_cast<float>(SCREEN_WIDTH)/2+100,static_cast<float>(SCREEN_HEIGHT/2)+70,0,100,0});

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
					case SDLK_r:
						Path.rotatePath(drawnRect.x,SCREEN_HEIGHT-drawnRect.y,.1);
						break;
				}	

			}

			if (event.type == SDL_MOUSEBUTTONDOWN)
			{
				SDL_GetMouseState(&mousex, &mousey);
				drawnRect.x=mousex-KASTENGROSSE/2;
				drawnRect.y=mousey-KASTENGROSSE/2;
			}
		}
		if( timer.get() >= (1000/FRAMERATE) )
		{
		SDL_SetRenderDrawColor(gRenderer, 0xFF, 0xFF, 0xFF, 0xFF);
		SDL_RenderClear(gRenderer);
		SDL_SetRenderDrawColor(gRenderer, 0x00, 0x00, 0x00, 0xFF);
		drawBezierPath(gRenderer,Path);
		for (int i=0;i<4;i++)
		{
			PRect = {static_cast<int>(std::round(Ps.at(i).X))-2,SCREEN_HEIGHT-static_cast<int>(std::round(Ps.at(i).Y))-2,4,4};
			SDL_RenderFillRect(gRenderer, &PRect);
		}
		for (std::vector<vec2>::iterator it=PHull.begin();it<PHull.end();it++)
		{
			SDL_RenderDrawLine(gRenderer,it->X,SCREEN_HEIGHT-it->Y,(PHull.begin()+(it-PHull.begin()+1)%PHull.size())->X,SCREEN_HEIGHT-(PHull.begin()+(it-PHull.begin()+1)%PHull.size())->Y);
		}	
		SDL_RenderDrawRect(gRenderer, &drawnRect);
		SDL_RenderPresent(gRenderer);
		timer.reset();
		}
	}

	SDL_DestroyWindow(gWindow);
	SDL_DestroyRenderer(gRenderer);
	gWindow = NULL;
	gRenderer = NULL;

	return 0;
}


