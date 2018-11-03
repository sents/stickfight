/*
 * =====================================================================================
 *
 *       Filename:  utility.h
 *
 *    Description:  header for small utility functions
 *
 *        Version:  1.0
 *        Created:  23.02.2017 16:28:48
 *       Revision:  none
 *       Compiler:  gcc
 *
 *         Author:  Arvid Krein (mn), arvid2000@googlemail.com
 *        Company:  -
 *
 * =====================================================================================
 */

#ifndef INIT_H
#define INIT_H

#include <iostream>
#include <stdio.h>
#include <string>
#include <cmath>
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include "bezier.h"
#include "physobj.h"

extern const int SCREEN_WIDTH;
extern const int SCREEN_HEIGHT;

//init function
bool init(SDL_Window** pWindow, SDL_Renderer** pRenderer, int height, int width);

//bezier creation functions
Bezpath createbezierpath(SDL_Renderer* gRenderer);
void drawBezierPath(SDL_Renderer* renderer,const Bezpath &bPath);

//physobj drawing function
void drawobj(SDL_Renderer* Renderer, kraftpartikel* Obj, int boxsize);
void drawobjs(SDL_Renderer* Renderer, std::vector<kraftpartikel> vObj, int boxsize);

//simpleTimer class
class simpleTimer
{
	public:
		void reset();
		void pause();
		void unpause();
		void flip();
		bool getstatus();
		Uint32 get(); //Time in milliseconds
                simpleTimer();
                private:
		Uint32 starttime;
		Uint32 pausedtime;
		bool ispaused=false;
};



#endif 
