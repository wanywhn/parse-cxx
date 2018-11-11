
#ifndef INCLUDE_USER_PARSEUSER_H_
#define INCLUDE_USER_PARSEUSER_H_

#include <string>
#include "Object/ParseObject.h"
#include "Query/ParseQuery.h"
#include "Utils/ParsePlatformMacros.h"
#include "ParseConstants.h"

NS_PC_BEGIN

class ParseUser : public ParseObject {
private:
  ParseUser();

public:
  /**
   * \~english The session token for the ParseUser. This is set by the server upon successful authentication.
   *
   */
  std::string sessionToken;

  /**
   * \~english The username for the ParseUser.
   *
   */
  std::string username;

  /**
   * \~english The password for the ParseUser. This will not be filled in from the server with
   * the password. It is only meant to be set.
   *
   */
  std::string password;

  /**
   * \~english The email for the ParseUser.
   *
   */
  std::string email;

public:
  /**
   * \~english Gets the currently logged in user from disk and returns an instance of it.
   *
   * \~english \return a ParseUser that is the currently logged in user. If there is none, returns nullptr.
   */
  static ParseUser* currentUser();

  /**
   * \~english Whether the user is an authenticated object for the device. An authenticated ParseUser is one that is obtained via
   * a signUp or logIn method. An authenticated object is required in order to save (with altered values) or delete it.
   *
   * \~english \return whether the user is authenticated.
   */
  bool isAuthenticated();

  /**
   * \~english Creates a new ParseUser object.
   *
   * \~english \return a new ParseUser object.
   */
  static ParseUser* user();

  /**
   * \~english Request to re-send email for verification.
   *
   * \~english \param email The address of email
   * \~english \param callback The callback function
   *
   */
  static void verificationEmailRequestWithCallback(std::string email,
                                                   BooleanResultCallback callback);

  /**
   * \~english release memory.
   *
   */
  void release();

  /**
   * \~english Signs up the user. Make sure that password and username are set. This will also enforce that the username isn't already taken.
   *
   */
  void signUp(BooleanResultCallback callback);

  /**
   * \~english update user's password with callback
   *
   * \~english \param oldPassword old password
   * \~english \param newPassword new password
   * \~english \param callback The callback to be executed
   */
  void updatePasswordWithCallback(std::string oldPassword,
                                  std::string newPassword,
                                  IdResultCallback callback);

  /**
   * \~english Makes a request to login a user with specified credentials. Returns an instance
   * of the successfully logged in ParseUser. This will also cache the user locally so
   * that calls to userFromCurrentUser will use the latest logged in user.
   *
   * \~english \param username The username of the user.
   * \~english \param password The password of the user.
   *
   * \~english \return an instance of the ParseUser on success. If login failed for either wrong password or wrong username, returns nullptr.
   */
  static ParseUser* loginWithUsernameAndPassword(std::string username,
                                              std::string password);

  /**
   * \~english Logs out the currently logged in user on disk.
   *
   */
  static void logOut();

  /**
   * \~english Send a password reset request for a specified email. If a user account exists with that email,
   * an email will be sent to that address with instructions on how to reset their password.
   *
   * \~english \param email
   */
  static void requestPasswordResetForEmail(std::string email, StringResultCallback callback);

  /**
   * \~english Creates a query for ParseUser objects.
   *
   * \~english \return An instance of ParseQuery
   */
  static ParseQuery* query();
};

NS_PC_END

#endif  // INCLUDE_USER_PARSEUSER_H_
