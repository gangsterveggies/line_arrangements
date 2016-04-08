#ifndef _GEOUTILS_
#define _GEOUTILS_

#include "Common.h"
#include "include/pngwriter.h"
#include "Debug.h"

class GeoUtils
{
public:
  static double cross3(Point a, Point b, Point c);
  static int ccw(Point a, Point b, Point c);
  static int on_seg(Segment l, Point p);
  static int intersects(Segment l1, Segment l2);
  static Line line_from_seg(Segment s);
  static Point line_intersect(Line l1, Line l2);
  static Point seg_seg_intersect(Segment s1, Segment s2);
  static double dist(Point a, Point b);
  static double dist2(Point a, Point b);
  static double dot(Point a, Point b);
  static Polygon read_polygon(int n);
  static Segment read_segment();
  static void print_point(Point p);
  static void print_segment(Segment s);
  static void draw_segment(pngwriter &png, Segment s, double scale, double xoffset, double yoffset);

private:
};

#endif
