#ifndef BEZIER
#define BEZIER

//awesome comment

#include <SDL2/SDL.h>
#include <cmath>
#include <iostream>
#include <vector>	
#include <array>
class Beznode
{
	public:
		Beznode(float X,float Y,float Angle,float Tangent1,float Tangent2); //Angle: Tangent Angle at Node; Tangent1/2: Distance of adjacent Bezier-points
		std::array<float,2> getCoords(); //Get Node Coordinates (x,y)
		float getX();
		float getY();
		float getAngle();
		float getTangent1();
		float getTangent2();
		std::array<float,2> getT1Coords(); //Gives Coordinates of previous Bezier-point
		std::array<float,2> getT2Coords(); //Gives Coordinates of previous Bezier-point
		std::array<float,2> getTangent(); //Gives Tangent1 and Tangent2
		void setCoords(std::array<float,2> Coords);
		void setCoords(float X,float Y);
		void setTangent1(float Tangent1);
		void setTangent2(float Tangent2);
		void setAngle(float Angle);
		void rotate(float Angle); 
		void translate(float X,float Y);
	private:
		float mX;
		float mY;
		float mAngle;
		float mTangent1;
		float mTangent2;
		
};
class Bezpath
{
	public:
		Bezpath(std::vector<Beznode> *Nodes);
		void translatePath(float X,float Y); //Translate every Node in Path
		void rotatePath(float X,float Y,float Angle);  //Rotate Node around Coordinate
		unsigned int get_nodecount();
		void insertNode(Beznode *Node,unsigned int pos);
		void deleteNode(unsigned int pos);
		void pushNode(Beznode *Node); //Add Node
		void popNode(); //Pop Node
		std::array<float,2> curve(float t); 
		std::vector<Beznode> mNodes;
};



#endif
