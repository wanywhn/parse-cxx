#ifndef INCLUDE_GEO_GEOPOINT_H_
#define INCLUDE_GEO_GEOPOINT_H_

#include <ParseConstants.h>
#include "Utils/ParsePlatformMacros.h"

NS_PC_BEGIN
class ParseGeoPoint {
private:
  ParseGeoPoint();
  ParseGeoPoint(double latitude, double longitude);

    double degreeToRadian(double degree);
  double radianToDegree(double radian);

private:
  /**
   * \~english Latitude of point in degrees.  Valid range (-90.0, 90.0).
   *
   */
  double latitude;

  /**
   * \~english Longitude of point in degrees.  Valid range (-180.0, 180.0).
   *
   */
  double longitude;

public:
  /**
   * \~english Creates a new GeoPoint object with the specified latitude and longitude.
   * if no latitude neither longitude specified ,0 are default
   *
   * \~english \param latitude Latitude of point in degrees.
   * \~english \param longitude Longitude of point in degrees.
   *
   * \~english \return New point object with specified latitude and longitude.
   */
  static ParseGeoPoint* geoPoint(double latitude=0,
                                 double longitude=0);

  /**
   * \~english release memory.
   *
   */
  void release();

  /**
   * \~english Get distance in radians from this point to specified point.
   *
   * \~english \param point GeoPoint location of other point.
   *
   * \~english \return distance in radians
   */
  double distanceInRadiansTo(ParseGeoPoint* point);

  /**
   * \~english Get distance in miles from this point to specified point.
   *
   * \~english \param point GeoPoint location of other point.
   *
   * \~english \return distance in miles
   */
  double distanceInMilesTo(ParseGeoPoint* point);

  /**
   * \~english Get distance in kilometers from this point to specified point.
   *
   * \~english \param point GeoPoint location of other point.
   *
   * \~english \return distance in kilometers.
   */
  double distanceInKillometersTo(ParseGeoPoint* point);

  /**
   * \~english convert to a Json to be sent
   *
   * \~english \return
   */
  Json toJson();
  /**
   * \~english get an GeoPoint from dictionary
   *
   * \~english \param dict
   *
   * \~english \return
   */
  static ParseGeoPoint* geoPointFromJson(Json const &dict);
};

NS_PC_END

#endif  // INCLUDE_GEO_GEOPOINT_H_
