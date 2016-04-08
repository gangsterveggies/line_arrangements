#include "Common.h"
#include "DCEL.h"
#include "GeoUtils.h"
#include "Debug.h"

int main()
{
  Debug::disabled = 1;

  int i;
  int n;
  scanf("%d", &n);

  Polygon p = GeoUtils::read_polygon(n);
  DCEL *d = new DCEL();
  d->add_polygon(p);
  d->print();
  Debug::print("----------\n");

  int q, sz;
  scanf("%d", &q);

  for (i = 0; i < q; i++)
  {
    scanf("%d", &sz);

    if (sz == 1)
    {
      Segment s = GeoUtils::read_segment();
      d->cut_singular(s);
    }
    else
    {
      Segment s1 = GeoUtils::read_segment();
      Segment s2 = GeoUtils::read_segment();
      d->cut_double(s1, s2);
      //d->cut_singular(s1);
      //d->cut_singular(s2);
    }
  }

//  d->print();
  d->print_image("test.png");

  return 0;
}
