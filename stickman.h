/*
 * =====================================================================================
 *
 *       Filename:  stickman.h
 *
 *    Description:  stickman class
 *
 *        Version:  1.0
 *        Created:  04.04.2017 14:21:37
 *       Revision:  none
 *       Compiler:  gcc
 *
 *         Author:  Arvid Krein (mn), arvid2000@googlemail.com
 *        Company:  -
 *
 * =====================================================================================
 */
#ifndef STICKMAN
#define STICKMAN
#include <vector>
#include <cmath>
#include "bezier.h"
#include "physobj.h"

class Bodypart
{
	public:
	Bodypart();
	Bodypart(Bezpath bodyPath);
	Bezpath mPath;
	std::vector<kraftpartikel> vJoints;
	void iterate(float t);
};

#endif
