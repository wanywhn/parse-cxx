#ifndef INCLUDE_ACL_PCROLE_H_
#define INCLUDE_ACL_PCROLE_H_

#include <string>
#include "Object/ParseObject.h"
#include "Utils/ParsePlatformMacros.h"

NS_PC_BEGIN

class ParseQuery;

class ParseRole : public ParseObject {
private:
  ParseRole();

public:
  /**
   * \~english The name for the role. \n
   * This value is required, and can only be set once as a role is being created. \n
   * The name must consist of alphanumeric characters, spaces, -, or _. \n
   * This name will be used to identify the Role without needing its objectId.
   *
   */
  std::string name;

public:
  /**
   * \~english Constructs a new Role with the given name. If no default ACL has been
   * specified, you must set an ACL for the role.
   *
   * \~english \param name The name of the Role to create.
   *
   * \~english \return pointer to Role instance
   */
  static ParseRole* roleWithName(std::string name);

  /**
   * \~english Constructs a new Role with the given name and ACL.
   *
   * \~english \param name The name of the Role to create.
   * \~english \param acl The ACL for this role. Roles must have an ACL.
   *
   * \~english \return pointer to Role instance
   */
  static ParseRole* roleWithNameAndACL(std::string name, ParseACL* acl);

  /**
   * \~english Release memory.
   *
   */
  void release();

  /**
   * \~english Gets the Relation for the Users that are direct children of this role.
   * These users are granted any privileges that this role has been granted
   * (e.g. read or write access through ACLs). You can add or remove users from
   * the role through this relation.
   *
   * \~english \return the relation for the users belonging to this role.
   */
  ParseRelation* getUsers();

  /**
   * \~english Gets the Relation for the Roles that are direct children of this role.
   * These roles' users are granted any privileges that this role has been granted
   * (e.g. read or write access through ACLs). You can add or remove child roles
   * from this role through this relation.
   *
   * \~english \return the relation for the roles belonging to this role.
   */
   //TODO to complete
  ParseRelation* getRoles();

  /**
   * \~english Querying for Roles
   *
   * \~english \return pointer to Query
   */
  static ParseQuery* getQuery();
};

NS_PC_END

#endif  // INCLUDE_ACL_PCROLE_H_
