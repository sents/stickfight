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
#include <vector>
#include "physobj.h"

class physobj
{
	public:
		double getx();
		double gety();
		int setx(double x);
		int sety(double y);
		double getxvel();
		double getyvel();
		int setxvel(double x);
		int setyvel(double y);
		physobj(double x, double y, double vx=0, double vy=0);
		~physobj();


	private:
		double xpos;
		double ypos;
		double xvel=0.;
		double yvel=0.;
};



class kraftpartikel : public physobj
{
	public:
		kraftpartikel(double x, double y, double mass = 1, double charge = 1);
		void iterate(double t);
		double getFx(); //get methods
		double getFy();
		double getMass();
		double getCharge();
		void setFx(double F); //set methods
		void setFy(double F);
		void setMass(double m);
		void setCharge(double q);

	private:
		double Fx;
		double Fy;
		double mMass=1;
		double mCharge=1;



};

class Worldframe
{
	public:
		void iterate(double t);
		~Worldframe();

		double xsize = 0; 
		double ysize = 0; 
		double coulombfaktor = 40000;
		double gravFx = 0;
		double gravFy = 2000;

		std::vector<kraftpartikel*> vKPartikel;
	private:
		void radialForce(kraftpartikel* part1, kraftpartikel* part2, double kraftfaktor, double exponent); //radialkraft part2 auf part1. Form: F^{->} = e^{^}_{r} * kraftfaktor * r^{exponent}
		void elasticBounce(kraftpartikel* part1, kraftpartikel* part2);
		void gravitationalForce(kraftpartikel* part, double Fx, double Fy); //In bestimmte Richtung gerichtete kraft.
		bool collisioncheck(physobj* part1, physobj* part2);
		bool isoutofworld(physobj* part);
};

#endif
