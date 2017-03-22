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

#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <iostream>
#include <stdio.h>
#include <string>
#include <cmath>




bool init(SDL_Window** pWindow, SDL_Renderer** pRenderer, int height, int width);

#endif 
