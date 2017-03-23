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
		physobj(double x, double y);
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
		kraftpartikel(double x, double y, double mass);
		void elastischerstoss(kraftpartikel& obj);

		double Fx;
		double Fy;
		int iterate(double t);
		void coulombkraft(kraftpartikel& obj, double kraft);
		double m=1;
};


#endif
