#include "bezier.h"

//Beznode class functions

Beznode::Beznode(): mVec(0,0),mX(0),mY(0),mAngle(0),mTangent1(0),mTangent2(0)
{}

Beznode::Beznode(float nX,float nY,float Angle,float Tangent1,float Tangent2) : mVec(nX,nY),mX(nX),mY(nY),mAngle(Angle),mTangent1(Tangent1),mTangent2(Tangent2)
{
}


float Beznode::getAngle() const
{
	return mAngle;
}


float Beznode::getX() const
{
	return mVec.X;
}

float Beznode::getY() const
{
	return mVec.Y;
}

float Beznode::getTangent1() const
{
	return mTangent1;
}

float Beznode::getTangent2() const
{
	return mTangent2;
}

vec2 Beznode::getCoords() const
{
	return mVec;
}


std::array<float,2> Beznode::getTangent() const
{
	return {{mTangent1,mTangent2}};

}

vec2 Beznode::getT1Coords() const
{
return mVec-(mTangent1)*eR(mAngle);
}

vec2 Beznode::getT2Coords() const
{
	return mVec+mTangent2*eR(mAngle);
}

void Beznode::setAngle(float Angle)
{
	Angle -= static_cast<int>(Angle/(2*M_PI))*2*M_PI;
	mAngle=Angle;
}

void Beznode::setCoords(float X,float Y)
{
	mVec.X=X;	
	mVec.Y=Y;
}

