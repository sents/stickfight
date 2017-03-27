#include "bezier.h"

Beznode::Beznode(float X,float Y,float Angle,float Tangent1,float Tangent2) : mX(X),mY(Y),mAngle(Angle),mTangent1(Tangent1),mTangent2(Tangent2)
{
}


float Beznode::getAngle()
{
	return mAngle;
}


float Beznode::getX()
{
	return mX;
}

float Beznode::getY()
{
	return mY;
}

float Beznode::getTangent1()
{
	return mTangent1;
}

float Beznode::getTangent2()
{
	return mTangent2;
}

std::array<float,2> Beznode::getCoords()
{
	std::array<float,2> Coords = {{mX,mY}};
	return Coords;
}


std::array<float,2> Beznode::getTangent()
{
	std::array<float,2> Coords = {{mTangent1,mTangent2}};
	return Coords;
}

std::array<float,2> Beznode::getT1Coords()
{
	float PX = std::sin(mAngle/360*2*M_PI)*mTangent1+mX;
	float PY = std::cos(mAngle/360*2*M_PI)*mTangent1+mY;
	std::array<float,2> Coords = {{PX,PY}};
	return Coords;
}

std::array<float,2> Beznode::getT2Coords()
{
	float PX = std::sin(mAngle/360*2*M_PI)*mTangent2*(-1)+mX;
	float PY = std::cos(mAngle/360*2*M_PI)*mTangent2*(-1)+mY;
	std::array<float,2> Coords = {{PX,PY}};
	return Coords;
}

void Beznode::setAngle(float Angle)
{
	mAngle=Angle;
}

void Beznode::setCoords(float X,float Y)
{
	mX=X;
	mY=Y;
}

void Beznode::setCoords(std::array<float,2> Coords)
{
	mX=Coords.at(0);
	mY=Coords.at(1);
}

void Beznode::setTangent1(float Tangent1)
{
	mTangent1=Tangent1;
}

void Beznode::setTangent2(float Tangent2)
{
	mTangent1=Tangent2;
}
void Beznode::rotate(float Angle)
{
	mAngle = Angle;
}
void Beznode::translate(float X,float Y)
{
	mX += X;
	mY += Y;
}

unsigned int Bezpath::get_nodecount()
{
	return static_cast<unsigned int>(mNodes.size());
}

void Bezpath::pushNode(Beznode *Node)
{
	mNodes.push_back(*Node);
}

void Bezpath::popNode()
{
	mNodes.pop_back();
}

void Bezpath::translatePath(float X, float Y)
{
	for(std::vector<Beznode>::iterator i = mNodes.begin();i <= mNodes.end();i++)
	{
		i->translate(X,Y);
	}
}

void Bezpath::rotatePath(float X, float Y, float Angle)
{

	float R;
	for(auto i : mNodes)
	{
		R=std::sqrt(pow(i.getX()-X,2)+pow(i.getY()-Y,2));
		i.translate(R*std::cos(Angle)-(i.getX()-X),R*std::sin(Angle)-(i.getY()-Y));
		i.rotate(Angle);
	}
}


Bezpath::Bezpath(std::vector<Beznode> *Nodes)
{
	mNodes=*Nodes;
}
std::array<float,2> Bezpath::curve(float t)
{
	float X;
	float Y;
	int n=static_cast<int>(t);
	if (t<0)
	{t=0;}
	else if (t>n)
	{t=n;}
	X=(1-t)*(mNodes.at(n).getX()*(1-t)+mNodes.at(n).getT2Coords().at(0))+t*((1-t)*mNodes.at(n+1).getT1Coords().at(0)+t*mNodes.at(n+1).getX());
	Y=(1-t)*(mNodes.at(n).getY()*(1-t)+mNodes.at(n).getT2Coords().at(1))+t*((1-t)*mNodes.at(n+1).getT1Coords().at(1)+t*mNodes.at(n+1).getY());
	std::array<float,2> vec = {{X,Y}};
	return vec;
}

void Bezpath::insertNode(Beznode *Node,unsigned int pos)
{
	std::vector<Beznode>::iterator it = mNodes.begin();
	mNodes.insert(it+pos,*Node);
}

void Bezpath::deleteNode(unsigned int pos)
{
	std::vector<Beznode>::iterator it = mNodes.begin();
	mNodes.erase(it+pos);
}
