#ifndef INCLUDE_ACL_PCACL_H_
#define INCLUDE_ACL_PCACL_H_

#include <string>
#include <ParseConstants.h>
#include "Utils/ParsePlatformMacros.h"

NS_PC_BEGIN

class ParseUser;
class ParseRole;

class ParseACL {
private:
  ParseACL();
  Json dictionaryToReadAndWrite(bool read, bool write);
  Json dictionaryForKeyIfCreate(std::string key, bool create);
  void setAllowReadForKey(bool allowed, std::string key);
  bool isReadAllowedForKey(std::string key);
  void setAllowWriteForyKey(bool allowed, std::string key);
  bool isWriteAllowedForyKey(std::string key);
  std::string roleName(std::string name);

public:
  Json permissionById;

public:
  /**
   * \~english \brief Create an ACL with no permissions granted.
   *
   * \~english \return pointer to ACL instance
   *
   * \~chinese \brief 创建一个没有任何权限的ACL
   * \~chinese \return 指向ACL实例的指针
   */
  static ParseACL* ACL();

  /**
   * \~english \brief Create an ACL where only the specific user has access.
   *
   * \~english \param user the specific User
   *
   * \~english \return pointer to ACL instance
   */
  static ParseACL* ACLForUser(ParseUser *user);

  /**
   * \~english \brief release memory.
   *
   * \~chinese \brief 释放内存
   */
  void release();

  /**
   * \~english \brief Set whether the public is allowed to read this object.
   *
   * \~english \param allowed allowed or not
   */
  void setPublicReadAccess(bool allowed);

  /**
   * \~english \brief Gets whether the public is allowed to read this object.
   *
   * \~english \return true if allowed else false
   */
  bool getPublicReadAccess();

  /**
   * \~english \briefSet whether the public is allowed to write this object.
   *
   * \~english \param allowed allowed or not
   */
  void setPublicWriteAccess(bool allowed);

  /**
   * \~english \brief Gets whether the public is allowed to write this object.
   *
   * \~english \return true if allowed else false
   */
  bool getPublicWriteAccess();

  /**
   *\~english  Set whether the given user id is allowed to read this object.
   *
   * \~english \param allowed allowed or not
   * \~english \param userId the AVUser's objectId
   */
  void setReadAccess(bool allowed, std::string userId);

  /**
   * \~english Gets whether the given user id is *explicitly* allowed to read this object.
   * Even if this returns false, the user may still be able to access it if getPublicReadAccess returns true
   * or if the user belongs to a role that has access.
   *
   * \~english \param userId the AVUser's objectId
   *
   * \~english \return true if allowed else false
   */
  bool getReadAccess(std::string userId);

  /**
   * \~english Set whether the given user id is allowed to write this object.
   *
   * \~english \param allowed allowed or not
   * \~english \param userId the AVUser's objectId
   */
  void setWriteAccess(bool allowed, std::string userId);

  /**
   * \~english Gets whether the given user id is *explicitly* allowed to write this object.
   * Even if this returns false, the user may still be able to write it if getPublicWriteAccess returns true
   * or if the user belongs to a role that has access.
   *
   * \~english \param userId the AVUser's objectId
   *
   * \~english \return true if allowed else false
   */
  bool getWriteAccess(std::string userId);

  /**
   * \~english Set whether the given user is allowed to read this object.
   *
   * \~english \warning if the user doesn't have valid objectId, no operation is performed.
   * \~english \param allowed allowed or not
   * \~english \param user the AVUser
   */
  void setReadAccess(bool allowed, ParseUser *user);

  /**
   * \~english Gets whether the given user is *explicitly* allowed to read this object.
   * Even if this returns false, the user may still be able to access it if getPublicReadAccess returns true
   * or if the user belongs to a role that has access.
   *
   * \~english \param user the AVUser
   *
   * \~english \return true if allowed else false. if the user doesn't have valid objectId, return true.
   */
  bool getReadAccess(ParseUser *user);

