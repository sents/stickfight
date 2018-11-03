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
 *         Author:  Finn Krein, finnkrein@googlemail.com; Arvid Krein, arvid2000@googlemail.com Company:  -
 *
 * =====================================================================================
 */

#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <iostream>
#include <cstdio>
#include <string>
#include <cmath>
#include <vector>
#include "utility.h"
#include "lTexture.h"
#include "physobj.h"
#include "bezier.h"
#include "stickman.h"

int KASTENGROSSE = 5;
const int FRAMERATE = 25;

int main ()
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
	Worldframe world(40000, 0, 0);
	world.size.X = SCREEN_WIDTH;
	world.size.Y = SCREEN_HEIGHT;
	world.gravF.Y = -0;
        std::cout << "xsize: " << world.size.X << "\n" << std::flush;
        std::cout << "ysize: " << world.size.Y << "\n" << std::flush;
	std::vector<Bezpath> vBpath;
	kraftpartikel tempPart;
        std::vector<kraftpartikel>::iterator vKpointer = world.vKPartikel.begin();
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
					case SDLK_b:
						vBpath.push_back(createbezierpath(gRenderer));
						break;
					case SDLK_p:
						timer.flip();
						break;
					case SDLK_o:
						timer.reset();
						break;
					case SDLK_i:
						std::cout << timer.get() << "\n" << std::flush;
						break;
					case SDLK_w:
						std::cout << (world.vKPartikel.end()-1)->getX() << " , " << (world.vKPartikel.end()-1)->getY() << "\n" << std::flush;
						break;
					case SDLK_n:
						tempPart.setX(static_cast<float>(mousex));
						tempPart.setY(static_cast<float>(SCREEN_HEIGHT - mousey));
						world.vKPartikel.push_back(tempPart);
                                                vKpointer = world.vKPartikel.begin();
						break;
                                        case SDLK_RIGHT:
                                                vKpointer->setxvel( vKpointer->getxvel() + 5);
                                                break;
                                        case SDLK_LEFT:
                                                vKpointer->setxvel( vKpointer->getxvel() - 5);
                                                break;
                                       case SDLK_UP:
                                                vKpointer->setyvel( vKpointer->getyvel() + 5);
                                                break;
                                        case SDLK_DOWN:
                                                vKpointer->setyvel( vKpointer->getyvel() - 5);
                                                break;
                                        case SDLK_PLUS:
                                                //vKpointer->color = {0xFF,0x00,0x00,0xFF};
                                                vKpointer+=1;
                                                //vKpointer->color = {0xFF,0x00,0x00,0xFF}; 
                                                break;
                                        case SDLK_MINUS:
                                                //vKpointer->color = {0xFF,0x00,0x00,0xFF};
                                                vKpointer-=1;
                                                //vKpointer->color = {0xFF,0x00,0x00,0xFF}; 
                                                break;
                                        case SDLK_e:
                                                std::cout << world.getEnergy() << "\n" << std::flush;
                                                break;
                                        case SDLK_t:
                                                std::cout << pow( vKpointer->getyvel(), 2 ) + pow( vKpointer->getxvel(), 2 ) << "\n" << std::flush;
                                                break;
                                        case SDLK_v:
                                                std::cout << vKpointer->vel.X << " , "  << vKpointer->vel.Y << "\n" << std::flush;
                                                break;
                                        case SDLK_1:
                                                world.coulombfaktor -= 1000;
                                        break;
                                        case SDLK_2:
                                                world.coulombfaktor += 1000;
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
		if (!timer.getstatus())
                {
                    //std::cout << timer.get() << "\n";
                    world.iterate(1e-5);
                }
		if( timer.get() >= (1000/FRAMERATE) )
		{
		SDL_SetRenderDrawColor(gRenderer, 0xFF, 0xFF, 0xFF, 0xFF);
		SDL_RenderClear(gRenderer);
		SDL_SetRenderDrawColor(gRenderer, 0x00, 0x00, 0x00, 0xFF);
		drawobjs(gRenderer, world.vKPartikel, KASTENGROSSE);
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


