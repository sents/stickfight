#ifndef BEZIER
#define BEZIER

//awesome comment

#include <SDL2/SDL.h>
#include <cmath>
#include <iostream>
#include <vector>	
#include <array>
#include "vec2.h"
class Beznode
{
	public:
		Beznode();
		Beznode(float X,float Y,float Angle,float Tangent1,float Tangent2); //Angle: Tangent Angle at Node; Tangent1/2: Distance of adjacent Bezier-points
		vec2 getCoords() const; //Get Node Coordinates (x,y)
		float getX() const;
		float getY() const;
		float getAngle() const;
		float getTangent1() const;
		float getTangent2() const;
		vec2 getT1Coords() const; //Gives Coordinates of previous Bezier-point
		vec2 getT2Coords() const; //Gives Coordinates of previous Bezier-point
		std::array<float,2> getTangent() const; //Gives Tangent1 and Tangent2
		void setCoords(const vec2 & Coords);
		void setCoords(float X,float Y);
		void setTangent1(float Tangent1);
		void setTangent2(float Tangent2);
		void setAngle(float Angle);
		void rotate(float Angle); 
		void rotateP(vec2 Point,float Angle);
		void translate(float X,float Y);
		private:
		vec2 mVec;
		float mX;
		float mY;
		float mAngle;
		float mTangent1;
		float mTangent2;
		
};
class Bezpath
{
	public:
		Bezpath();
		Bezpath(const std::vector<Beznode> &Nodes);
		void translatePath(float X,float Y); //Translate every Node in Path
		void translatePath(vec2 Vec);
		void rotatePath(float X,float Y,float Angle);  //Rotate Node around Coordinate
		void rotatePath(vec2 Vec, float Angle);
		unsigned int get_nodecount() const;
		void insertNode(const Beznode &Node,unsigned int pos);
		std::array<vec2,4> controlPoints(unsigned int n) const;
		void insertNode(Beznode *Node,unsigned int pos);
		void deleteNode(unsigned int pos);
		void pushNode(const Beznode &Node); //Add Node
		void popNode(); //Pop Node
		vec2 curve(float t) const; 
		bool intersect(std::vector<vec2> Poly) const;
		bool pathsect(const Bezpath &Path) const;
		friend bool Bezsect(const Bezpath &Path,std::vector<vec2> Poly);
		Beznode & node(unsigned int n);
		std::vector<Beznode> mNodes;
};

// Geometry utillity functions
std::vector<vec2> hull(std::array<vec2,4> Points);
bool polysect(const std::vector<vec2> & Poly1,const std::vector<vec2> & Poly2);
std::array<float,3> vecsec(std::array<float,2> A,std::array<float,2> B,std::array<float,2> C,std::array<float,2> D);
float angleFromPoints(std::array<float,2> P1, std::array<float,2> P2);
float distFromPoints(std::array<float,2> P1, std::array<float,2> P2);
Bezpath splitCurve(Beznode Start,Beznode End,float t);
bool Bezsect(const Bezpath &Path,std::vector<vec2> Poly);
void rotatePoints(std::vector<vec2> &Points);
float polyArea(std::vector<vec2> Points);
void polanticlock(std::vector<vec2> &Points);

#endif
