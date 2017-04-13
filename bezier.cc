#include "bezier.h"

//Beznode class functions

Beznode::Beznode(): mX(0),mY(0),mAngle(0),mTangent1(0),mTangent2(0)
{}

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
	return {{mX,mY}};
}


std::array<float,2> Beznode::getTangent() const
{
	return {{mTangent1,mTangent2}};

}

std::array<float,2> Beznode::getT1Coords() const
{
	float PX = std::cos(mAngle)*mTangent1*(-1)+mX;
	float PY = std::sin(mAngle)*mTangent1*(-1)+mY;
	return {{PX,PY}};
}

std::array<float,2> Beznode::getT2Coords() const
{
	float PX = std::cos(mAngle)*mTangent2+mX;
	float PY = std::sin(mAngle)*mTangent2+mY;
	return {{PX,PY}};
}

void Beznode::setAngle(float Angle)
{
	Angle -= static_cast<int>(Angle/(2*M_PI))*2*M_PI;
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
	if (std::abs(mAngle+Angle)>=2*M_PI)
	{
		mAngle = (mAngle+Angle)-static_cast<int>((mAngle+Angle)/(2*M_PI))*2*M_PI;
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
Bezpath::Bezpath(const std::vector<Beznode> &Nodes)
{
	mNodes=Nodes;
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

	float R,a0;
	for(std::vector<Beznode>::iterator i = mNodes.begin();i<mNodes.end();i++)
	{
		R=distFromPoints({X,Y},i->getCoords());
		
		a0=angleFromPoints({X,Y},i->getCoords());
		i->setCoords({X+R*std::cos(Angle+a0),Y+R*std::sin(Angle+a0)});
		i->rotate(Angle);
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

bool Bezpath::intersect(std::vector<std::array<float,2>> Poly) const
{
	return Bezsect(*this,Poly);
}


		
bool Bezpath::pathsect(const Bezpath &Path) const
{
	bool col;
	float minsize = .3;
	std::vector<std::array<float,2>> curHull;
	Bezpath fine;
	col = false;
	for (std::vector<Beznode>::const_iterator it = mNodes.begin();it<mNodes.end()-1;it++)
	{
		curHull=hull({it->getCoords(),it->getT2Coords(),(it+1)->getT1Coords(),(it+1)->getCoords()});
		if (Path.intersect(curHull) == true)
		{
			if (polyArea(curHull) > minsize)
			{
				fine = splitCurve(*it,*(it+1),.5);
				col = fine.pathsect(Path);
			}
			else
			{
				return true;
			}
		}
	}
	return col;
}


bool Bezsect(const Bezpath &Path,std::vector<std::array<float,2>> Poly)
{
	bool col;
	float minsize = .3;
	std::vector<std::array<float,2>> curHull;
	Bezpath fine;
	col = false;
	for (std::vector<Beznode>::const_iterator it = Path.mNodes.begin();it<Path.mNodes.end()-1;it++)
	{
		curHull=hull({it->getCoords(),it->getT2Coords(),(it+1)->getT1Coords(),(it+1)->getCoords()});
		if (polysect(curHull,Poly) == true)
		{
			//cancel condition for recursion:
			if (polyArea(curHull) > minsize)
			{
				fine = splitCurve(*it,*(it+1),.5);
				col = Bezsect(fine,Poly);

			}
			else
			{
				return true;
			}
				
		}
	}
	return col;
}



std::vector<std::array<float,2>> hull(std::array<std::array<float,2>,4> Points)
{
	float det;
	float x0,x1;
	bool dcount=false;
	int front = 0;
	std::array<float,3> inter;
	std::vector<std::array<float,2>> out;
	for (std::array<std::array<float,2>,4>::iterator i=Points.begin()+1;i<Points.end();i++)
	{
		inter = vecsec(Points.at(0),*i,*(Points.begin()+1+(i-(Points.begin()+1)+1)%3),*(Points.begin()+1+(i-(Points.begin()+1)+2)%3));
		det = inter.at(0);
		x0 = inter.at(1);
		x1 = inter.at(2);
		if (det!=0)
		{
			if (x0 > 0 && x0<1 && x1 > 0 && x1 < 1) //if true then quadrangle with AI and I+1I+2 intersection
			{
				out = {Points.at(0),*(Points.begin()+1+(i-(Points.begin()+1)+1)%3),*i,*(Points.begin()+1+(i-(Points.begin()+1)+2)%3)};
				return out;
			}
			else if (x0 == 0)
			{	
				out = {Points.at(1),Points.at(2),Points.at(3)};
				return out;
			}
			else if (x0 == 1 && x1 <= 1 && x1 >= 0)
			{
				out = {Points.at(0),*(Points.begin()+1+(i-(Points.begin()+1)+1)%3),*(Points.begin()+1+(i-(Points.begin()+1)+2)%3)};
			}
			else if (x0 > 1)
			{front = i-Points.begin(); 
			}
		}
		else
		{
			dcount = true;
		}
	}
	if (dcount==true)
	{
			return {Points.at(0),Points.at(3)};
	}
	out = {Points.at((front+1)%4),Points.at((front+2)%4),Points.at((front+3)%4)};
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
	float det = (A.at(0)-B.at(0))*(D.at(1)-C.at(1))-(A.at(1)-B.at(1))*(D.at(0)-C.at(0));
	if (det==0)
	{	return {{0,0,0}};}
//	float x0 = (D.at(1)-C.at(1))*(A.at(0)-C.at(1))+(C.at(0)-D.at(1))*(A.at(1)-C.at(1));
//	float x1 = (B.at(1)-A.at(1))*(A.at(0)-C.at(1))+(A.at(0)-B.at(0))*(A.at(1)-C.at(1));
	return {{det, ((D.at(1)-C.at(1))*(A.at(0)-C.at(0))+(C.at(0)-D.at(0))*(A.at(1)-C.at(1)))/det ,((B.at(1)-A.at(1))*(A.at(0)-C.at(0))+(A.at(0)-B.at(0))*(A.at(1)-C.at(1)))/det}};
}


float angleFromPoints(std::array<float,2> P1, std::array<float,2> P2)
{
	return std::atan2(P2.at(1)-P1.at(1),P2.at(0)-P1.at(0));
}

float distFromPoints(std::array<float,2> P1, std::array<float,2> P2)
{
	return std::sqrt(pow(P2.at(1)-P1.at(1),2)+pow(P2.at(0)-P1.at(0),2));
}



Bezpath splitCurve(Beznode Start,Beznode End,float t)
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
	P2.setCoords(Points.at(3).at(0));
	P2.setAngle(angleFromPoints(Points.at(2).at(0),Points.at(3).at(0)));
	P2.setTangent1(distFromPoints(Points.at(2).at(0),Points.at(3).at(0)));
	P2.setTangent2(distFromPoints(Points.at(2).at(1),Points.at(3).at(0)));
	P3.setAngle(angleFromPoints(Points.at(1).at(2),Points.at(0).at(3)));
	P3.setTangent1(distFromPoints(Points.at(1).at(2),Points.at(0).at(3)));
	Bezpath Path({P1,P2,P3});
	return Path;
}




void  rotatePoints(std::vector<std::array<float,2>> &Points,std::array<float,2> rotPoint,float Angle)
{

	float R,a0;
	for(std::vector<std::array<float,2>>::iterator i = Points.begin();i<Points.end();i++)
	{
		R=distFromPoints(rotPoint,*i);
		
		a0=angleFromPoints(rotPoint,*i);
		*i={{rotPoint.at(0)+R*std::cos(Angle+a0),rotPoint.at(1)+R*std::sin(Angle+a0)}};
	}
}

//Area for Polygons
float polyArea(std::vector<std::array<float,2>> Points)
{
	float a=0;
	std::array<float,2> P;
	if (Points.size()>3)
	{
		a = polyArea({Points.at(0),Points.at(1),Points.at(2)});
		Points.erase(Points.begin()+1);
		return a+polyArea(Points);
	}
	else if (Points.size() == 3)
	{			
		rotatePoints(Points,Points.at(0),M_PI/2-angleFromPoints(Points.at(0),Points.at(1)));
		P={Points.at(0).at(0),Points.at(2).at(1)};
		return distFromPoints(Points.at(2),P)*distFromPoints(Points.at(0),Points.at(1))/2;
	}
	else if (Points.size() == 2)
	{
		return distFromPoints(Points.at(0),Points.at(1));
	}
	else
	{
		std::cout << "polyArea needs min 2 Points! returning 0" << std::endl;
		return 0;
	}
	return 0;
}
