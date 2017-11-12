
typedef pair<PT, double> circle;

// determine if point p is inside circle c.
bool contains(const circle& c, const PT& p) {
  return LE(abs(c.A - p), c.B);
}

//  intersection(s) of line a-b with circle @ c with radius r > 0
vector <PT> line_circle(PT a, PT b, PT c, double r) {
    vector<PT> ret;
    b = b - a;
    a = a - c;
    double A = norm(b);
    double B = dot(a, b);
    double C = norm(a) - r*r;
    double D = B*B - A*C;
    if (D < -eps) return ret;
    ret.push_back(c + a + b*(-B + sqrt(D + eps)) / A);
    if (D > eps)
        ret.push_back(c + a + b*(-B - sqrt(D)) / A);
    return ret;
}

//circle inscribed within points a, b, and c
circle incircle(const PT & a, const PT & b, const PT & c) {
  double al = abs(b - c);
  double bl = abs(a - c);
  double cl = abs(a - b);
  double p = al + bl + cl;
  if (EQ(p, 0)) return circle(a, 0);
  circle res;
  double x = (al * a.X + bl * b.X + cl * c.X) / p;
  double y = (al * a.Y + bl * b.Y + cl * c.Y) / p;
  double r = fabs((a.X - c.X) * (b.Y - c.Y) - (a.Y - c.Y) * (b.X - c.X)) / p;
  return circle(PT(x, y), r);
}

//  compute center of circle given three points
pair<PT, double> circle_center(const PT& a, const PT& b, const PT& c) {
    PT ab = (a + b) / double(2);
    PT ac = (a + c) / double(2);
    PT center;
    line_line(ab, ab + rotate_ccw(a - b, PT(0, 0), M_PI/2), ac, ac + rotate_ccw( a - c, PT(0 ,0), M_PI/2), center);
    return MP(center, abs(center - a));
}

//  compute center of circle given oppsite diametral points a and b.
pair<PT, double> circle_center(const PT& a, const PT& b) {
  PT cntr = (a + b) * (1/2.0);
  double r = abs(a - b) * (1/2.0);
  return MP(cntr, r);
}

// given two points on the circle and the radius, find the center.
// set flip to true, to get the second circle.
bool circle_center(PT p1, PT p2, double r, PT &c, bool flip) {
    if (flip) swap(p1, p2);
    double d2 = (p1.X - p2.X) * (p1.X - p2.X) +
        (p1.Y - p2.Y) * (p1.Y - p2.Y);
    double det = r * r / d2 - 0.25;
    if (det < 0.0) return false;
    double h = sqrt(det);
    double x = (p1.X + p2.X) * 0.5 + (p1.Y - p2.Y) * h;
    double y = (p1.Y + p2.Y) * 0.5 + (p2.X - p1.X) * h;
    c = PT(x, y);
    return true;
}

// find lines that are tangent to both circles.
// lines are external
typedef pair<PT, PT> Segment;
pair<Segment,Segment> circle_tangent(circle a, circle b){
  double R2 = a.B;
  double R1 = b.B;
  PT d = b.A - a.A;
  double theta = acos((R2 - R1) / abs(d));

  Segment a1, a2;
  a1.A = a.A + d/PT(abs(d),0.0)*polar(1.0,theta)*R2;
  a2.A = a.A + d/PT(abs(d),0.0)*polar(1.0,-theta)*R2;

  a1.B = b.A + d/PT(abs(d),0.0)*polar(1.0,theta)*R1;
  a2.B = b.A + d/PT(abs(d),0.0)*polar(1.0,-theta)*R1;
  return make_pair(a1,a2);
}

