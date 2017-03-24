/*
 * =====================================================================================
 *
 *       Filename:  utility.cpp
 *
 *    Description:  small utility functions
 *
 *        Version:  1.0
 *        Created:  23.02.2017 16:32:35
 *       Revision:  none
 *       Compiler:  gcc
 *
 *         Author:  Arvid Krein (mn), arvid2000@googlemail.com
 *        Company:  -
 *
 * =====================================================================================
 */

#include "utility.h"
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <iostream>
#include <stdio.h>
#include <string>
#include <cmath>

//init function
bool init (SDL_Window** pWindow, SDL_Renderer** pRenderer, int height, int width)
{
	bool success = true;

	if (SDL_Init(SDL_INIT_VIDEO) < 0)
	{
		printf("Could not initialze SDL! SDL Error: %n\n", SDL_GetError() );
	}
	else
	{
			//Set texture filtering to linear
			if( !SDL_SetHint( SDL_HINT_RENDER_SCALE_QUALITY, "1" ) )
			{
				printf( "Warning: Linear texture filtering not enabled!" );
			}

		//CreateWindow
		*pWindow = SDL_CreateWindow("Testfenster", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, width, height,SDL_WINDOW_SHOWN);
		if (*pWindow == NULL)
		{
			printf( "Window could not be created! SDL Error: %s\n", SDL_GetError() );
			success = false;
		}

		*pRenderer = SDL_CreateRenderer ( *pWindow, -1 , SDL_RENDERER_ACCELERATED);
		if (*pRenderer == NULL)
		{
			printf( "Renderer could not be created! SDL Error: %s\n", SDL_GetError() );
			success = false;
		}
		else
		{
			SDL_SetRenderDrawColor( *pRenderer, 0xFF, 0xFF, 0xFF, 0xFF);

			int imgFlags = IMG_INIT_PNG;
			if( !( IMG_Init( imgFlags ) & imgFlags ) ) 
			{
				printf( "SDL_image could not initialize! SDL_image Error: %s\n", IMG_GetError() );
				success = false;
			}
		}
	}
	std::cout << "Intialisation successful!\n";	
	return success;
}

//simpleTimer class functions
simpleTimer::simpleTimer()
{
	reset();
}

void simpleTimer::reset()
{
		starttime=SDL_GetTicks();
}

void simpleTimer::pause()
{
	if(!ispaused)
	{
	ispaused = true;
	pausedtime = SDL_GetTicks();
	}
}

void simpleTimer::unpause()
{
	if(ispaused)
	{
	starttime = starttime + (SDL_GetTicks() - pausedtime);
	ispaused = false;
	}
}

Uint32 simpleTimer::get()
{
	if (ispaused)
		return (pausedtime - starttime);
	else
		return (SDL_GetTicks() - starttime);
}

