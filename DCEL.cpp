#include "DCEL.h"
#include <assert.h>

DCEL::DCEL()
{
  face_index = next_color = 0;
  window_x = 600, window_y = 400, pad = 20;
  out_face = NULL;
  color_list[0] = Color(1, 0, 0);
  color_list[1] = Color(1, 1, 0);
  color_list[2] = Color(0, 1, 0);
  color_list[3] = Color(0, 1, 1);
  color_list[4] = Color(1, 0, 1);
}

void DCEL::add_polygon(Polygon p)
{
  iEdge *initial = new iEdge(), *cur;
  out_face = new iFace();
  out_face->rep = initial;
  out_face->index = face_index++;
  initial->vbeg = create_vertex(initial, p[0]);

  cur = initial;
  int i;
  for (i = 1; i < (int)p.size(); i++)
  {
    iEdge *next = new iEdge();
    cur->next = next;
    cur->face = out_face;

    next->vbeg = create_vertex(cur, p[i]);
    cur->vend = next->vbeg;
    next->prev = cur;
    cur = next;
  }

  cur->next = initial;
  cur->face = out_face;

  cur->vend = initial->vbeg;
  initial->prev = cur;
  cur = initial;

  iFace *inner = new iFace();
  inner->index = face_index++;

  for (i = 0; i < (int)p.size(); i++)
  {
    iEdge *icur = new iEdge();

    cur->twin = icur;
    icur->twin = cur;
    icur->vbeg = cur->vend;
    icur->vend = cur->vbeg;

    if (i != 0)
    {
      icur->prev = cur->next->twin;
      cur->next->twin->next = icur;
    }

    icur->face = inner;

    cur = cur->prev;
  }

  initial->twin->prev = initial->next->twin;
  initial->next->twin->next = initial->twin;
  inner->rep = initial->twin;
}

DCEL::iVertex* DCEL::create_vertex(iEdge *edge, Point p)
{
  iVertex *v = new iVertex();
  v->tail = edge;
  v->coor = p;
  return v;
}

DCEL::iFace* DCEL::create_face_from(iFace *face)
{
  iFace *nf = new iFace();
  nf->colors = face->colors;
  return nf;
}

void DCEL::cut_singular(Segment s)
{
  iFace* initial = cut_segment(s);
  visited.clear();
  fill_single_visible(initial, s);
  next_color++;
}

void DCEL::cut_double(Segment s1, Segment s2)
{
  iFace* initial = cut_segment(s1);
  cut_segment(s2);
  visited.clear();
  fill_double_visible(initial, s1, s2);
  next_color++;
}

void DCEL::fill_single_visible(iFace *face, Segment s)
{
  if (visited.find(face->index) != visited.end())
    return;
  visited.insert(face->index);

  iEdge *cur = face->rep->next;
  double x = 0, y = 0;
  int nm = 0;
  while (1)
  {
    fill_single_visible(cur->twin->face, s);

    x += cur->seg().p0.X;
    y += cur->seg().p0.Y;
    nm++;

    if (cur == face->rep)
      break;

    cur = cur->next;
  }

  x /= nm;
  y /= nm;


  if (GeoUtils::ccw(s.p0, s.p1, Point(x, y)))
    face->colors.push_back(next_color);
}

void DCEL::fill_double_visible(iFace *face, Segment s1, Segment s2)
{
  if (visited.find(face->index) != visited.end())
    return;
  visited.insert(face->index);

  iEdge *cur = face->rep->next;
  double x = 0, y = 0;
  int nm = 0;
  while (1)
  {
    fill_double_visible(cur->twin->face, s1, s2);

    x += cur->seg().p0.X;
    y += cur->seg().p0.Y;
    nm++;

    if (cur == face->rep)
      break;

    cur = cur->next;
  }

  x /= nm;
  y /= nm;


  if (GeoUtils::ccw(s1.p0, s1.p1, Point(x, y)) && !GeoUtils::ccw(s2.p0, s2.p1, Point(x, y)))
    face->colors.push_back(next_color);
}

DCEL::iFace* DCEL::cut_segment(Segment s)
{
  Debug::print("---- CUT ----\n");

  // Find leading edge intersection
  iEdge *cur = out_face->rep->next;
  Point a, b, first_point;
  iEdge *aE, *bE, *first_edge;
  int found = 0;

  while (1)
  {
    if (GeoUtils::intersects(cur->seg(), s))
    {
      Point inter = GeoUtils::seg_seg_intersect(cur->seg(), s);
      if (found)
      {
        b = inter;
        bE = cur;
      }
      else
      {
        a = inter;
        aE = cur;
        found = 1;
      }
    }

    if (cur == out_face->rep)
      break;
    cur = cur->next;
  }

  if (GeoUtils::dist2(a, s.p0) <= GeoUtils::dist2(b, s.p0))
  {
    first_edge = aE;
    first_point = a;
  }
  else
  {
    first_edge = bE;
    first_point = b;
  }

  Debug::print("SP: ");
  GeoUtils::print_point(first_point);

  // Cut edge
  split_edge(first_edge, first_point);

  // Cut faces
  cut_face(first_edge->prev->twin, s);

  return first_edge->prev->twin->face;
}

