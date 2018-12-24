/*
 * =====================================================================================
 *
 *       Filename:  physobj.cc
 *
 *    Description:  physical object class
 *
 *        Version:  1.0
 *        Created:  14.03.2017 11:27:04
 *       Revision:  none
 *       Compiler:  gcc
 *
 *         Author:  Arvid Krein (mn), arvid2000@googlemail.com
 *        Company:  -
 *
 * =====================================================================================
 */

#include "physobj.h"

inline float realmod (float x, float y)
{
  float result = fmod(x, y);
  return result >= 0 ? result : result + y;
}

//physobj Functions

physobj::physobj(float x, float y, float vx, float vy)
{
	pos = {x, y};
	vel = {vx, vy};
}


physobj::~physobj()
{
}

float physobj::getX() const
{
	return pos.X;
}

float physobj::getY() const
{
	return pos.Y;
}

int physobj::setX(float x)
{
	pos.X = x;
	return 1;
}

int physobj::setY(float y)
{
	pos.Y = y;
	return 1;
}

float physobj::getxvel() const
{
	return vel.X;
}

float physobj::getyvel() const
{
	return vel.Y;
}

int physobj::setxvel (float vx)
{
        vel.X = vx;
	return 1;
}

int physobj::setyvel(float vy)
{
	vel.Y = vy;
	return 1;
}

//kraftpartikel Functions

kraftpartikel::kraftpartikel(float x, float y, float mass, float charge) : physobj(x,y)
{
	mMass=mass;
	mCharge=charge;
}


float kraftpartikel::getMass() const
{
	return mMass;
}

float kraftpartikel::getCharge() const
{
	return mCharge;
}


void kraftpartikel::setMass(float m)
{
	mMass = m;
}

void kraftpartikel::setCharge(float q)
{
	mCharge = q;
}


//Worldframe class functions
Worldframe::Worldframe()
{
}

Worldframe::Worldframe(float tcoulombfaktor, float tgravFx, float tgravFy)
{
    coulombfaktor = tcoulombfaktor;
    gravF.Y = tgravFy;
    gravF.X = tgravFx;
}

Worldframe::~Worldframe()
{
}


void Worldframe::elasticBounce(kraftpartikel* part1, kraftpartikel* part2)
{
        vec2 r1 = part1->pos;
        vec2 r2 = part2->pos;
        vec2 v1 = part1->vel;
        vec2 v2 = part2->vel;
        float m1 = part1->getMass();
        float m2 = part2->getMass();

	float faktor = 5;
        vec2 rrel = r2 - r1;
        vec2 vrel = v2 - v1;
	float r = rrel.abs();
        float vrelr = rrel * vrel;

	if ( (r <= faktor) && (vrelr < 0) )
	{

        part1->vel = (1 / (m1 + m2)) * (m1 * v1 + m2 * v2 - m2 * (v1 - v2)) ;
        part2->vel = (1 / (m1 + m2)) * (m1 * v1 + m2 * v2 - m1 * (v2 - v1));

        }
}

bool Worldframe::isoutofworld(const physobj& part) const
{
	bool flag = false;
	if ( part.getX() >= size.X || part.getX() <= 0 )
		flag = true;
	if ( part.getY() >= size.Y || part.getY() <= 0 )
		flag = true;
	return flag;

}

bool Worldframe::collisioncheck(physobj* part1, physobj* part2)
{
	return pow( pow( part2->getX() - part1->getX() , 2 ) + pow( part2->getY() - part1->getY() , 2) , 0.5 ) <= 1. ;
}

void Worldframe::periodicboundary(physobj* part)
{
    part->setX( realmod( ( part->getX() ), size.X  ) );
    part->setY( realmod( ( part->getY() ), size.Y ) );
}

float Worldframe::getEnergy()
{
    float T = 0;
    for ( auto i = vKPartikel.begin(); i < vKPartikel.end(); i++)
    {
        T += pow( i->getxvel(), 2 ) + pow( i->getyvel(), 2);
    }
    return T;
}

void Worldframe::wallbounce(physobj* part)
{

    if (part->pos.X > size.X || part->pos.X < 0)
        part->vel.X *= -1;

    if (part->pos.Y > size.Y || part->pos.Y < 0)
        part->vel.Y *= -1;

    return;
}

Worldframe::rk_integrate()
{

}
