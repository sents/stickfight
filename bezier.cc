#include "bezier.h"

//Beznode class functions
Beznode::Beznode(float X,float Y,float Angle,float Tangent1,float Tangent2) : mX(X),mY(Y),mAngle(Angle),mTangent1(Tangent1),mTangent2(Tangent2)
{
}


float Beznode::getAngle() const
{
	return mAngle;
}


float Beznode::getX() const
{
	return mX;
}

float Beznode::getY() const
{
	return mY;
}

float Beznode::getTangent1() const
{
	return mTangent1;
}

float Beznode::getTangent2() const
{
	return mTangent2;
}

std::array<float,2> Beznode::getCoords() const
{
	std::array<float,2> Coords = {{mX,mY}};
	return Coords;
}


std::array<float,2> Beznode::getTangent() const
{
	std::array<float,2> Coords = {{mTangent1,mTangent2}};
	return Coords;
}

std::array<float,2> Beznode::getT1Coords() const
{
	float PX = std::sin(mAngle/360*2*M_PI)*mTangent1+mX;
	float PY = std::cos(mAngle/360*2*M_PI)*mTangent1+mY;
	std::array<float,2> Coords = {{PX,PY}};
	return Coords;
}

std::array<float,2> Beznode::getT2Coords() const
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

unsigned int Bezpath::get_nodecount() const
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

	Angle = Angle/360*M_PI*2;
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


std::array<std::array<float, 2>, 4> Bezpath::controlPoints(unsigned int n )
{
	if (n>=mNodes.size())
	{n=mNodes.size()-1;}
	std::array<std::array<float,2>,4> Points = {{mNodes.at(n).getCoords(),mNodes.at(n).getT2Coords(),mNodes.at(n+1).getT1Coords(),mNodes.at(n+1).getCoords()}};
	return Points;
}