// find tangent lines to a and b. on the inner side
bool inner_tangent(circle a, circle b, pair<Segment, Segment>& tangents) {
  double d = abs(a.A - b.A);
  double R1 = a.B, R2 = b.B;
  if (d - R1 - R2 < eps) return false;

  double x = d * R1 / (R1 + R2);
  double theta = acos(R1 / x);

  PT p1 = rotate_ccw(b.A, a.A, theta); p1 = a.A + (p1 - a.A) * (R1 / abs(p1 - a.A));
  PT p2 = rotate_ccw(a.A, b.A, theta); p2 = b.A + (p2 - b.A) * (R2 / abs(p2 - b.A));
  tangents.A = MP(p1, p2);

  PT q1 = rotate_ccw(b.A, a.A, -theta); q1 = a.A + (q1 - a.A) * (R1 / abs(q1 - a.A));
  PT q2 = rotate_ccw(a.A, b.A, -theta); q2 = b.A + (q2 - b.A) * (R2 / abs(q2 - b.A));
  tangents.B = MP(q1, q2);
  return true;
}

// find center of a circle of radius R that is tangent to both circles centered
// at a and b with radii r1, r2, respectively.
bool circle_tangent(PT a, double r1, PT b, double r2, double R, pair<PT, PT>& posible_circles) {
  if (abs(a - b) - r1 - r2 - 2*R < eps) return false;
  double v = R + r2;
  double u = R + r1;
  double d = abs(a - b);
  double theta = acos((u*u + d*d - v*v) / (2*u*d));

  PT p1 = rotate_ccw(b, a, theta);
  PT p2 = rotate_ccw(b, a, -theta);
  p1 = p1 * (u / abs(p1 - a));
  p2 = p2 * (u / abs(p2 - a));
  posible_circles = MP(p1, p2);
  return true;
}

// The Great-Circle Distance between any two points A and B on sphere
// is the shortest distance along a path on the surface of the sphere
double greater_circle_distance(double pLat, double pLong, double qLat, double qLong, double radius) {
    pLat *= PI / 180; pLong *= PI / 180; // conversion from degree to radian
    qLat *= PI / 180; qLong *= PI / 180;
    return radius * acos(cos(pLat)*cos(pLong)*cos(qLat)*cos(qLong) +
                        cos(pLat)*sin(pLong)*cos(qLat)*sin(qLong) +
                        sin(pLat)*sin(qLat));
}

double circle_area(double R) {
    return PI * R*R;
}

// given circle centered at 0,0 with radius R, finds the area that lies to the
// right of the line x = d
double circle_segment_area(double R, double d) {
    if (d == 0) return circle_area(R) / 2;
    double hh = (R*R - d*d);
    double theta = 2 * acos((R*R + d*d - hh) / (2*R*d));
    return R*R * (theta - sin(theta)) / 2;
}

// return volume of cap sphere with radius R, cap height h
double spherical_cap_volume(double R, double h) {
    return PI * h * h * (3.0 * R - h) / 3.0;
}

double spherical_cap_area(double R, double h) {
    return 2 * PI * R * h;
}

int main() {
  // circle c(-2, 5, sqrt(10)); //(x+2)^2+(y-5)^2=10
  // assert(c == circle(PT(-2, 5), sqrt(10)));
  // assert(c == circle(PT(1, 6), PT(-5, 4)));
  // assert(c == circle(PT(-3, 2), PT(-3, 8), PT(-1, 8)));
  // assert(c == incircle(PT(-12, 5), PT(3, 0), PT(0, 9)));
  // assert(c.contains(PT(-2, 8)) && !c.contains(PT(-2, 9)));
  // assert(c.on_edge(PT(-1, 2)) && !c.on_edge(PT(-1.01, 2)));

  // pair<PT, PT> ct = circle_tangent(PT(0,0), 6, PT(10, 0), 6, 10);
  // cout << ct.first << "\n" << ct.second << "\n";

  circle c1 = MP(PT(0,0), 10);
  circle c2 = MP(PT(30,0), 5);
  pair<Segment,Segment> t;
  if (inner_tangent(c1, c2, t)) {
    cout << t.A.A << " " << t.A.B << "\n";
    cout << t.B.A << " " << t.B.B << "\n";
  }

  return 0;
}