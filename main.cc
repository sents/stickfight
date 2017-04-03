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
Bezpath createbezierpath(SDL_Renderer* gRenderer);
void drawBezierPath(SDL_Renderer* renderer, Bezpath bPath);
int KASTENGROSSE = 5;
const int SCREEN_WIDTH = 800;
const int SCREEN_HEIGHT = 600;
const int FRAMERATE = 25;

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
	world.gravFy = 10;
	createbezierpath(gRenderer);
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
		if (!timer.getstatus())
		world.iterate(1e-5);
		if( timer.get() >= (1000/FRAMERATE) )
		{
		SDL_SetRenderDrawColor(gRenderer, 0xFF, 0xFF, 0xFF, 0xFF);
		SDL_RenderClear(gRenderer);
		SDL_SetRenderDrawColor(gRenderer, 0x00, 0x00, 0x00, 0xFF);
		drawobjs(gRenderer, world.vKPartikel);
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

Bezpath createbezierpath(SDL_Renderer* renderer)
{
	bool quit = false;
	SDL_Event event;
	std::array<int,2> temppos;
	Bezpath path;
	bool pos_or_tan;
	std::vector<Beznode>::iterator tempiter;
	Beznode tempnode(temppos[0],temppos[1],0,0,0);


	while (!quit)
	{
		while (SDL_PollEvent(&event) != 0)
		{
			if(event.type == SDL_QUIT)
			{
				std::cout << "TEST!" << std::flush;
				quit = true;
			}

			if(event.type == SDL_MOUSEBUTTONDOWN)
			{
				std::cout << "nodecount: " << path.get_nodecount() << "\n";
				std::cout << "iterpos: " << std::distance(path.mNodes.begin(),tempiter) << "\n" << std::flush;
				SDL_GetMouseState(&temppos[0], &temppos[1]);
				tempnode.setCoords(static_cast<float>(temppos[0]),static_cast<float>(temppos[1]));
				
				if (path.mNodes.size() == 0)
				{
					tempnode.setCoords(static_cast<float>(temppos[0]),static_cast<float>(temppos[1]));
					path.pushNode(tempnode);
					tempiter = path.mNodes.begin();
					pos_or_tan = false;
					std::cout << "Clicked first time\n" << std::flush;
				}
				else
				{
					if (pos_or_tan)
					{
						std::cout << "TEST: ! \n" << std::flush;
						tempiter->setCoords(static_cast<float>(temppos[0]),static_cast<float>(temppos[1]));
						pos_or_tan = false;
					}
					else
					{
						tempnode.setCoords(static_cast<float>(temppos[0]),static_cast<float>(temppos[1]));
						path.pushNode( tempnode);
						tempiter = path.mNodes.end()-1;
						pos_or_tan=true;
						std::cout << "Clicked second time\n" << "\n";
					}
				}
			}

			if (event.type == SDL_KEYDOWN)
			{
				switch (event.key.keysym.sym)
				{
					case SDLK_b:
						quit = true;
						break;
					case SDLK_w:
						std::cout << " x,y: " << tempiter->getX() << " , " << tempiter->getY() << "\n" << std::flush;
						break;
					case SDLK_r:
						path.rotatePath(200,200,20);
				}

			}
		}
		

		SDL_RenderClear(renderer);
		if( path.mNodes.size() != 0 )
		{	
			SDL_GetMouseState(&temppos[0],&temppos[1]);
			if(pos_or_tan)
			{
				tempiter->setCoords(static_cast<float>(temppos[0]),static_cast<float>(temppos[1]));
			}
			else
			{
				float xrel = tempiter->getX()-temppos[0];
				float yrel = tempiter->getY()-temppos[1];
				float tangent = pow( pow( xrel ,2) + pow( yrel ,2) ,0.5);
				float angle = atanf(yrel/xrel)/M_PI*360;
				tempiter->setTangent1(tangent);
				tempiter->setTangent2(tangent);
				tempiter->setAngle(angle);
			}

			for( auto i : path.mNodes)
			{

				SDL_Rect objRect = {0,0,KASTENGROSSE,KASTENGROSSE};
				objRect.x=i.getX()-KASTENGROSSE/2;
				objRect.y=i.getY()-KASTENGROSSE/2;
				SDL_SetRenderDrawColor(renderer,0x00,0x00,0x00,0xFF);
				SDL_RenderFillRect(renderer,&objRect);
			}
		}

		drawBezierPath(renderer, path);	

		SDL_SetRenderDrawColor(renderer,0xFF,0xFF,0xFF,0xFF);
		SDL_RenderPresent(renderer);
	}

	return path;
}

void drawBezierPath(SDL_Renderer* renderer,Bezpath bPath)
{
	if (bPath.get_nodecount() > 1)
	{
		//std::cout << "nodecount: " << bPath.get_nodecount() << "\n";
		int count = 20 * (bPath.get_nodecount() -1);
		SDL_Point points[count+1];
		float d = static_cast<float>(bPath.get_nodecount()-1)/(static_cast<float>(count));
		SDL_SetRenderDrawColor(renderer,0x00,0x00,0x00,0xFF);

		std::cout << "pathcount: " << bPath.get_nodecount() - 1 << "\n" << std::flush;
		std::cout << "höchstes t: " << count * d << "\n" << std::flush;
		std::cout << "count: " << count << "\n" << std::flush;
		std::cout << "d: " << d << "\n" << std::flush;

		for(int i=0; i <= count; i++)
		{
			std::cout << "i*d " << i*d << "\n" << std::flush;
			points[i].x = static_cast<int>(bPath.curve(i*d)[0]);
			points[i].y = static_cast<int>(bPath.curve(i*d)[1]);
		}
		std::cout << "Letzter Punkt x,y: : " << points[count].x << " , " << points[count].y << "\n" << std::flush;
		std::cout << "Nodepunkt x,y: : " << (bPath.mNodes.end()-1)->getX() << " , " << (bPath.mNodes.end()-1)->getY() << "\n" << std::flush;
			SDL_RenderDrawLines(renderer,points,count);
	}
	return;
}