void DCEL::split_edge(iEdge *edge, Point p)
{
  iEdge *new_edge = new iEdge();
  new_edge->prev = edge->prev;
  new_edge->next = edge;
  new_edge->vbeg = edge->vbeg;
  edge->prev->next = new_edge;
  edge->vbeg->tail = new_edge;
  new_edge->face = edge->face;
  edge->prev = new_edge;
  edge->vbeg = create_vertex(edge, p);
  new_edge->vend = edge->vbeg;

  iEdge *twin = edge->twin;
  iEdge *new_twin = new iEdge();
  new_twin->prev = twin;
  new_twin->next = twin->next;
  new_twin->vbeg = edge->vbeg;
  new_twin->face = twin->face;
  twin->next->prev = new_twin;
  twin->next = new_twin;
  new_twin->vend = twin->vend;
  twin->vend = edge->vbeg;

  new_twin->twin = new_edge;
  new_edge->twin = new_twin;
}

void DCEL::cut_face(iEdge *cur, Segment s)
{
  if (cur->face == out_face)
    return;

  iEdge *initial = cur;
  cur = cur->next;

  while (!GeoUtils::intersects(cur->seg(), s))
    cur = cur->next;

  Point inter = GeoUtils::seg_seg_intersect(cur->seg(), s);

  Debug::print("SP: ");
  GeoUtils::print_point(inter);
  split_edge(cur, inter);

  iEdge *a = initial;
  iEdge *b = cur;
  iEdge *na = new iEdge();
  iEdge *nb = new iEdge();

  na->twin = nb;
  nb->twin = na;

  na->next = a;
  na->prev = b->prev;
  nb->next = b;
  nb->prev = a->prev;

  a->prev->next = nb;
  b->prev->next = na;
  a->prev = na;
  b->prev = nb;

  na->vbeg = b->vbeg;
  na->vend = a->vbeg;
  nb->vbeg = a->vbeg;
  nb->vend = b->vbeg;

  if (face_index == 4)
  {
    GeoUtils::print_segment(na->seg());
    GeoUtils::print_segment(nb->seg());
    GeoUtils::print_segment(a->seg());
    GeoUtils::print_segment(b->seg());
    GeoUtils::print_segment(a->next->seg());
    GeoUtils::print_segment(a->next->next->seg());
  }

  iFace *new_face = create_face_from(a->face);
  new_face->index = face_index++;
  na->face = a->face;
  nb->face = new_face;
  a->face->rep = na;
  new_face->rep = nb;

  cur = nb->next;
  while (cur != nb)
  {
    cur->face = new_face;
    cur = cur->next;
  }

  cut_face(b->twin->next, s);
}

void DCEL::print()
{
  iEdge *cur = out_face->rep;
//  GeoUtils::print_point(cur->vbeg->coor);
  GeoUtils::print_segment(cur->seg());

  cur = cur->next;

  while (cur != out_face->rep)
  {
//    GeoUtils::print_point(cur->vbeg->coor);
    GeoUtils::print_segment(cur->seg());
    cur = cur->next;
  }
}

void DCEL::print_image(char* path)
{
  pngwriter png(window_x, window_y, 1.0, path);

  visited.clear();
  min_x = max_x = out_face->rep->seg().p0.X;
  min_y = max_y = out_face->rep->seg().p0.Y;
  find_bounds(out_face);
  visited.clear();
  print_faces(png, out_face, min((window_x - 2 * pad) / (max_x - min_x), (window_y - 2 * pad) / (max_y - min_y)));
     
  png.close();
}

void DCEL::print_faces(pngwriter &png, iFace *face, double scale)
{
  if (visited.find(face->index) != visited.end())
    return;
  visited.insert(face->index);
  Debug::print("I: %d\n", face->index);

  iEdge *cur = face->rep->next;
  double x = 0, y = 0;
  int nm = 0;
  while (1)
  {
    if (face != out_face)
    {
      GeoUtils::draw_segment(png, cur->seg(), scale, pad, pad);
      GeoUtils::print_segment(cur->seg());
    }
    x += cur->seg().p0.X * scale + pad;
    y += cur->seg().p0.Y * scale + pad;
    nm++;

    if (cur == face->rep)
      break;

    cur = cur->next;
  }

  cur = face->rep->next;
  while (1)
  {
    print_faces(png, cur->twin->face, scale);

    if (cur == face->rep)
      break;

    cur = cur->next;
  }

  x /= nm;
  y /= nm;

  char buffer [50];
  sprintf (buffer, "%d", face->index);
  if (face != out_face)
  {
    //png.plot_text("fonts/font.ttf", 12, x - 6, y - 6, 0, buffer, 0, 0, 0);
    int cx = x - (10 * (int)face->colors.size() + 5 * ((int)face->colors.size() - 1)) / 2;
    for (int i = 0; i < (int)face->colors.size(); i++)
    {
      png.filledsquare(cx, y - 5, cx + 10, y + 5,
                       color_list[face->colors[i]].R,
                       color_list[face->colors[i]].G,
                       color_list[face->colors[i]].B);
      cx += 15;
    }
  }
}


void DCEL::find_bounds(iFace *face)
{
  if (visited.find(face->index) != visited.end())
    return;
  visited.insert(face->index);

  iEdge *cur = face->rep->next;
  while (1)
  {
    max_x = max(max_x, cur->seg().p0.X);
    min_x = min(min_x, cur->seg().p0.X);
    max_y = max(max_y, cur->seg().p0.Y);
    min_y = min(min_y, cur->seg().p0.Y);

    find_bounds(cur->twin->face);

    if (cur == face->rep)
      break;

    cur = cur->next;
  }
}
