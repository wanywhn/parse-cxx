#include <utility>

#include <utility>

#include <string>

#include "ParseCloud.h"
#include "Request/ParsePaasClient.h"
#include "File/UploaderManager.h"
NS_PC_BEGIN

//TODO init url
    void ParseCloud::setApplicationConfig(const std::string& url, std::string applicationId, std::string clientKey) {
    ParsePaasClient::sharedInstance()->setBaseUrl(url);
  ParsePaasClient::sharedInstance()->applicationId = std::move(applicationId);
  ParsePaasClient::sharedInstance()->clientKey = std::move(clientKey);
}

std::string ParseCloud::getApplicationId() {
  return ParsePaasClient::sharedInstance()->applicationId;
}

std::string ParseCloud::getClientKey() {
  return ParsePaasClient::sharedInstance()->clientKey;
}

NS_PC_END
