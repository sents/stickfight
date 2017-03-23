#include "bezier.h"

beznode::beznode(float X,float Y,float Angle,float Tangent1,float Tangent2) : mX(X),mY(Y),mAngle(Angle),mTangent1(Tangent1),mTangent2(Tangent2)
{
}

float beznode::getAngle()
{
	return mAngle;
}


float beznode::getX()
{
	return mX;
}

float beznode::getY()
{
	return mY;
}

float beznode::getTangent1()
{
	return mTangent1;
}

float beznode::getTangent2()
{
	return mTangent2;
}

std::array<float,2> beznode::getCoords()
{
	std::array<float,2> Coords = {{mX,mY}};
	return Coords;
}


std::array<float,2> beznode::getTangent()
{
	std::array<float,2> Coords = {{mTangent1,mTangent2}};
	return Coords;
}

std::array<float,2> beznode::getT1Coords()
{
	float PX = std::sin(mAngle/360*2*M_PI)*mTangent1+mX;
	float PY = std::cos(mAngle/360*2*M_PI)*mTangent1+mY;
	std::array<float,2> Coords = {{PX,PY}};
	return Coords;
}

std::array<float,2> beznode::getT2Coords()
{
	float PX = std::sin(mAngle/360*2*M_PI)*mTangent2*(-1)+mX;
	float PY = std::cos(mAngle/360*2*M_PI)*mTangent2*(-1)+mY;
	std::array<float,2> Coords = {{PX,PY}};
	return Coords;
}

void beznode::setAngle(float Angle)
{
	mAngle=Angle;
}

void beznode::setCoords(float X,float Y)
{
	mX=X;
	mY=Y;
}

void beznode::setCoords(std::array<float,2> Coords)
{
	mX=Coords.at(0);
	mY=Coords.at(1);
}

void beznode::setTangent1(float Tangent1)
{
	mTangent1=Tangent1;
}

void beznode::setTangent2(float Tangent2)
{
	mTangent1=Tangent2;
}
