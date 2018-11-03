#include "vec2.h"
#include <iostream>

vec2::vec2(): X(0),Y(0)
{
}
vec2::vec2(std::array<float,2> vec): X(vec.at(0)),Y(vec.at(0))
{
}
vec2::vec2(float X,float Y): X(X),Y(Y)
{
}
float vec2::abs() const
{
	return std::sqrt(pow(X,2)+pow(Y,2));
}


float vec2::angle() const
{
	return std::atan2(this->Y,this->X);
}

std::array<float,2> vec2::toarr() const
{
	return {X,Y};
}

vec2 &  vec2::operator+=(const vec2 &b)
{
	this->X += b.X;
	this->Y += b.Y;
	return *this;
}
vec2 &  vec2::operator-=(const vec2 &b)
{
	this->X -= b.X;
	this->Y -= b.Y;
	return *this;
}

vec2 & vec2::operator*=(float f)
{
	this->X *= f;
	this->Y *= f;
	return *this;
}

float vec2::distTo( const vec2 &b) const
{
	return std::sqrt(pow(this->X-b.X,2)+pow(this->Y-b.Y,2));
}

float vec2::angTo( const vec2 &b) const
{
	return std::atan2(b.Y-this->Y,b.X-this->X);
}

float vec2::xTo( const vec2 &b) const
{
	return this->X*b.Y-this->Y*b.X;
}


void vec2::rotate(float angle)
{
	float angle0 = this->angle();
	float abs0 = this-> abs();
	X = std::cos(angle0+angle)*abs0;
	Y = std::sin(angle0+angle)*abs0;
}


vec2 vec2::project(vec2 b) const
{
	if (b.abs() == 0)
	{
		return {0,0};
	}
	b *= 1/b.abs();
	return (*this*b)*b;
}


vec2 operator+(vec2 lhs,const vec2 & rhs)
{
	return lhs += rhs;
}

vec2 operator-(vec2 lhs,const vec2 & rhs)
{
	return lhs -= rhs;
}


float operator*(const vec2 & lhs,const vec2 & rhs)
{
	return lhs.X*rhs.X+lhs.Y*rhs.Y;
}

vec2 operator*(float lhs, vec2 rhs)
{
	rhs *= lhs;
	return rhs;
}

vec2 operator*(vec2 lhs, float rhs)
{
return rhs * lhs;
}


vec2 operator*(const mat2 & M,const vec2 & x)
{
	return {M.a*x.X+M.b*x.Y,M.c*x.X+M.d*x.Y};
}

mat2::mat2(float a,float b, float c, float d):a(a),b(b),c(c),d(d)
{
}
mat2::mat2(const vec2 & A,const vec2 & B): a(A.X),b(B.X),c(A.Y),d(A.Y)
{
}

float mat2::det() const
{
	return a*d-c*b;
}

mat2 mat2::inv() const 
{
	float det0 = this->det();
	return {this->d/det0,-this->b/det0,-this->c/det0,this->a/det0};
}

mat2 mat2::T() const
{
	return {this->a,this->c,this->b,this->d};
}

std::array<float,3> vec2sec(vec2 A,vec2 B,vec2 C,vec2 D)
{
	mat2 M = {A-B,D-C};
	std::cout << "Vecsec: A: x=" << A.X << " y=" << A.Y << "B: x=" << B.X << " y=" << B.Y << "C: x=" << C.X << " y=" << C.Y << "D: x=" << D.X << " y=" << D.Y << std::endl; 
 

	std::cout << "a: " << M.a << " b: " << M.b << " c: " << M.c << " d: " << M.d << std::endl;
	float det0 = M.det();
	if (M.det()==0)
	{
		return {0,0,0};
	}
	vec2 x = M.inv()*(A-C);
	return {det0,x.X,x.Y};
}

vec2 eR(float angle)
{
	return {std::cos(angle),std::sin(angle)};

}
