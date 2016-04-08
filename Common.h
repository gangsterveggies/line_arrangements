#ifndef _COMMON_
#define _COMMON_

#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <algorithm>
#include <vector>
#include <set>

using namespace std;

#define EPS (1e-6)

struct Point
{
  double X, Y;
  Point(){}
  Point(double _x, double _y) : X(_x), Y(_y) {}
  Point operator + (const Point &p)  const { return Point(X + p.X, Y + p.Y); }
  Point operator - (const Point &p)  const { return Point(X - p.X, Y - p.Y); }
  Point operator * (double c) const { return Point(X * c, Y * c); }
  Point operator / (double c) const { return Point(X / c, Y / c); }
};

struct Segment
{
  Point p0, p1;
  Segment(){}
  Segment(Point _p0, Point _p1) : p0(_p0), p1(_p1) {}
};

struct Line
{
  double a, b, c; // ax + by = c
  Line(){}
  Line(double _a, double _b, double _c) : a(_a), b(_b), c(_c) {}
};

struct Color
{
  double R, G, B;
  Color(){}
  Color(double _r, double _g, double _b) : R(_r), G(_g), B(_b) {}
};

typedef vector<Point> Polygon;

#endif
