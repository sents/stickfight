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

const int SCREEN_WIDTH = 1000;
const int SCREEN_HEIGHT = 750;

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
    if(!ispaused)
    {
        starttime=SDL_GetTicks();
    }
    else
    {
        pausedtime = SDL_GetTicks();
    }
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

void simpleTimer::flip()
{
	if(ispaused)
		unpause();
	else
		pause();
}

bool simpleTimer::getstatus()
{
	return ispaused;
}

Uint32 simpleTimer::get()
{
	if (ispaused)
		return (pausedtime - starttime);
	else
		return (SDL_GetTicks() - starttime);
}


// function to create bezier path
Bezpath createbezierpath(SDL_Renderer* renderer)
{
	int KASTENGROSSE = 5;
	bool quit = false;
	bool pos_or_tan; //boolean switch for setting normal and control points,
	SDL_Event event;
	std::array<int,2> temppos; //array to receive mouse state
	Beznode tempnode(temppos[0],temppos[1],0,0,0); //node to push into Bezpath
	std::vector<Beznode>::iterator tempiter; 
	Bezpath path;


	while (!quit)
	{
		while (SDL_PollEvent(&event) != 0)
		{
			if(event.type == SDL_QUIT)
			{
				quit = true;
			}

			if(event.type == SDL_MOUSEBUTTONDOWN)
			{
				
				if (path.get_nodecount() == 0)
				{
					tempnode.setCoords(static_cast<float>(temppos[0]),static_cast<float>(temppos[1]));
					path.pushNode(tempnode);
					tempiter = path.mNodes.begin();
					pos_or_tan = false;
				}
				else
				{
					if (pos_or_tan)
						pos_or_tan = false;
					else
					{
						tempnode.setCoords(static_cast<float>(temppos[0]),static_cast<float>(temppos[1]));
						tempnode.setTangent1(0);
						tempnode.setTangent2(0);
						path.pushNode( tempnode);
						tempiter = path.mNodes.end()-1;
						pos_or_tan=true;
					}
				}
			}

			if (event.type == SDL_KEYDOWN)
			{
				switch (event.key.keysym.sym)
				{
					case SDLK_q:
						quit = true;
						break;
					case SDLK_w:
						std::cout << " x,y: " << tempiter->getX() << " , " << tempiter->getY() << "\n" << std::flush;
						break;
					case SDLK_r:
						path.rotatePath(temppos[0],temppos[1],.01);
				}

			}
		}

		SDL_GetMouseState(&temppos[0], &temppos[1]);
		temppos[1] = SCREEN_HEIGHT-temppos[1];
		SDL_SetRenderDrawColor(renderer,0xFF,0xFF,0xFF,0xFF);
		SDL_RenderClear(renderer);
		if( path.get_nodecount() != 0 )
		{	
			
			if(pos_or_tan)
			{
				tempiter->setCoords(static_cast<float>(temppos[0]),static_cast<float>(temppos[1]));
			}
			else
			{
				float xrel = tempiter->getX()-temppos[0];
				float yrel = tempiter->getY()-temppos[1];
				float tangent = pow( pow( xrel ,2) + pow( yrel ,2) ,0.5);
				float angle = atan2(yrel,xrel);
				if (std::isnan(angle))
					angle=0;
				tempiter->setTangent1(tangent);
				tempiter->setTangent2(tangent);
				tempiter->setAngle(angle);
			}

			for( auto i : path.mNodes)
			{

				SDL_Rect objRect = {0,0,KASTENGROSSE,KASTENGROSSE};
				objRect.x=i.getX()-KASTENGROSSE/2;
				objRect.y= SCREEN_HEIGHT - i.getY()-KASTENGROSSE/2;
				SDL_SetRenderDrawColor(renderer,0x00,0x00,0x00,0xFF);
				SDL_RenderFillRect(renderer,&objRect);
			}
		}

		
		drawBezierPath(renderer, path);	

		SDL_RenderPresent(renderer);
	}

	return path;
}

//object drawing functions
void drawBezierPath(SDL_Renderer* renderer,const Bezpath &bPath)
{
	if (bPath.get_nodecount() > 1)
	{
		//std::cout << "nodecount: " << bPath.get_nodecount() << "\n";
		int count = 20 * (bPath.get_nodecount() -1);
		SDL_Point points[count+1];
		float d = static_cast<float>(bPath.get_nodecount()-1)/(static_cast<float>(count));
		SDL_SetRenderDrawColor(renderer,0x00,0x00,0x00,0xFF);
		for(int i=0; i <= count; i++)
		{
			points[i].x = std::round(bPath.curve(i*d).X);
			points[i].y = SCREEN_HEIGHT - std::round(bPath.curve(i*d).Y);
		}
				SDL_RenderDrawLines(renderer,points,count+1);
	}
	return;
}

void drawobj(SDL_Renderer* Renderer, kraftpartikel* Obj, int boxsize)
{
    SDL_Rect objRect = {0,0,boxsize,boxsize};
    objRect.x= Obj->getX()-boxsize/2;
    objRect.y= SCREEN_HEIGHT - Obj->getY()-boxsize/2;
    std::array<Uint8,4> color = Obj->color;
    std::cout << unsigned(color.at(0)) << "\n" << std::flush;// << Obj->color[1] << Obj->color[2] << Obj->color[3] << "\n" << std::flush;
    SDL_SetRenderDrawColor(Renderer, color.at(0), color.at(1), color.at(2), color.at(3));
    SDL_RenderFillRect(Renderer,&objRect);
}

void drawobjs(SDL_Renderer* Renderer, std::vector<kraftpartikel> vObj, int boxsize)
{
	for (auto it = begin(vObj); it != end(vObj); ++it)
	{
            drawobj(Renderer, &(*it), boxsize);
        }
}

