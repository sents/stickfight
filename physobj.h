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
		int reset(double x, double y);
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
		void elastischerstoss(kraftpartikel& obj);
		void iterate(double t);
		void coulombkraft(kraftpartikel& obj, double kraft);
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
		void radialForce(kraftpartikel* part1, kraftpartikel* part2, double kraftfaktor, double exponent);
		void elasticBounce(kraftpartikel* part1, kraftpartikel* part2);
		void gravitationalForce(kraftpartikel* part, double Fx, double Fy);
		bool collisioncheck();
		bool isoutofworld(physobj* part);

		~Worldframe();

		double xsize = 0; 
		double ysize = 0; 
		double coulombfaktor = 1;
		double gravFx = 0;
		double gravFy = 1;

		std::vector<kraftpartikel*> vKPartikel;
	private:

};

#endif
