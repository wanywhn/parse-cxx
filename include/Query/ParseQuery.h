
#ifndef INCLUDE_QUERY_QUERY_H_
#define INCLUDE_QUERY_QUERY_H_

#include <string>
#include <vector>
#include <unordered_set>
#include "ParseConstants.h"
#include "Object/ParseObject.h"
#include "Geo/ParseGeoPoint.h"
#include "Utils/ParsePlatformMacros.h"

NS_PC_BEGIN
class ParseQuery {
private:
  ParseQuery();
  void addWhereItemForKey(Json const & dict, std::string key);
  void assembleParameters();

private:
  Json parameters;
  Json where;
  std::unordered_set<std::string> selectedKeys;
  std::unordered_set<std::string> include;
  std::string order;

public:
  /**
   * \~english The class name to query for
   *
   */
  std::string className;

  /**
   * \~english A limit on the number of objects to return.  Note: If you are calling findObject with limit=1, you may find it easier to use getFirst instead.
   *
   */
  int limit;

  /**
   * \~english The number of objects to skip before returning any.
   *
   */
  int skip;

public:
  /**
   * \~english Initializes the query with a class name.
   *
   * \~english \param newClassName The class name.
   *
   * \~english \return
   */
  static ParseQuery* queryForClassName(std::string className);

  /**
   * \~english Returns a Query that is the or of the passed in Querys.
   *
   * \~english \param queries The list of queries to or together.
   *
   * \~english \return a Query that is the or of the passed in Querys.
   */
  static ParseQuery* orQueryWithSubqueries(std::vector<ParseQuery*> queries);

  /**
   * \~english Returns a Query that is the AND of the passed in Querys.
   *
   * \~english \param queries The l¡ist of queries to AND together.
   *
   * \~english \return a Query that is the AND of the passed in Querys.
   */
  static ParseQuery* andQueryWithSubqueries(std::vector<ParseQuery*> queries);

  /**
   * \~english release memory.
   *
   */
  void release();

  /**
   * \~english Make the query include Objects that have a reference stored at the provided key.
   * This has an effect similar to a join.  You can use dot notation to specify which fields in
   * the included object are also fetch.
   *
   * \~english \param key The key to load child Objects for.
   */
  void includeKey(std::string key);

  /**
   * \~english Make the query restrict the fields of the returned Objects to include only the provided keys.
   * If this is called multiple times, then all of the keys specified in each of the calls will be included.
   *
   * \~english \param keys The keys to include in the result.
   */
  void selectKeys(std::vector<std::string> keys);

  /**
   * \~english Add a constraint that requires a particular key exists.
   *
   * \~english \param key The key that should exist.
   */
  void whereKeyExists(std::string key);

  /**
   * \~english Add a constraint that requires a key not exist.
   *
   * \~english \param key The key that should not exist.
   */
  void whereKeyDoesNotExist(std::string key);

  /**
   * \~english Add a constraint to the query that requires a particular key's object to be equal to the provided object.
   *
   * \~english \param key The key to be constrained.
   * \~english \param object The object that must be equalled.
   */
  void whereKeyEqualTo(std::string key, Json const& object);

  /**
   * \~english Add a constraint to the query that requires a particular key's object to be equal to the provided object.
   *
   * \~english \param key The key to be constrained.
   * \~english \param object a pointer to Object instance.
   */
  void whereKeyEqualTo(std::string key, ParseObject* object);

  /**
   * \~english Add a constraint to the query that requires a particular key's object to be less than the provided object.
   *
   * \~english \param key The key to be constrained.
   * \~english \param object The object that provides an upper bound.
   */
  void whereKeyLessThan(std::string key, Json const & object);

  /**
   * \~english Add a constraint to the query that requires a particular key's object to be less than or equal to the provided object.
   *
   * \~english \param key The key to be constrained.
   * \~english \param object The object that must be equalled.
   */
  void whereKeyLessThanOrEqualTo(std::string key, Json const & object);

  /**
   * \~english Add a constraint to the query that requires a particular key's object to be greater than the provided object.
   *
   * \~english \param key The key to be constrained.
   * \~english \param object The object that must be equalled.
   */
  void whereKeyGreaterThan(std::string key, Json const & object);

  /**
   * \~english Add a constraint to the query that requires a particular key's object to be greater than or equal to the provided object.
   *
   * \~english \param key The key to be constrained.
   * \~english \param object The object that must be equalled.
   */
  void whereKeyGreaterThanOrEqualTo(std::string key,
                                    Json const & object);

  /**
   * \~english Add a constraint to the query that requires a particular key's object to be not equal to the provided object.
   *
   * \~english \param key The key to be constrained.
   * \~english \param object The object that must not be equalled.
   */
  void whereKeyNotEqualTo(std::string key, Json const & object);

  /**
   * \~english Add a constraint to the query that requires a particular key's object to be contained in the provided array.
   *
   * \~english \param key The key to be constrained.
   * \~english \param array The possible values for the key's object.
   */
  void whereKeyContainedIn(std::string key,
                           std::vector<Json> const & array);

  /**
   * \~english Add a constraint to the query that requires a particular key's object not be contained in the provided array.
   *
   * \~english \param key The key to be constrained.
   * \~english \param array The list of values the key's object should not be.
   */
  void whereKeyNotContainedIn(std::string key,
                              std::vector<Json> const & array);

  /**
   * \~english Add a constraint to the query that requires a particular key's array contains every element of the provided array.
   *
   * \~english \param key The key to be constrained.
   * \~english \param array The array of values to search for.
   */
  void whereKeyContainsAllObjectsInArray(std::string key,
                                         std::vector<Json> const & array);

