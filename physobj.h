/*
 * =====================================================================================
 *
 *       Filename:  physobj.h
 *
 *    Description:  physical object
 *
 *        Version:  1.0
 *        Created:  13.03.2017 19:50:00
 *       Revision:  none
 *       Compiler:  gcc
 *
 *         Author:  Arvid Krein (mn), arvid2000@googlemail.com
 *        Company:  -
 *
 * =====================================================================================
 */

#ifndef PHYSOBJ
#define PHYSOBJ

#include <iostream>
#include <stdio.h>
#include <string>
#include <cmath>
#include <array>
#include <vector>
#include "physobj.h"

class physobj
{
	public:
		float getX() const;
		float getY() const;
		int setX(float x);
		int setY(float y);
		float getxvel() const;
		float getyvel() const;
		int setxvel(float x);
		int setyvel(float y);
		physobj(float x=0, float y=0, float vx=0, float vy=0);
		~physobj();


	private:
		float xpos;
		float ypos;
		float xvel;
		float yvel;
};



class kraftpartikel : public physobj
{
	public:
		kraftpartikel(float x = 0, float y = 0, float mass = 1, float charge = 1);
		void iterate(float t);
		float getFx() const; //get methods
		float getFy() const;
		float getMass() const;
		float getCharge() const;
		void setFx(float F); //set methods
		void setFy(float F);
		void setMass(float m);
		void setCharge(float q);

	private:
		float Fx;
		float Fy;
		float mMass=1;
		float mCharge=1;



};

class Worldframe
{
	public:
		bool flag=true;
		void iterate(float t);
		~Worldframe();

		float xsize = 0; 
		float ysize = 0; 
		float coulombfaktor = 40000;
		float gravFx = 0;
		float gravFy = 2000;

		std::vector<kraftpartikel> vKPartikel;
	private:
		void radialForce(kraftpartikel* part1, kraftpartikel* part2, float kraftfaktor, float exponent); //radialkraft part2 auf part1. Form: F^{->} = e^{^}_{r} * kraftfaktor * r^{exponent}
		void elasticBounce(kraftpartikel* part1, kraftpartikel* part2);
		void gravitationalForce(kraftpartikel* part, float Fx, float Fy); //In bestimmte Richtung gerichtete kraft.
		bool collisioncheck(physobj* part1, physobj* part2);
		bool isoutofworld(const physobj& part) const;
};

#endif
