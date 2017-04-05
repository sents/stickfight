/*
 * =====================================================================================
 *
 *       Filename:  stickman.cc
 *
 *    Description:  a stickman class
 *
 *        Version:  1.0
 *        Created:  04.04.2017 14:21:19
 *       Revision:  none
 *       Compiler:  gcc
 *
 *         Author:  Arvid Krein (mn), arvid2000@googlemail.com
 *        Company:  -
 *
 * =====================================================================================
 */

#include "stickman.h"

//Bodypart class functions
Bodypart::Bodypart()
{
	kraftpartikel tempPart;
	for(std::vector<Beznode>::iterator i = mPath.mNodes.begin(); i < mPath.mNodes.end(); ++i)
	{
		tempPart.setX(i->getX());
		tempPart.setY(i->getY());
		vJoints.push_back(tempPart);
	}
}

Bodypart::Bodypart(Bezpath bodyPath)
{
	mPath = bodyPath;
	kraftpartikel tempPart;
	for(std::vector<Beznode>::iterator i = mPath.mNodes.begin(); i < mPath.mNodes.end(); ++i)
	{
		tempPart.setX(i->getX());
		tempPart.setY(i->getY());
		vJoints.push_back(tempPart);
	}

}

void Bodypart::iterate(float t)
{

}
