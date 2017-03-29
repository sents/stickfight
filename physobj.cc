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

physobj::physobj(double x, double y, double vx, double vy)
{
	xpos = x;
	ypos = y;
	xvel = vx;
	yvel = vy;
}


physobj::~physobj()
{
}

double physobj::getx()
{
	return xpos;
}

double physobj::gety()
{
	return ypos;
}

int physobj::setx(double x)
{
	xpos = x;
	return 1;
}

int physobj::sety(double y)
{
	ypos = y;
	return 1;
}

double physobj::getxvel()
{
	return xvel;
}

double physobj::getyvel()
{
	return yvel;
}

int physobj::setxvel (double vx)
{
	xvel = vx;
	return 1;
}

int physobj::setyvel(double vy)
{
	yvel = vy;
	return 1;
}

//kraftpartikel Functions

kraftpartikel::kraftpartikel(double x, double y, double mass, double charge) : physobj(x,y)
{
	mMass=mass;
	mCharge=charge;
}

double kraftpartikel::getFx()
{
	return Fx;
}

double kraftpartikel::getFy()
{
	return Fy;
}
double kraftpartikel::getMass()
{
	return mMass;
}

double kraftpartikel::getCharge()
{
	return mCharge;
}

void kraftpartikel::setFx(double F)
{
	Fx = F;
}

void kraftpartikel::setFy(double F)
{
	Fy = F;
}

void kraftpartikel::setMass(double m)
{
	mMass = m;
}

void kraftpartikel::setCharge(double q)
{
	mCharge = q;
}





void kraftpartikel::iterate(double t) //iteration of simulation
{
	sety( gety() + t * getyvel() ); //Iterate Position with velocity
	setx( getx() + t * getxvel() );
	setyvel( getyvel() + t * Fy / mMass ); //Iterate velocity with force
	setxvel( getxvel() + t * Fx / mMass );
	return;
}


//Worldframe class functions

Worldframe::~Worldframe()
{
	for (auto i : vKPartikel)
	{
		delete i;

	}
}

void Worldframe::iterate(double t)
{
	for ( std::vector<kraftpartikel*>::iterator i = vKPartikel.begin(); i < vKPartikel.end(); i++)
	{
		for (std::vector<kraftpartikel*>::iterator j = i+1; j < vKPartikel.end(); j++)
		{
		//	elasticBounce(*i,*j);
		}
		if (isoutofworld(*i))
		{
			delete *i;
			vKPartikel.erase(i);
		}
	}

	for (auto i : vKPartikel)
	{
		i->setFx(0.);
		i->setFy(0.);
		for( auto j: vKPartikel )
		{
			if ( i!=j )
			{
			radialForce(i,j,coulombfaktor,-2.);
			radialForce(i,j,-1*coulombfaktor,-5.);
			}
		}
		gravitationalForce(i,gravFx,gravFy);
		i->iterate(t);
	}
}

void Worldframe::radialForce(kraftpartikel* part1, kraftpartikel* part2, double kraftfaktor, double exponent)
{
	double x = part2->getx() - part1->getx();
	double y = part2->gety() - part1->gety();
	part1->setFx(part1->getFx() + part1->getCharge() * part2->getCharge() * x * kraftfaktor * pow( pow(x,2) + pow(y,2) , 0.5 * (exponent - 1.)));
	part1->setFy(part1->getFy() + part1->getCharge() * part2->getCharge() * y * kraftfaktor * pow( pow(x,2) + pow(y,2) , 0.5 * (exponent - 1)));
}

void Worldframe::gravitationalForce(kraftpartikel* part, double Fx, double Fy)
{
	part->setFx( part->getFx() + Fx);
	part->setFy( part->getFy() + Fy);
}

void Worldframe::elasticBounce(kraftpartikel* part1, kraftpartikel* part2)
{
	double xverb = part2->getx() - part1->getx();
	double yverb = part2->gety() - part1->gety();
	if ( (std::abs(pow(pow(xverb,2)+pow(yverb,2),0.5))) <= 1.)
	{
	double normverbindungsvx = xverb / pow( pow(xverb,2) + pow(yverb,2) ,.5);
	double normverbindungsvy = yverb / pow( pow(xverb,2) + pow(yverb,2) ,.5);
	double v1t = part1->getxvel() * normverbindungsvx + part1->getyvel() * normverbindungsvy;
	double v2t = part2->getxvel() * normverbindungsvx + part2->getyvel() * normverbindungsvy;
	double v1tp = 2 * ( part1->getMass() * v1t + part2->getMass() * v2t ) / ( part1->getMass() + part2->getMass() ) - v1t;
	double v2tp = 2 * ( part1->getMass() * v1t + part2->getMass() * v2t ) / ( part1->getMass() + part2->getMass() ) - v2t;
	part1->setxvel( normverbindungsvx * v1tp );
	part1->setyvel( normverbindungsvy * v1tp );
	part2->setxvel( normverbindungsvx * v2tp );
	part2->setyvel( normverbindungsvy * v2tp );
	}
}

bool Worldframe::isoutofworld(physobj* part)
{
	bool flag = false;
	if ( part->getx() >= xsize || part->getx() <= 0 )
		flag = true;
	if ( part->gety() >= ysize || part->gety() <= 0 )
		flag = true;
	return flag;

}

bool Worldframe::collisioncheck(physobj* part1, physobj* part2)
{
	return pow( pow( part2->getx() - part1->getx() , 2 ) + pow( part2->gety() - part1->gety() , 2) , 0.5 ) <= 1. ;
}
