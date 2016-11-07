// This untidy code is Property of Tanuj Khattar

const double EPS = 1e-9,PI = acos(-1);
double DEG_to_RAD(double theta){
  return ((theta*PI)/180.0);
}//************** Point *****************
class point{
  public:
    double x,y;point(){x=0;y=0;}
    point(double _x,double _y):x(_x),y(_y){}
    bool operator < (point other) const{
      if(fabs(x - other.x)>EPS)return x<other.x;
      return y<other.y;
    }bool operator == (point other) const{
      return ((fabs(x-other.x)<EPS )&&(fabs(y-other.y)<EPS));
    }};
double dist(point p1,point p2){
  return hypot(p1.x-p2.x,p1.y-p2.y);
}//rotate p by theta "degrees" counter clockwise wrt origin (0,0)
point rotate(point p,double theta){
  double rad = DEG_to_RAD(theta);
  return point(p.x*cos(rad)-p.y*sin(rad) , p.x*sin(rad)+p.y*cos(rad));
}//************** LINE ******************
//Represented as ax+by+c=0 where b=1 i.e y=(-A)x+(-C)
class line{
  public:
    double a,b,c;line(){a=b=c=0;}
    line(point p1,point p2){
      if(fabs(p1.x-p2.x)<EPS)//vertical line
        a=1.0,b=0.0,c=-p1.x;
      else
        a=-(p2.y-p1.y)/(p2.x-p1.x),b=1,c=-(p1.y + a*p1.x);
    }//slope : m and point p on line
    line(point p,double m){a=-m;b=1;c=-(a*p.x+b*p.y);}
};bool areParallel(line l1,line l2){
  return ((fabs(l1.a-l2.a)<EPS) && (fabs(l1.b-l2.b)<EPS));
}bool areSame(line l1,line l2){
  return (areParallel(l1,l2) && (fabs(l1.c-l2.c)<EPS));
}bool areIntersect(line l1,line l2,point &p){
  if(areParallel(l1,l2)) return false;
  //solve system of 2 linear algebraic equations with 2 unknowns
  p.x = (l2.b*l1.c - l1.b*l2.c)/(l2.a*l1.b - l1.a*l2.b);
  //special case for vertical line to avoid division by zero.
  if(fabs(l1.b)>EPS)p.y= -(l1.a*p.x + l1.c);
  else p.y = -(l2.a*p.x + l2.c);
  return true;
}/************* Vector Algebra *************/
class vect{
  public:
    double x,y;vect():x(0),y(0){}
    vect(double _x,double _y):x(_x),y(_y){}
    vect(point a,point b):x(b.x-a.x),y(b.y-a.y){}
};double dot(vect a,vect b){
  return (a.x*b.x + a.y*b.y);
}double cross(vect a,vect b){
  return a.x*b.y - a.y*b.x;
}//Returns vector of length s along v
vect scale(vect v,double s){
  double k = s/sqrt(dot(v,v));
  return vect(k*v.x,k*v.y);
}//Move point p along direction v by length of v
point translate(point p,vect v){
  return point(p.x+v.x,p.y+v.y);
}/************* Miscellaneous **************/
//dist of p from line(a,b).c:closest point to p on line l
double distToLine(point p,point a,point b,point &c){
  vect ap(a,p),ab(a,b);double u = dot(ap,ab)/sqrt(dot(ab,ab));
  c = translate(a,scale(ab,u));
  return dist(p,c);
}//dist of p from line-segment(a,b).c:closest point to p on line-segment
double distToLineSegment(point p,point a,point b,point &c){
  vect ap(a,p),ab(a,b);double u = dot(ap,ab)/dot(ab,ab);
  if(u<0.0){c=a;return dist(p,a);}//closer to a
  if(u>1.0){c=b;return dist(p,b);}//closer to b
  return distToLine(p,a,b,c);//run dist to line as above
}//returns angle aob in rad
double angle(point a,point o,point b){
  vect oa(o,a),ob(o,b);
  return acos(dot(oa,ob)/sqrt(dot(oa,oa)*dot(ob,ob)));
}//returns true if point r is on left side of line pq
bool ccw(point p,point q,point r){//>= to accept 
  return cross(vect(p,q),vect(p,r))>0;//colliner points
}//returns true if point r is on same line as p,q
bool collinear(point p,point q,point r){
  return fabs(cross(vect(p,q),vect(p,r)))<EPS;
}/************* Polygons ***************/
//Polygon is represented as vector of counter-clockwise points,
//with last point equal to first point. 
double area(vector<point> &P){
  double res=0;
  for(int i=0;i<SZ(P)-1;i++)
    res+=(P[i].x*P[i+1].y-P[i+1].x*P[i].y);
  return fabs(res);
}//Check if given polygon is Convex
bool isConvex(vector<point> &P){
  if(SZ(P)<=3)return false;
  bool isLeft=ccw(P[0],P[1],P[2]);
  for(int i=1;i<SZ(P)-1;i++)
    if(ccw(P[i],P[i+1],P[(i+2)==SZ(P)?1:i+2]) != isLeft)
      return false;
  return true;
}//Returns true if point p is inside (concave/convex)P
bool inPolygon(point p,vector<point>& P){
  if(!SZ(P))return false;
  double sum=0;
  for(int i=0;i<SZ(P)-1;i++)
    if(ccw(p,P[i],P[i+1]))sum+=angle(P[i],p,P[i+1]);
    else sum-=angle(P[i],p,P[i+1]);
  return fabs(fabs(sum)-2*PI)<EPS;
}/************* Convex Hull **************/
point pivot(0,0);
bool angleCmp(point a,point b){
  if(collinear(pivot,a,b))
    return dist(pivot,a)<dist(pivot,b);
  double d1x=a.x-pivot.x,d1y=a.y-pivot.y;
  double d2x=b.x-pivot.x,d2y=b.y-pivot.y;
  return (atan2(d1y,d1x)-atan2(d2y,d2x))<0;
}//returns convex hull of polygon. 
vector<point> CH(vector<point> P){
  int i,j,n=SZ(P);
  if(n<=3){if(!P[0]==P[n-1])P.PB(P[0]);return P;}
  //Find P0=point with lowest Y and if tie:rightmost X
  int P0=0;
  for(int i=1;i<n;i++)
    if(P[i].y<P[P0].y||(P[i].y==P[P0].y&&P[i].x>P[P0].x))
      P0=i;
  swap(P[P0],P[0]);
  pivot=P[0];
  sort(P.begin()+1,P.end(),angleCmp);
  vector<point>S;
  S.PB(P[n-1]);S.PB(P[0]);S.PB(P[1]);i=2;
  while(i<n){//change ccw to accept collinear point if required
    j=SZ(S)-1;if(ccw(S[j-1],S[j],P[i]))S.PB(P[i++]);
    else S.pop_back();
  }return S;
}
