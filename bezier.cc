#include "bezier.h"

//Beznode class functions
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
	Angle -= (static_cast<int>(Angle)/360)*360;
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
	if (std::abs(mAngle+Angle)>=360)
	{
		mAngle = (mAngle+Angle)-(static_cast<int>(mAngle+Angle)/360)*360;
	}
	else
	{
		mAngle += Angle;
	}
}

void Beznode::translate(float X,float Y)
{
	mX += X;
	mY += Y;
}

//Bezpath class functions
Bezpath::Bezpath(std::vector<Beznode> *Nodes)
{
	mNodes=*Nodes;
}

Bezpath::Bezpath()
{}

unsigned int Bezpath::get_nodecount()
{
	return static_cast<unsigned int>(mNodes.size());
}

void Bezpath::pushNode(const Beznode &Node)
{
	mNodes.push_back(Node);
}

void Bezpath::popNode()
{
	mNodes.pop_back();
}

void Bezpath::translatePath(float X, float Y)
{
	for(std::vector<Beznode>::iterator i = mNodes.begin();i < mNodes.end();i++)
	{
		i->translate(X,Y);
	}
}
void Bezpath::translatePath(std::array<float,2> Vec)
{
	translatePath(Vec.at(0),Vec.at(1));
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

void Bezpath::rotatePath(std::array<float,2> Vec, float Angle)
{
	rotatePath(Vec.at(0),Vec.at(1),Angle);
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
	float P0X = mNodes.at(n).getX();
	float P1X = mNodes.at(n).getT2Coords().at(0);
	float P2X = mNodes.at(n+1).getT1Coords().at(0);
	float P3X = mNodes.at(n+1).getX();
	float P0Y = mNodes.at(n).getY();
	float P1Y = mNodes.at(n).getT2Coords().at(1);
	float P2Y = mNodes.at(n+1).getT1Coords().at(1);
	float P3Y = mNodes.at(n+1).getY();
	X=(1-t)*((1-t)*((1-t)*P0X+t*P1X)+t*((1-t)*P1X+t*P2X))+t*((1-t)*((1-t)*P1X+t*P2X)+t*((1-t)*P2X+t*P3X));
	Y=(1-t)*((1-t)*((1-t)*P0Y+t*P1Y)+t*((1-t)*P1Y+t*P2Y))+t*((1-t)*((1-t)*P1Y+t*P2Y)+t*((1-t)*P2Y+t*P3Y));
	std::array<float,2> vec = {{X,Y}};
	return vec;
}

void Bezpath::insertNode(const Beznode &Node,unsigned int pos)
{
	std::vector<Beznode>::iterator it = mNodes.begin();
	mNodes.insert(it+pos,Node);
}

void Bezpath::deleteNode(unsigned int pos)
{
	std::vector<Beznode>::iterator it = mNodes.begin();
	mNodes.erase(it+pos);
}


