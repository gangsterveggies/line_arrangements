#ifndef _DCEL_
#define _DCEL_

#include "Common.h"
#include "GeoUtils.h"
#include "include/pngwriter.h"
#include "Debug.h"

class DCEL
{
public:
  DCEL();

  void add_polygon(Polygon p);
  void cut_singular(Segment s);
  void cut_double(Segment s1, Segment s2);
  void print();
  void print_image(char* path);

private:
  struct iEdge;

  struct iFace
  {
    vector<int> colors;
    iEdge *rep;
    int index;
  };

  struct iVertex
  {
    iEdge *tail;
    Point coor;
  };

  struct iEdge
  {
    iEdge *next, *prev;
    iEdge *twin;

    iVertex *vbeg, *vend;
    iFace *face;

    Segment seg()
      {
        return Segment((*vbeg).coor, (*vend).coor);
      }
  };

  iFace *out_face;
  double min_x, max_x, min_y, max_y;
  int window_x, window_y, pad;
  Color color_list[10];

  int face_index, next_color;
  set<int> visited;

  iFace* cut_segment(Segment s);
  void fill_single_visible(iFace *face, Segment s);
  void fill_double_visible(iFace *face, Segment s1, Segment s2);
  iVertex* create_vertex(iEdge *edge, Point p);
  iFace* create_face_from(iFace *face);
  void split_edge(iEdge *edge, Point p);
  void cut_face(iEdge* cur, Segment s);
  void print_faces(pngwriter &png, iFace *face, double scale);
  void find_bounds(iFace *face);
};

#endif