  /**
   * \~english Add a constraint to the query that requires a particular key's coordinates (specified via GeoPoint) be near
   * a reference point.  Distance is calculated based on angular distance on a sphere.  Results will be sorted by distance
   * from reference point.
   *
   * \~english \param key The key to be constrained.
   * \~english \param geoPoint The reference point.  A GeoPoint.
   */
  void whereKeyNearGeoPoint(std::string key, ParseGeoPoint* geoPoint);

  /**
   * \~english Add a constraint to the query that requires a particular key's coordinates (specified via GeoPoint) be near
   * a reference point and within the maximum distance specified (in miles).  Distance is calculated based on
   * a spherical coordinate system.  Results will be sorted by distance (nearest to farthest) from the reference point.
   *
   * \~english \param key The key to be constrained.
   * \~english \param geoPoint The reference point.  A GeoPoint.
   * \~english \param maxDistance Maximum distance in miles.
   */
  void whereKeyNearGeoPointWithinMiles(std::string key,
                                       ParseGeoPoint* geoPoint,
                                       double maxDistance);

  /**
   * \~english Add a constraint to the query that requires a particular key's coordinates (specified via GeoPoint) be near
   * a reference point and within the maximum distance specified (in kilometers).  Distance is calculated based on
   * a spherical coordinate system.  Results will be sorted by distance (nearest to farthest) from the reference point.
   *
   * \~english \param key The key to be constrained.
   * \~english \param geoPoint The reference point.  A GeoPoint.
   * \~english \param maxDistance Maximum distance in kilometers.
   */
  void whereKeyNearGeoPointWithinKilometers(std::string key,
                                            ParseGeoPoint* geoPoint,
                                            double maxDistance);

  /**
   * \~english Add a constraint to the query that requires a particular key's coordinates (specified via GeoPoint) be near
   * a reference point and within the maximum distance specified (in radians).  Distance is calculated based on
   * angular distance on a sphere.  Results will be sorted by distance (nearest to farthest) from the reference point.
   *
   * \~english \param key The key to be constrained.
   * \~english \param geoPoint The reference point.  A GeoPoint.
   * \~english \param maxDistance Maximum distance in radians.
   */
  void whereKeyNearGeoPointWithinRadians(std::string key,
                                         ParseGeoPoint* geoPoint,
                                         double maxDistance);

  /**
   * \~english Add a regular expression constraint for finding string values that match the provided regular expression.
   * This may be slow for large datasets.
   *
   * \~english \param key The key that the string to match is stored in.
   * \~english \param regex The regular expression pattern to match.
   */
  void whereKeyMatchesRegex(std::string key, std::string regex);

  /**
   * \~english Add a constraint for finding string values that contain a provided substring.
   * This will be slow for large datasets.
   *
   * \~english \param key The key that the string to match is stored in.
   * \~english \param substring The substring that the value must contain.
   */
  void whereKeyContainsString(std::string key, std::string substring);

  /**
   * \~english Add a constraint for finding string values that start with a provided prefix.
   * This will use smart indexing, so it will be fast for large datasets.
   *
   * \~english \param key The key that the string to match is stored in.
   * \~english \param prefix The substring that the value must start with.
   */
  void whereKeyHasPrefix(std::string key, std::string prefix);

  /**
   * \~english Add a constraint for finding string values that end with a provided suffix.
   * This will be slow for large datasets.
   *
   * \~english \param key The key that the string to match is stored in.
   * \~english \param suffix The substring that the value must end with.
   */
  void whereKeyHasSuffix(std::string key, std::string suffix);

  /**
   * \~english Matches any array with the number of elements specified by count
   *
   * \~english \param key The key that the value is stored in, value should be kind of array
   * \~english \param count the array size
   */
  void whereKeySizeEqualTo(std::string key, int count);

  /**
   * \~english Sort the results in ascending order with the given key.
   *
   * \~english \param key The key to order by.
   */
  void orderByAscending(std::string key);

  /**
   * \~english Also sort in ascending order by the given key.  The previous keys provided will
   *
   * \~english \param key The key to order bye
   */
  void addAscendingOrder(std::string key);

  /**
   * \~english Sort the results in descending order with the given key.
   *
   * \~english \param key The key to order by.
   */
  void orderByDescending(std::string key);

  /**
   * \~english Also sort in descending order by the given key.  The previous keys provided will
   * precedence over this key.
   *
   * \~english \param key The key to order by.
   */
  void addDescendingOrder(std::string key);

  /**
   * \~english Returns a Object with the given id.
   * This mutates the Query.
   *
   * \~english \param objectId The id of the object that is being requested.
   *
   * \~english \return The Object if found. Returns nil if the object isn't found, or if there was an error.
   */
  ParseObject* getObjectWithId(std::string objectId);

  /**
   * \~english Finds objects based on the constructed query.
   *
   * \~english \return an array of Objects that were found.
   */
  std::vector<ParseObject*> findObjects();

  /**
   * \~english Counts objects based on the constructed query.
   *
   * \~english \return the number of Objects that match the query, or -1 if there is an error.
   */
  int countObjects();

  /**
   * \~english Counts objects asynchronously and calls the given block with the counts.
   *
   * \~english \param callback the callbck to execute. The callback should have the following argument sianature: (int const&, Error const&).
   */
  void countObjectsInBackgroundWithCallback(IntegerResultCallback callback);

  /**
   * \~english Set where of The Query
   *
   * \~english \param where
   */
  void setWhere(Json where);
};

NS_PC_END

#endif  // INCLUDE_QUERY_QUERY_H_
