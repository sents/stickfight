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

int main (/*int argc, char* args[])*/)
{
	const int SCREEN_WIDTH = 200;
	const int SCREEN_HEIGHT = 200;
	SDL_Window* gWindow;
	SDL_Renderer* gRenderer;
	init(&gWindow,&gRenderer, SCREEN_HEIGHT, SCREEN_WIDTH);
	bool quit=false;
	SDL_Event event;
	while (quit!=true)
	{
		while(SDL_PollEvent(&event)!=0)
		{
			if(event.type == SDL_QUIT)
				quit=true;
		}
		SDL_SetRenderDrawColor(gRenderer, 0xFF, 0xFF, 0xFF, 0xFF);
		SDL_RenderClear(gRenderer);
		SDL_RenderPresent(gRenderer);
	}
	return 0;
}
