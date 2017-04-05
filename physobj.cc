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

//physobj Functions

physobj::physobj(float x, float y, float vx, float vy)
{
	xpos = x;
	ypos = y;
	xvel = vx;
	yvel = vy;
}


physobj::~physobj()
{
}

float physobj::getX() const
{
	return xpos;
}

float physobj::getY() const
{
	return ypos;
}

int physobj::setX(float x)
{
	xpos = x;
	return 1;
}

int physobj::setY(float y)
{
	ypos = y;
	return 1;
}

float physobj::getxvel() const
{
	return xvel;
}

float physobj::getyvel() const
{
	return yvel;
}

int physobj::setxvel (float vx)
{
	xvel = vx;
	return 1;
}

int physobj::setyvel(float vy)
{
	yvel = vy;
	return 1;
}

//kraftpartikel Functions

kraftpartikel::kraftpartikel(float x, float y, float mass, float charge) : physobj(x,y)
{
	mMass=mass;
	mCharge=charge;
}

float kraftpartikel::getFx() const
{
	return Fx;
}

float kraftpartikel::getFy() const
{
	return Fy;
}
float kraftpartikel::getMass() const
{
	return mMass;
}

float kraftpartikel::getCharge() const
{
	return mCharge;
}

void kraftpartikel::setFx(float F)
{
	Fx = F;
}

void kraftpartikel::setFy(float F)
{
	Fy = F;
}

void kraftpartikel::setMass(float m)
{
	mMass = m;
}

void kraftpartikel::setCharge(float q)
{
	mCharge = q;
}





void kraftpartikel::iterate(float t) //iteration of simulation
{
	setY( getY() + t * getyvel() ); //Iterate Position with velocity
	setX( getX() + t * getxvel() );
	setyvel( getyvel() + t * Fy / mMass ); //Iterate velocity with force
	setxvel( getxvel() + t * Fx / mMass );
	return;
}


//Worldframe class functions

Worldframe::~Worldframe()
{
}

void Worldframe::iterate(float t)
{
	for ( std::vector<kraftpartikel>::iterator i = vKPartikel.begin(); i < vKPartikel.end(); i++)
	{
		for (std::vector<kraftpartikel>::iterator j = i+1; j < vKPartikel.end(); j++)
		{
			elasticBounce(&*i,&*j);
		}
		if (isoutofworld(*i))
		{
			vKPartikel.erase(i);
		}
	}

	for ( std::vector<kraftpartikel>::iterator i = vKPartikel.begin(); i < vKPartikel.end(); i++)
	{
		i->setFx(0.);
		i->setFy(0.);
		for ( std::vector<kraftpartikel>::iterator j = vKPartikel.begin(); j < vKPartikel.end(); j++)
		{
			if ( i!=j )
			{
			radialForce(&*i,&*j,coulombfaktor,-2.);
			//radialForce(&*i,&*j,-9*coulombfaktor,-5.);
			}
		}
		gravitationalForce(&*i,gravFx,gravFy);
		i->iterate(t);
	}
}

void Worldframe::radialForce(kraftpartikel* part1, kraftpartikel* part2, float kraftfaktor, float exponent)
{
	float x = part2->getX() - part1->getX();
	float y = part2->getY() - part1->getY();
	part1->setFx(part1->getFx() + part1->getCharge() * part2->getCharge() * x * kraftfaktor * pow( pow(x,2) + pow(y,2) , 0.5 * (exponent - 1.)));
	part1->setFy(part1->getFy() + part1->getCharge() * part2->getCharge() * y * kraftfaktor * pow( pow(x,2) + pow(y,2) , 0.5 * (exponent - 1)));
}

void Worldframe::gravitationalForce(kraftpartikel* part, float Fx, float Fy)
{
	part->setFx( part->getFx() + Fx);
	part->setFy( part->getFy() + Fy);
}

void Worldframe::elasticBounce(kraftpartikel* part1, kraftpartikel* part2)
{
	std::cout << part1->getxvel() +part2->getxvel() << " , " << part1->getyvel() + part2->getyvel() << "\n" << std::flush; 
	float faktor = 5;
	float xverb = part2->getX() - part1->getX();
	float yverb = part2->getY() - part1->getY();
	float r = pow(pow(xverb,2)+pow(yverb,2),0.5);
	std::array<float,2> v1temp;
	std::array<float,2> v2temp;
	if ( r <= faktor && flag)
	{
	v1temp[0] = part1->getxvel() * ( 1 - part2->getMass() / ( part1->getMass() + part2->getMass() ) ) + part2->getxvel() * 2 * part2->getMass() / ( part1->getMass() +  part2->getMass() ) ;
	v1temp[1] = part1->getyvel() * ( 1 - part2->getMass() / ( part1->getMass() + part2->getMass() ) ) + part2->getyvel() * 2 * part2->getMass() / ( part1->getMass() +  part2->getMass() ) ;
	v2temp[0] = part2->getxvel() * ( 1 - part1->getMass() / ( part1->getMass() + part2->getMass() ) ) + part1->getxvel() * 2 * part1->getMass() / ( part1->getMass() +  part2->getMass() ) ;
	v2temp[1] = part2->getyvel() * ( 1 - part1->getMass() / ( part1->getMass() + part2->getMass() ) ) + part1->getyvel() * 2 * part1->getMass() / ( part1->getMass() +  part2->getMass() ) ;
	std::cout << v1temp[0] + v2temp[0] << " , " << v1temp[1] + v2temp[1] << "\n" << std::flush; 
	part1->setxvel(v1temp[0]);
	part1->setyvel(v1temp[1]);
	part2->setxvel(v2temp[0]);
	part2->setyvel(v2temp[1]);
	r/=faktor;
	/*part1->setX(part1->getX() - xverb/r);
	part1->setX(part1->getX() - xverb/r);
	part1->setX(part1->getX() - xverb/r);
	part1->setX(part1->getX() - xverb/r);*/
	flag=false;
	}
}

bool Worldframe::isoutofworld(const physobj& part) const
{
	bool flag = false;
	if ( part.getX() >= xsize || part.getX() <= 0 )
		flag = true;
	if ( part.getY() >= ysize || part.getY() <= 0 )
		flag = true;
	return flag;

}

bool Worldframe::collisioncheck(physobj* part1, physobj* part2)
{
	return pow( pow( part2->getX() - part1->getX() , 2 ) + pow( part2->getY() - part1->getY() , 2) , 0.5 ) <= 1. ;
}
