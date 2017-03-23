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

physobj::physobj(double x, double y)
{
	xpos = x;
	ypos = y;
	/*std::cout << "xvel?\n";
	std::cin >> xvel;
	std::cout << "\nyvel\n";
	std::cin >> yvel;*/
}

int physobj::reset(double x, double y)
{
	xpos = x;
	ypos = y;
	std::cout << "xvel?\n";
	std::cin >> xvel;
	std::cout << "\nyvel\n";
	std::cin >> yvel;
	std::cout << "vx,vy" << xvel << "," << yvel << std::flush;
	return 1;
}


physobj::~physobj()
{
}
//kraftpartikel Functions
int kraftpartikel::iterate(double t)
{
	sety(gety()+t*getyvel());
	setyvel(getyvel()+t*Fy/m);
	//std::cout << "x=x+v*t \n"  << getx()+t*getxvel() << " = " << getx() << " + " << getxvel() << " * " << t  << "\n";
	//std::cout << "v=v+t*F/m \n"  << getxvel()+t*Fx/m << " = " << getxvel() << " + " <<  t << " * " << Fx << " / " << m << "\n";
	setx(getx()+t*getxvel());
	setxvel(getxvel()+t*Fx/m);
	return 1;
}


void kraftpartikel::coulombkraft(kraftpartikel& von, double kraft)
{
	double x=von.getx()-getx();
	double y=von.gety()-gety();
	//std::cout << "Fx = x * a / (x**2 + y**2)**3/2 :\n" << x * kraft / pow(pow(x,2)+pow(y,2),3/2) << " = " << x << " * " << kraft << " / " << pow(pow(x,2)+pow(y,2),3/2) << "\n";
	double charge = m*von.m;
	Fx= charge * x * kraft / pow(pow(x,2)+pow(y,2),1.5);
	Fy= charge * y * kraft / pow(pow(x,2)+pow(y,2),1.5);
	von.Fx = -Fx;
	von.Fy = -Fy;
}

kraftpartikel::kraftpartikel(double x, double y, double mass) : physobj(x,y)
{
	m=mass;
	std::cout << "memberMasse= " << m << "\n" <<std::flush;
}

void kraftpartikel::elastischerstoss(kraftpartikel& obj)
{
	double xverb = obj.getx()-getx();
	double yverb = obj.gety()-gety();
	if ( (std::abs(pow(pow(xverb,2)+pow(yverb,2),0.5))) <= 1.)
	{
	double normverbindungsvx = xverb/pow(pow(xverb ,2)+pow(yverb,2),.5);
	double normverbindungsvy = yverb/pow(pow(xverb,2)+pow(yverb,2),.5);
	double v1t = getxvel() * normverbindungsvx + getyvel() * normverbindungsvy;
	double v2t = obj.getxvel() * normverbindungsvx + obj.getyvel() * normverbindungsvy;
	double v1tp = 2 * (m * v1t + obj.m * v2t) / (m + obj.m)- v1t;
	double v2tp = 2 * (m * v1t + obj.m * v2t) / (m + obj.m)- v2t;
	setxvel(normverbindungsvx*v1tp);
	setyvel(normverbindungsvy*v1tp);
	obj.setxvel(normverbindungsvx*v2tp);
	obj.setyvel(normverbindungsvy*v2tp);
	}

	/*double x= getx()-obj.getx();
	double y= gety()-obj.gety();

	if ( (std::abs(pow(pow(x,2)+pow(y,2),0.5))) <= 5.)
	{
		setxvel(getxvel()*-1);
		setyvel(getyvel()*-1);
	}*/
}