void Beznode::setCoords(const vec2 & Coords)
{
	mVec=Coords;
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
void Beznode::rotateP(vec2 Point,float Angle)
{
	if (std::abs(mAngle+Angle)>=2*M_PI)
	{
		mAngle = (mAngle+Angle)-static_cast<int>((mAngle+Angle)/(2*M_PI))*2*M_PI;
	}
	else
	{
		mAngle += Angle;
	}
	mVec = Point+Point.distTo(mVec)*eR(Point.angTo(mVec)+Angle);

}

void Beznode::translate(float X,float Y)
{
	mVec += {X,Y};
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

Beznode & Bezpath::node(unsigned int n)
{
	return mNodes.at(n);
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
void Bezpath::translatePath(vec2 Vec)
{
	translatePath(Vec.X,Vec.Y);
}

void Bezpath::rotatePath(vec2 Point, float Angle)
{
	for(std::vector<Beznode>::iterator i = mNodes.begin();i<mNodes.end();i++)
	{
		i->rotateP(Point,Angle);
	}
}

void Bezpath::rotatePath(float X,float Y, float Angle)
{
	rotatePath({X,Y},Angle);
}


std::array<vec2,4> Bezpath::controlPoints(unsigned int n ) const
{
	if (n>=mNodes.size())
	{n=mNodes.size()-1;}
	std::array<vec2,4> Points = {{mNodes.at(n).getCoords(),mNodes.at(n).getT2Coords(),mNodes.at(n+1).getT1Coords(),mNodes.at(n+1).getCoords()}};
	return Points;
}


vec2 Bezpath::curve(float t) const
{
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
	std::array<vec2,4> P=this->controlPoints(n);
	return (1-t)*((1-t)*((1-t)*P.at(0)+t*P.at(1))+t*((1-t)*P.at(1)+t*P.at(2)))+t*((1-t)*((1-t)*P.at(1)+t*P.at(2))+t*((1-t)*P.at(2)+t*P.at(3)));
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

bool Bezpath::intersect(std::vector<vec2> Poly) const
{
	return Bezsect(*this,Poly);
}


		
bool Bezpath::pathsect(const Bezpath &Path) const
{
	bool col;
	float minsize = .3;
	std::vector<vec2> curHull;
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


bool Bezsect(const Bezpath &Path,std::vector<vec2> Poly)
{
	bool col;
	float minsize = .3;
	std::vector<vec2> curHull;
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

void polanticlock(std::vector<vec2> & Points)
{
	if (Points.size()>2)
	{
		if (Points.at(0).xTo(Points.at(1))<0)
		{
			Points.crbegin();
		}
	}
}


std::vector<vec2> hull(std::array<vec2,4> Points)
{
	float det;
	float x0,x1;
	bool dcount=false;
	int front = 0;
	std::array<float,3> inter;
	std::vector<vec2> out;
	for (std::array<vec2,4>::iterator i=Points.begin()+1;i<Points.end();i++)
	{
		inter = vec2sec(Points.at(0),*i,*(Points.begin()+1+(i-(Points.begin()+1)+1)%3),*(Points.begin()+1+(i-(Points.begin()+1)+2)%3));
		det = inter.at(0);
		x0 = inter.at(1);
		x1 = inter.at(2);
		std::cout << "Hull: Gang: " << i-Points.begin() << " det= " << det << " x0 = " << x0 << " x1 = " << x1 << std::endl << std::flush;
		if (det!=0)
		{
			if (x0 > 0 && x0<1 && x1 > 0 && x1 < 1) //if true then quadrangle with AI and I+1I+2 intersection
			{
				return {Points.at(0),*(Points.begin()+1+(i-(Points.begin()+1)+1)%3),*i,*(Points.begin()+1+(i-(Points.begin()+1)+2)%3)};
			}
			else if (x0 == 0)
			{	
				return {Points.at(1),Points.at(2),Points.at(3)};
			}
			else if (x0 == 1 && x1 <= 1 && x1 >= 0)
			{
				return {Points.at(0),*(Points.begin()+1+(i-(Points.begin()+1)+1)%3),*(Points.begin()+1+(i-(Points.begin()+1)+2)%3)};
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

std::vector<vec2> hull(vec2 A,vec2 C,vec2 B,vec2 D)
{
	return hull({{A,B,C,D}});
}

bool polysect(const std::vector<vec2> & Poly1,const std::vector<vec2> & Poly2)
{
	std::array<float,3> sec;
	std::vector<vec2> curPol1 = Poly1;
	std::vector<vec2> curPol2 = Poly2;
	bool col;
	vec2 x;
	for (int i = 0;i<2;i++)
	{
		if (i==1)
		{
			curPol1 = Poly2;
			curPol2 = Poly1;
		}
	if (curPol1.size()>3) // split Poly1 into triangles
	{
		if (polysect({curPol1.at(0),curPol1.at(1),curPol1.at(2)},curPol2)==true)
		{
			return true;
		}
		curPol1.erase(curPol1.begin()+1);
		return polysect(curPol1,curPol2);
	}
	else if (curPol1.size() == 3)
	{
		for (auto i: curPol2)
		{
			x = mat2(curPol1.at(1)-curPol1.at(0),curPol1.at(2)-curPol1.at(0)).inv()*(i-curPol1.at(0));	
			if (true?(x.X>=0 && x.X <=1 && x.Y>=0 && x.Y<=1 && (x.Y+x.X)<=1):false)
			{
				return true;
			}
		}

	}
	else if (curPol1.size() == 2)
	{
		for (std::vector<vec2>::iterator it = curPol2.begin();it<curPol2.end();it++)
		{
			sec = vec2sec(curPol1.at(0),curPol1.at(1),*it,*(curPol2.begin()+(it-curPol2.begin()+1)%curPol2.size()));
			
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
	std::array<std::vector<vec2>,4> Points;
	Points.at(0)={Start.getCoords(),Start.getT2Coords(),End.getT1Coords(),End.getCoords()};
	for (int i = 1; i<=3;i++)
	{
		for (int j = 0;j <= 3-i;i++)
		{
			Points.at(i).at(j)=(1-t)*Points.at(i-1).at(j)+t*Points.at(i-1).at(j+1);

		}
	}
	P1.setAngle(Points.at(0).at(0).angTo(Points.at(1).at(0)));
	P1.setTangent2(Points.at(0).at(0).distTo(Points.at(1).at(0)));
	P2.setCoords(Points.at(3).at(0));
	P2.setAngle(Points.at(2).at(0).angTo(Points.at(3).at(0)));
	P2.setTangent1(Points.at(2).at(0).distTo(Points.at(3).at(0)));
	P2.setTangent2(Points.at(2).at(1).distTo(Points.at(3).at(0)));
	P3.setAngle(Points.at(1).at(2).angTo(Points.at(0).at(3)));
	P3.setTangent1(Points.at(1).at(2).distTo(Points.at(0).at(3)));
	Bezpath Path({P1,P2,P3});
	return Path;
}




void  rotatePoints(std::vector<vec2> &Points,vec2 rotPoint,float Angle)
{
	for(std::vector<vec2>::iterator i = Points.begin();i<Points.end();i++)
	{
		*i = rotPoint+ i->distTo(rotPoint)*eR(rotPoint.angTo(*i)+Angle);
	}
}

//Area for Polygons
float polyArea(std::vector<vec2> Points)
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
		rotatePoints(Points,Points.at(0),M_PI/2-Points.at(0).angTo(Points.at(1)));
		P={Points.at(0).X,Points.at(2).Y};
		return Points.at(2).distTo(P)*Points.at(0).distTo(Points.at(1))/2;
	}
	else if (Points.size() == 2)
	{
		return Points.at(0).distTo(Points.at(1));
	}
	else
	{
		std::cout << "polyArea needs min 2 Points! returning 0" << std::endl;
		return 0;
	}
	return 0;
}