  /**
   * \~english Set whether the given user is allowed to write this object.
   *
   * \~english \warning if the user doesn't have valid objectId, no operation is performed.
   * \~english \param allowed allowed or not
   * \~english \param user the AVUser
   */
  void setWriteAccess(bool allowed, ParseUser *user);

  /**
   * \~english Gets whether the given user is *explicitly* allowed to write this object.
   * Even if this returns false, the user may still be able to write it if getPublicWriteAccess returns true
   * or if the user belongs to a role that has access.
   *
   * \~english \param user the AVUser
   *
   * \~english \return true if allowed else false. if the user doesn't have valid objectId, return true.
   */
  bool getWriteAccess(ParseUser *user);

  /**
   * \~english Set whether users belonging to the role with the given name are allowed
   * to read this object.
   *
   * \~english \param allowed The name of the role.
   * \~english \param name Whether the given role can read this object.
   */
  void setReadAccessForRole(bool allowed, std::string name);

  /**
   * \~english Get whether users belonging to the role with the given name are allowed
   * to read this object. Even if this returns false, the role may still
   * be able to read it if a parent role has read access.
   *
   * \~english \param name The name of the role.
   *
   * \~english \return if allowed else false
   */
  bool getReadAccessForRole(std::string name);

  /**
   * \~english Set whether users belonging to the role with the given name are allowed
   * to write this object.
   *
   * \~english \param allowed The name of the role.
   * \~english \param name Whether the given role can write this object.
   */
  void setWriteAccessForRole(bool allowed, std::string name);

  /**
   * \~english Get whether users belonging to the role with the given name are allowed
   * to write this object. Even if this returns false, the role may still
   * be able to write it if a parent role has write access.
   *
   * \~english \param name The name of the role.
   *
   * \~english \return if allowed else false
   */
  bool getWriteAccessForRole(std::string name);

  /**
   * \~english Set whether users belonging to the given role are allowed to read this
   * object. The role must already be saved on the server and its data must have
   * been fetched in order to use this method.
   *
   * \~english \param allowed Whether the given role can read this object.
   * \~english \param role The role to assign access.
   */
  void setReadAccessForRole(bool allowed, ParseRole* role);

  /**
   * \~english Get whether users belonging to the given role are allowed to read this
   * object. Even if this returns NO, the role may still be able to
   * read it if a parent role has read access. The role must already be saved on
   * the server and its data must have been fetched in order to use this method.
   *
   * \~english \param role the given role
   *
   * \~english \return true if the role has read access. false otherwise.
   */
  bool getReadAccessForRole(ParseRole* role);

  /**
   * \~english Set whether users belonging to the given role are allowed to write this
   * object. The role must already be saved on the server and its data must have
   * been fetched in order to use this method.
   *
   * \~english \param allowed Whether the given role can write this object.
   * \~english \param role The role to assign access.
   */
  void setWriteAccessForRole(bool allowed, ParseRole* role);

  /**
   * \~english Get whether users belonging to the given role are allowed to write this
   * object. Even if this returns NO, the role may still be able to
   * write it if a parent role has write access. The role must already be saved on
   * the server and its data must have been fetched in order to use this method.
   *
   * \~english \param role the given role
   *
   * \~english \return true if the role has write access. false otherwise.
   */
  bool getWriteAccessForRole(ParseRole* role);

  /**
   * \~english Sets a default ACL that will be applied to all Objects when they are created.
   *
   * \~english \param acl The ACL to use as a template for all Objects created after setDefaultACL has been called.
   * This value will be copied and used as a template for the creation of new ACLs, so changes to the
   * instance after setDefaultACL has been called will not be reflected in new AVObjects.
   * \~english \param currentUserAccess If true, the AVACL that is applied to newly-created AVObjects will
   * provide read and write access to the currentUser at the time of creation. If false,
   * the provided ACL will be used without modification. If acl is nil, this value is ignored.
   */
  static void setDefaultACLWithCurrentUser(ParseACL* acl, bool currentUserAccess);
};

NS_PC_END

#endif  // INCLUDE_ACL_PCACL_H_
