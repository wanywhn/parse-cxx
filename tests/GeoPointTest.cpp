#include <string>
#include <vector>
#include "Geo/ParseGeoPoint.h"
#include <catch/catch.hpp>

using namespace std;
using namespace parsecloud;

TEST_CASE("GeoPoint Distance Test","[GeoPoint]"){
  ParseGeoPoint* point1 = ParseGeoPoint::geoPoint(39.9139, 116.3917);
  ParseGeoPoint* point2 = ParseGeoPoint::geoPoint(-39.9139, 116.3917);

  CHECK(point1->distanceInKillometersTo(point2) >= 8000);

  point1->release();
  point2->release();
}
