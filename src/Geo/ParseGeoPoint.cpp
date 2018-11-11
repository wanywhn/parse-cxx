#include "Geo/ParseGeoPoint.h"

#include <cmath>
#include <ParseConstants.h>
#include <Geo/ParseGeoPoint.h>


NS_PC_BEGIN

const double pi = 3.14159265358;
const double R=6371.393; //KM

ParseGeoPoint::ParseGeoPoint():latitude(0),
                         longitude(0) {
}


ParseGeoPoint* ParseGeoPoint::geoPoint(double latitude, double longitude) {
  ParseGeoPoint* point = new ParseGeoPoint(latitude,longitude);
  point->latitude = latitude;
  point->longitude = longitude;

  return point;
}

void ParseGeoPoint::release() {
  PC_SAFE_DELETE(this);
}

double ParseGeoPoint::distanceInRadiansTo(ParseGeoPoint* point) {
  return this->distanceInKillometersTo(point) / 6378.140;
}

double ParseGeoPoint::distanceInMilesTo(ParseGeoPoint* point) {
  return this->distanceInKillometersTo(point) / 1.609344;
}

double ParseGeoPoint::distanceInKillometersTo(ParseGeoPoint* point) {
  double theta = this->longitude - point->longitude;
  double dist = sin(degreeToRadian(this->latitude)) * sin(degreeToRadian(point->latitude))
                + cos(degreeToRadian(this->latitude)) * cos(degreeToRadian(point->latitude))
                * cos(degreeToRadian(theta));

//  dist = dist * 60 * 1.1515 * 1.609344;
    dist*=R;

  return dist;
}


ParseGeoPoint* ParseGeoPoint::geoPointFromJson(Json const &dict) {
  return ParseGeoPoint::geoPoint(dict["latitude"],
                                 dict["longitude"]);
}

///////////////////////////////private methods/////////////////////////////
double ParseGeoPoint::degreeToRadian(double degree) {
  return degree * pi / 180.0;
}

double ParseGeoPoint::radianToDegree(double radian) {
  return radian * 180.0 / pi;
}

    ParseGeoPoint::ParseGeoPoint(double latitude, double longitude):latitude(latitude),longitude(longitude) {

    }

    Json ParseGeoPoint::toJson() {
      Json dict;
      dict["__type"] = "GeoPoint";
      dict["latitude"] =this->latitude;
      dict["longitude"] =this->longitude;

      return dict;
    }

NS_PC_END
