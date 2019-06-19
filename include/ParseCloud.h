#ifndef INCLUDE_PARSECLOUD_H_
#define INCLUDE_PARSECLOUD_H_

#include <iostream>
#include <string>
#include "ParseConstants.h"
#include "Utils/ParsePlatformMacros.h"

NS_PC_BEGIN

class ParseCloud {
public:
  /**
   * \~english Sets the URL,applicationId and clientKey of your application.
   *
   * \~english \param applicationId The applicaiton id for your Parse Cloud application.
   * \~english \param clientKey The client key for your Parse Cloud application.
   */
  static void setApplicationConfig(const std::string& url, std::string applicationId, std::string clientKey);

  /**
   * \~english  get Application Id
   *
   * \~english  \return Application Id
   */
  static std::string getApplicationId();

  /**
   *  \~english get Client Key
   *
   *  \~english \return Client Key
   */
  static std::string getClientKey();
};

NS_PC_END

#endif  // INCLUDE_PARSECLOUD_H_