std::array<float,2> Bezpath::curve(float t) const
{
	float X;
	float Y;
	int n=static_cast<int>(t);
	if (t<0)
	{t=0;}
	else if (t>get_nodecount()-1)
	{t=get_nodecount()-1;}
	if ( t == n )
	{
		return mNodes.at(n).getCoords();
	}
	t-=n;
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


bool intersect(const Bezpath &Path)
{
	
}

/*
std::array<float,3> Bezpath::koeff(int n)
{
	float P0X = mNodes.at(n).getX();
	float P1X = mNodes.at(n).getT2Coords().at(0);
	float P2X = mNodes.at(n+1).getT1Coords().at(0);
	float P3X = mNodes.at(n+1).getX();
	float P0Y = mNodes.at(n).getY();
	float P1Y = mNodes.at(n).getT2Coords().at(1);
	float P2Y = mNodes.at(n+1).getT1Coords().at(1);
	float P3Y = mNodes.at(n+1).getY();
	std::<float,3> vec = {{P}}

}
*/

int triangleorient(std::array<float,2> A,std::array<float,2> C,std::array<float,2> B)
{
	float result = B[0]*C[1]-B[1]*C[0]-B[0]*A[1]+B[1]*A[0]-C[1]*A[0]+C[0]*A[1];
	if (result<0)
	{return -1;}
	else if (result>0)
	{return 1;}
	else
	{return 0;}
}

std::vector<std::array<float,2>> hullfor4(std::array<float,2> A,std::array<float,2> C,std::array<float,2> B,std::array<float,2> D)
{
	std::vector<std::array<float,2>> hull;
	//Checking Orientation of partial triangles
	int triangle_ABC=triangleorient(A,B,C);
	int triangle_ABD=triangleorient(A,B,D);
	int triangle_BCD=triangleorient(B,C,D);
	int triangle_CAD=triangleorient(C,A,D);
	int zahl= triangle_ABC+triangle_ABD+triangle_BCD+triangle_CAD;
	if (abs(zahl) == 4)
	{
		hull.push_back(A);
		hull.push_back(B);
		hull.push_back(C);

	}
	else if (abs(zahl) == 3)
	{
		hull.push_back(A);
		hull.push_back(B);
		hull.push_back(C);
		hull.push_back(D);
	}
	else if (abs(zahl) == 2)
	{
		if (triangle_ABC == triangle_ABD)
		{
			hull.push_back(A);
			hull.push_back(B);
			hull.push_back(D);
		}
		if (triangle_ABC == triangle_BCD)
		{
			hull.push_back(B);
			hull.push_back(C);
			hull.push_back(D);
		}
		if (triangle_ABC == triangle_CAD)
		{
			hull.push_back(C);
			hull.push_back(A);
			hull.push_back(D);
		}
	}
	else
	{
	       	hull.push_back(A);
		hull.push_back(B);
	}
	return hull;
}


std::vector<std::array<float,2>> hull(std::array<std::array<float,2>,4> Points)
{
	float det;
	float x0,x1;
	int front = 0;
	std::vector<std::array<float,2>> out;
	for (std::array<std::array<float,2>,4>::iterator i=Points.begin()+1;i<Points.end();i++)
	{
		std::array<float,3> inter = vecsec(Points.at(0),*i,*(Points.begin()+(i-Points.begin()+1)%3),*(Points.begin()+(i-Points.begin()+2)%3));
		/*
		a=Points.at(0).at(0)-i->at(0);
		
		b=((Points.begin()+(i-Points.begin()+2)%3)->at(0)-(Points.begin()+(i-Points.begin()+1)%3)->at(0));
		c=Points.at(0).at(1)-i->at(0);
		d=((Points.begin()+(i-Points.begin()+2)%3)->at(1)-(Points.begin()+(i-Points.begin()+1)%3)->at(1));
		det=a*d-b*c;
		y0=Points.at(0).at(0)-(Points.begin()+(i-Points.begin()+1)%3)->at(0);
		y1=Points.at(0).at(1)-(Points.begin()+(i-Points.begin()+1)%3)->at(1);
		*/
		det = inter.at(0);
		x0 = inter.at(1);
		x1 = inter.at(2);
		if (det!=0)
		{
			//x0=(y0*d-b*y1)/det;
			//x1=(a*y1-c*y0)/det;
			if (x0 >= 0 && x0<=1 && x1 >= 0 && x1 <= 0) //if true then quadrangle with AI and I+1I+2 intersection
			{
				out = {Points.at(0),*(Points.begin()+(i-Points.begin()+1)%3),*i,*(Points.begin()+(i-Points.begin()+1)%3)};
				return out;
			}
			else if (x0 == 0)
			{
				out = {Points.at(1),Points.at(2),Points.at(3)};
				return out;
			}
			else if (x0 == 1 && x1 <= 1 && x1 >= 0)
			{
				out = {Points.at(0),*(Points.begin()+(i-Points.begin()+1)%3),*(Points.begin()+(i-Points.begin()+2)%3)};
			}
			else if (x0 >= 1)
			{front = i-Points.begin()+1; 
			}
		}
		else
		{
			out = {Points.at(0),Points.at(3)};
			return out;
		}
	}
	out = {Points.at(front+1%4),Points.at(front+2%4),Points.at(front+3%4)};
	return out;
}

std::vector<std::array<float,2>> hull(std::array<float,2> A,std::array<float,2> C,std::array<float,2> B,std::array<float,2> D)
{
	return hull({{A,B,C,D}});
}

bool polysect(std::vector<std::array<float,2>> Poly1,std::vector<std::array<float,2>> Poly2)
{
	std::array<float,3> sec;
	for (std::vector<std::array<float,2>>::iterator it1 = Poly1.begin();it1<Poly1.end();it1++)
	{
		for (std::vector<std::array<float,2>>::iterator it2 = Poly1.begin();it2<Poly1.end();it2++)
		{
			sec = vecsec(*it1,*(Poly1.begin()+(it1-Poly1.begin())%Poly1.size()),*it2,*(Poly2.begin()+(it2-Poly2.begin())%Poly2.size()));
			if (sec.at(0) != 0 && sec.at(1)  >= 0 && sec.at(1) <= 1 && sec.at(2) >= 0 && sec.at(1) <= 1)
			{
				return true;
			}

		}

	}
	return false;
}

std::array<float,3> vecsec(std::array<float,2> A,std::array<float,2> B,std::array<float,2> C,std::array<float,2> D)
{
	float det = (A.at(0)-B.at(1))*(D.at(1)-C.at(1))-(A.at(1)-B.at(1))*(D.at(0)-C.at(0));
	if (det==0)
	{	return {{0,0,0}};}
//	float x0 = (D.at(1)-C.at(1))*(A.at(0)-C.at(1))+(C.at(0)-D.at(1))*(A.at(1)-C.at(1));
//	float x1 = (B.at(1)-A.at(1))*(A.at(0)-C.at(1))+(A.at(0)-B.at(0))*(A.at(1)-C.at(1));
	return {{det, (D.at(1)-C.at(1))*(A.at(0)-C.at(1))+(C.at(0)-D.at(1))*(A.at(1)-C.at(1)) ,(B.at(1)-A.at(1))*(A.at(0)-C.at(1))+(A.at(0)-B.at(0))*(A.at(1)-C.at(1))}};
}


float angleFromPoints(std::array<float,2> P1, std::array<float,2> P2)
{
	return std::atan2(P2.at(1)-P1.at(1),P2.at(0)-P1.at(0));
}

float distFromPoints(std::array<float,2> P1, std::array<float,2> P2)
{
	return std::sqrt(pow(P2.at(1)-P1.at(1),2)+pow(P2.at(0)-P1.at(0),2));
}



std::array<Beznode,3> splitCurve(Beznode Start,Beznode End,float t)
{
	Beznode P1 = Start;
	Beznode P2(0,0,0,0,0);
	Beznode P3 = End;
	std::array<std::vector<std::array<float,2>>,4> Points;
	Points.at(0)={Start.getCoords(),Start.getT2Coords(),End.getT1Coords(),End.getCoords()};
	for (int i = 1; i<=3;i++)
	{
		for (int j = 0;j <= 3-i;i++)
		{
			Points.at(i).at(j)={{Points.at(i-1).at(j).at(0)*(1-t)+Points.at(i-1).at(j+1).at(0)*t,Points.at(i-1).at(j).at(1)*(1-t)+Points.at(i-1).at(j+1).at(1)*t}};

		}
	}
	P1.setAngle(angleFromPoints(Points.at(0).at(0),Points.at(1).at(0)));
	P1.setTangent2(distFromPoints(Points.at(0).at(0),Points.at(1).at(0)));





	return {P1,P2,P3};
}
