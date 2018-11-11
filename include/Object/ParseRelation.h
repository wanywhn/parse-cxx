
#ifndef INCLUDE_PARSEOBJECT_RELATION_H_
#define INCLUDE_PARSEOBJECT_RELATION_H_

#include <string>

#include "Object/ParseObject.h"
#include "Query/ParseQuery.h"
#include "Utils/ParsePlatformMacros.h"

NS_PC_BEGIN

/**
 * \~english \brief A class that is used to access all of the children of a many-to-many relationship.  Each instance
 * of AVRelation is associated with a particular parent object and key.
 */
class ParseRelation {
public:
  /**
   * \~english parent object of this Relation.
   *
   */
  ParseObject* parent;

  /**
   * \~english key of this AVRelation.
   *
   */
  std::string key;

  /**
   * \~english className of target class. if a person `like` a book, the relation key is `like`,
   * while the targetClass is the className of book.
   *
   */
  std::string targetClass;

public:
  /**
   * \~english Query for an Relation.
   *
   * \~english \return A AVQuery that can be used to get objects in this relation.
   */
  ParseQuery* query();

  /**
   * \~english release memory.
   *
   */
  void release();

  /**
   * \~english Adds a relation to the passed in object.
   *
   * \~english \param object AVObject to add relation to.
   */
  void addObject(ParseObject* object);

  /**
   * \~english Removes a relation to the passed in object.
   *
   * \~english \param object AVObject to add relation to.
   */
  void removeObject(ParseObject* object);

  /**
   * \~english A Query that can be used to get parent objects in this relation.
   *
   * \~english \param parentClassName parent Class Name
   * \~english \param relationKey relation Key
   * \~english \param child child object
   *
   * \~english \return the Query
   */
  static ParseQuery* reverseQueryWithRelationKeyAndChildObject(std::string parentClassName,
                                                            std::string relationKey,
                                                            ParseObject* child);
};

NS_PC_END

#endif  // INCLUDE_PARSEOBJECT_RELATION_H_
