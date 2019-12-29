#ifndef INCLUDE_REQUEST_PARSEPAASCLIENT_H_
#define INCLUDE_REQUEST_PARSEPAASCLIENT_H_

#include <string>
#include <vector>
#include <unordered_map>
#include <unordered_set>
#include <mutex>

#include <cpprest/http_client.h>
#include "Utils/ParsePlatformMacros.h"
#include "Utils/ParseStringUtils.h"
#include "ParseConstants.h"
#include "Object/ParseObject.h"
#include "User/ParseUser.h"
#include "ACL/ParseACL.h"


using http_client=web::http::client::http_client;
using uri_builder=web::uri_builder;
using http_request=web::http::http_request;
using http_response=web::http::http_response;
NS_PC_BEGIN


class ParsePaasClient {
public:
  typedef Json map;
  typedef std::unordered_map<std::string, std::string> stringMap;
//  RestClient::Connection *clientImpl;
  http_client *clientImpl{nullptr};
  std::string applicationId;
  std::string clientKey;
  std::string baseURL;
  std::string apiVersion;
  std::string apiKeyField;
  std::string applicationIdField;
  std::string sessionTokenField;
  ParseUser* currentUser;
  ParseACL* defaultACL;
  bool currentUserAccessForDefaultACL;
  int timeoutInterval;
  bool productionMode;
  bool isLastModify;

private:
  static ParsePaasClient *_instance;
  static std::recursive_mutex _lock;
  stringMap _headerMap;
public:
    void logOut();
    void setBaseUrl(const std::string &b);

private:
    bool _requestProcessed;
  std::string _responseBody;

private:
  ParsePaasClient();
  void updateHeaders(std::string const &content_type="application/json");
  void addHeaderMap(stringMap && headerMap);
  pplx::task<Json> processResponse(const pplx::task<web::http::http_response>& response,
                       web::http::status_code);

public:
  ~ParsePaasClient();
  static ParsePaasClient* sharedInstance();
  void release();
  void clearLastModifyCache();
//  bool isUrulu();
//  void useAVOSCloudUS();
//  void useAVOSCloudCN();
//  void useServer(std::string const & baseURL);

  static map batchMethod(std::string const & method,
                         std::string const & path,
                         map const & body,
                         map const & parameters);

  static void updateBatchMethod(std::string const & method,
                                std::string const & path,
                                map & map);

  pplx::task<Json> getObject(std::string const & path,
                 map const & parameters,
                  web::http::status_code expectCode=web::http::status_codes::OK);

  pplx::task<Json> getObject(std::string const & path,
                 map const & parameters,
                 CachePolicy policy,
                 int maxCacheAge,
                 web::http::status_code expectCode=web::http::status_codes::OK);

  pplx::task<Json> putObject(std::string const & path,
                 map const & parameters,
                 std::string const & sessionToken,
                 web::http::status_code expectCode=web::http::status_codes::OK);

  pplx::task<Json> postBatchObject(std::vector<map> const & parameterArray,
                 web::http::status_code expectCode=web::http::status_codes::OK);

  pplx::task<Json> postBatchObject(std::vector<map> const & parameterArray,
                   map const & headerMap,
                   bool const & isEventually,
             web::http::status_code expectCode=web::http::status_codes::OK);

  pplx::task<Json> postBatchSaveObject(std::vector<map> const & parameterArray,
                       map const & headerMap,
                       bool const & isEventually,
             web::http::status_code expectCode=web::http::status_codes::OK);

   pplx::task<Json> postObject(std::string const &path, map const &parameters,
             web::http::status_code expectCode=web::http::status_codes::OK,
                std::string const &content_type="application/json");
 pplx::task<Json>  postFile(std::string const &path, const std::string &data,
            std::string const &content_type,
             web::http::status_code expectCode=web::http::status_codes::OK);

  pplx::task<Json> postObject(std::string const & path,
              map const & parameters,
              bool const & isEventually,
              web::http::status_code expectCode=web::http::status_codes::OK);

  pplx::task<Json> deleteObject(std::string const & path,
                map const & parameters,
              web::http::status_code expectCode=web::http::status_codes::OK);

  pplx::task<Json> deleteObject(std::string const & path,
                map const & parameters,
                bool const & isEventually,
              web::http::status_code expectCode=web::http::status_codes::OK);

  pplx::task<Json> deleteObjects(std::vector<ParseObject> const & objects,
                     bool const & isEventually,
                  web::http::status_code expectCode=web::http::status_codes::OK);

  void cancelQuery(std::string const & path, map const & parameters);

  std::string absoluteStringFromPath(std::string const & path,
                                     map const & parameters);

  void handleAllArchiveRequest();


/**
 * \~english create url and set headers
 * \~english \param path
 * \~english \param parameters
 * \~english \param isQuery
 * \~english \return
 */
        http_request createRequestUrl(std::string const &path,
                                      map const &parameters,
                                      bool isQuery);

//  void uploadFileToQiniuWithBodyAndCallback(std::string const & body,
//                                            BooleanResultCallback callback);

  void fetchFileDataIntoPathWithUrl(std::string const & path,
                                    std::string const & url);
};

NS_PC_END

#endif  // INCLUDE_REQUEST_PARSEPAASCLIENT_H_
