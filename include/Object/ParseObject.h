
#ifndef INCLUDE_PARSEOBJECT_PARSEOBJECT_H_
#define INCLUDE_PARSEOBJECT_PARSEOBJECT_H_

#include <string>
#include <vector>
#include <unordered_map>
#include <pplx/pplxtasks.h>

#include "ParseConstants.h"
#include "Utils/ParsePlatformMacros.h"
#include "Geo/ParseGeoPoint.h"

NS_PC_BEGIN

class ParseRelation;
class ParseACL;

class ParseObject {
private:
  ParseACL* acl;

private:
  void addObjectIntoRelationDataForKey(std::unordered_map<std::string, std::vector<ParseObject*>> *relationData,
                                       ParseObject* object,
                                       std::string key);
  Json generateRelationObjectsByArray(std::vector<ParseObject*> objects,
                                             bool isAdded);

protected:
  /**
   * \~english constructor of Object.
   *
   * \~english \return
   */
  ParseObject();

  /**
   * \~english \brief get object path of the object.
   *
   * \~english \return
   */
  virtual std::string myObjectPath();


public:
  /**
   * \~english  The id of the object.
   *
   */
  std::string objectId;

  /**
   * \~english When the object was last updated.
   *
   */
  std::string updatedAt;

  /**
   * \~english When the object was created.
   *
   */
  std::string createdAt;

  /**
   * \~english The class name of the object.
   *
   */
  std::string className;

  /**
   * \~english store data of object, which is key-value compliant.
   *
   */
  Json localData;

  /**
   * \~english objects that will be added into Relation.
   *
   */
  std::unordered_map<std::string, std::vector<ParseObject*>> addedRelationData;

  /**
   * \~english objects will be removed from Relation.
   *
   */
  std::unordered_map<std::string, std::vector<ParseObject*>> removedRelationData;

public:
  /**
   * \~english Creates a new Object with a class name.
   *
   * \~english \param className A class name can be any alphanumeric string that begins with a letter. It represents an object in your app, like a User of a Document.
   *
   * \~english \return the object that is instantiated with the given class name.
   */
  static ParseObject* objectForClassName(std::string const &className);

  /**
   * \~english Creates a reference to an existing Object for use in creating associations between Objects.  Calling isDataAvailable on this
 object will return NO until fetchIfNeeded or refresh has been called.  No network request will be made.
   *
   * \~english \param className The object's class.
   * \~english \param objectId The object id for the referenced object.
   *
   * \~english \return A Object without data.
   */
  static ParseObject* objectForClassNameAndId(std::string const &className,
                                              std::string const &objectId);

    virtual /**
   * \~english Release memory.
   *
   */
  void release();

  /**
   * \~english Set the ACL for this object.
   *
   */
  void setACL(ParseACL* acl);

  /**
   * \~english Get the ACL for this object.
   *
   * \~english \return ACL of this object
   */
  ParseACL* getACL();

  /**
   * \~english  Returns an array of the keys contained in this object. This does not include
 createdAt, updatedAt, authData, or objectId. It does include things like username
 and ACL.
   *
   * \~english \return
   */
  std::vector<std::string> allKeys();

  /**
   * \~english Returns the object associated with a given key.
   *
   * \~english \param key The key that the object is associated with.
   *
   * \~english \return The value associated with the given key, or nil if no value is associated with key.
   */
  Json objectForKey(std::string const & key);

  /**
   * \~english Sets the object associated with a given key.
   *
   * \~english \param object The object.
   * \~english \param key The key.
   */
  void setObjectForKey(Json const & object, std::string const & key);

  /**
   * \~english Sets the object associated with a given key.
   *
   * \~english \param object an Object instance with objectId.
   * \~english \param key The key.
   */
  void setObjectForKey(ParseObject* const &  object, std::string const & key);

  /**
   * \~english Sets the object associated with a given key.
   *
   * \~english \param object an GeoPoint instance.
   * \~english \param key The key.
   */
  void setObjectForKey(ParseGeoPoint* const & geoPoint, std::string const & key);

  /**
   * \~english Sets the object associated with a given key.
   *
   * \~english \param key The key.
   */
  void removeObjectForKey(std::string const & key);

  /**
   * \~english Returns the relation object associated with the given key
   *
   * \~english \param key The key that the relation is associated with.
   *
   * \~english \return an Relation.
   */
  ParseRelation* relationForKey(std::string key);

  /**
   *\~english  Add object into relation data; The object must have valid objectId.
   *
   * \~english \param object The object to be added into relation data;
   * \~english \param key relation key.
   */
  void addRelationForKey(ParseObject* object, std::string key);

  /**
   * \~english Remove object from relation data; The object must have valid objectId.
   *
   * \~english \param object The object to be removed from relation data;
   * \~english \param key relation key.
   */
  void removeRelationForKey(ParseObject* object, std::string key);

  /**
   * \~english Saves the Object asynchronously.
   *
   */
  pplx::task<PCError> saveInBackground();

    /**
   *  Saves a collection of objects all at once asynchronously and calls a callback when done.
   *
   * \~english \param objects
   * \~english \param callback
   */
  static pplx::task<std::vector<PCError>> saveAllInBackgroundWithCallback(std::vector<ParseObject *> objects);

  /**
   * \~english  Fetches the Object with the current data from the server.
   *
   */
  pplx::task<PCError> fetch();

  /**
   *  \~english Fetches the Object with the current data and specified keys from the server and sets an error if it occurs.
   *
   * \~english \param keys
   */
  pplx::task<PCError> fetchWithKeys(std::vector<std::string> keys);

    /**
     * \~english delet the Object and execute the given callback.
     *
     * \~english \param callback
     */
  pplx::task<PCError> deleteInBackgroundWithCallback();

  /**
   * \~english check if the object has valid objectId
   *
   * \~english \return
   */
  bool hasValidObjectId();

    virtual ~ParseObject()= default;
};

NS_PC_END

#endif  // INCLUDE_PARSEOBJECT_PARSEOBJECT_H_
