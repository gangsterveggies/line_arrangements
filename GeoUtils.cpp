#include "GeoUtils.h"

double GeoUtils::cross3(Point a, Point b, Point c)
{ 
  return (c.X - b.X) * (a.Y - b.Y) - (c.Y - b.Y) * (a.X - b.X);
}

int GeoUtils::ccw(Point a, Point b, Point c)
{
  return cross3(a, b, c) > EPS;
}

int GeoUtils::on_seg(Segment l, Point p)
{
  if (min(l.p0.X, l.p1.X) <= p.X && p.X <= max(l.p0.X, l.p1.X) && min(l.p0.Y, l.p1.Y) <= p.Y && p.Y <= max(l.p0.Y, l.p1.Y))
    return 1;
  return 0;
}

int GeoUtils::intersects(Segment l1, Segment l2)
{
  double d1 = cross3(l1.p0, l1.p1, l2.p0);
  double d2 = cross3(l1.p0, l1.p1, l2.p1);
  double d3 = cross3(l2.p0, l2.p1, l1.p0);
  double d4 = cross3(l2.p0, l2.p1, l1.p1);
  
  if (((d1 < -EPS && d2 > EPS) || (d1 > EPS && d2 < -EPS)) && ((d3 < -EPS && d4 > EPS) || (d3 > EPS && d4 < -EPS)))
    return 1;
  if (fabs(d1) <= EPS && on_seg(l1, l2.p0) || (fabs(d2) <= EPS && on_seg(l1, l2.p1)) || (fabs(d3) <= EPS && on_seg(l2, l1.p0)) || (fabs(d4) <= EPS && on_seg(l2, l1.p1)))
    return 1;
  
  return 0;
}

Line GeoUtils::line_from_seg(Segment s)
{
  Line nline = Line(s.p0.Y - s.p1.Y, s.p1.X - s.p0.X, 0);
  nline.c = nline.a * s.p0.X + nline.b * s.p0.Y;
  return nline;
}

Point GeoUtils::line_intersect(Line l1, Line l2)
{
  return Point(l1.c * l2.b - l2.c * l1.b, l1.a * l2.c - l2.a * l1.c) / (l1.a * l2.b - l2.a * l1.b);
}

Point GeoUtils::seg_seg_intersect(Segment s1, Segment s2)
{
  Line l1 = line_from_seg(s1);
  Line l2 = line_from_seg(s2);
  return line_intersect(l1, l2);
}

double GeoUtils::dist(Point a, Point b)
{
  return sqrt(dist2(a, b));
}

double GeoUtils::dist2(Point a, Point b)
{
  return dot(a - b, a - b);
}

double GeoUtils::dot(Point a, Point b)
{
  return a.X * b.X + a.Y * b.Y;
}

Polygon GeoUtils::read_polygon(int n)
{
  int i;
  double x, y;
  Polygon p;

  for (i = 0; i < n; i++)
  {
    scanf("%lf %lf", &x, &y);
    p.push_back(Point(x, y));
  }

  return p;
}

Segment GeoUtils::read_segment()
{
  double x0, y0, x1, y1;
  scanf("%lf %lf %lf %lf", &x0, &y0, &x1, &y1);
  return Segment(Point(x0, y0), Point(x1, y1));
}

void GeoUtils::print_point(Point p)
{
  Debug::print("%0.2lf %0.2lf\n", p.X, p.Y);
}

void GeoUtils::print_segment(Segment s)
{
  Debug::print("%0.2lf %0.2lf - %0.2lf %0.2lf\n", s.p0.X, s.p0.Y, s.p1.X, s.p1.Y);
}

void GeoUtils::draw_segment(pngwriter &png, Segment s, double scale, double xoffset, double yoffset)
{
  png.line(s.p0.X * scale + xoffset,
           s.p0.Y * scale + yoffset,
           s.p1.X * scale + xoffset,
           s.p1.Y * scale + yoffset, 0, 0, 0);
}
