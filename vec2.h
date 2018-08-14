#ifndef VEC2
#define VEC2

#include <cmath>
#include <array>
class vec2
{
	public:
	float X;
	float Y;
	vec2();
	vec2(std::array<float,2> vec);
	vec2(float X,float Y);
	std::array<float,2> toarr() const;
	float abs() const;
	float angle() const;
	float distTo(const vec2 &b) const;
	float angTo(const vec2 &b) const;
	float xTo(const vec2 &b) const;
	vec2 project(vec2 b) const;
	void rotate(float Angle);
	vec2 & operator+=(const vec2 & b);
	vec2 & operator-=(const vec2 & b);
	vec2 & operator*=(float f);
};


vec2 operator+(vec2 lhs,const vec2 & rhs);
vec2 operator-(vec2 lhs,const vec2 & rhs);
vec2 operator*(float rhs,vec2 lhs);
float operator*(const vec2 & lhs,const vec2 & rhs);
vec2 eR(float angle);

class mat2
{
	public:
	float a;
	float b;
	float c;
	float d;
	mat2(float a,float b,float c, float d);
	mat2(const vec2 & A,const vec2 & B);
	mat2 inv() const;
	mat2 T() const;
	float det() const;
};

vec2 operator*(const mat2 & M,const vec2 & x);
std::array<float,3> vec2sec(vec2 A,vec2 B,vec2 C,vec2 D);

#endif
