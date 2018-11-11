#include <string>

#include "ParseCloud.h"
#include "Request/ParsePaasClient.h"
#include "File/UploaderManager.h"

NS_PC_BEGIN

//TODO init url
    void ParseCloud::setApplicationConfig(std::string url, std::string applicationId, std::string clientKey) {
    ParsePaasClient::sharedInstance()->baseURL=url;
  ParsePaasClient::sharedInstance()->applicationId = applicationId;
  ParsePaasClient::sharedInstance()->clientKey = clientKey;
}

std::string ParseCloud::getApplicationId() {
  return ParsePaasClient::sharedInstance()->applicationId;
}

std::string ParseCloud::getClientKey() {
  return ParsePaasClient::sharedInstance()->clientKey;
}

NS_PC_END
