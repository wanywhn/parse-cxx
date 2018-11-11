#ifndef INCLUDE_REQUEST_PARSEPAASCLIENT_H_
#define INCLUDE_REQUEST_PARSEPAASCLIENT_H_

#include <string>
#include <vector>
#include <unordered_map>
#include <unordered_set>
#include <mutex>
#include "Utils/ParsePlatformMacros.h"
#include "Utils/ParseStringUtils.h"
#include "ParseConstants.h"
#include "Object/ParseObject.h"
#include "User/ParseUser.h"
#include "ACL/ParseACL.h"
#include <restclient-cpp/restclient.h>
#include <restclient-cpp/connection.h>

NS_PC_BEGIN


class ParsePaasClient {
public:
  typedef Json map;
  typedef std::unordered_map<std::string, std::string> stringMap;
  RestClient::Connection *clientImpl;
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

private:
    bool _requestProcessed;
  std::string _responseBody;

private:
  ParsePaasClient();
  void updateHeaders(std::string const &content_type="application/json");
  void addHeaderMap(stringMap && headerMap);
  void processResponse(const RestClient::Response &response,
                       IdResultCallback callback);

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

  void getObject(std::string const & path,
                 map const & parameters,
                 IdResultCallback callback);

  void getObject(std::string const & path,
                 map const & parameters,
                 CachePolicy policy,
                 int maxCacheAge,
                 IdResultCallback callback);

  void putObject(std::string const & path,
                 map const & parameters,
                 std::string const & sessionToken,
                 IdResultCallback callback);

  void postBatchObject(std::vector<map> const & parameterArray,
                       VectorResultCallback callback);

  void postBatchObject(std::vector<map> const & parameterArray,
                       map const & headerMap,
                       bool const & isEventually,
                       VectorResultCallback callback);

  void postBatchSaveObject(std::vector<map> const & parameterArray,
                           map const & headerMap,
                           bool const & isEventually,
                           IdResultCallback callback);

  void postObject(std::string const &path, map const &parameters, IdResultCallback callback,
                    std::string const &content_type="application/json");
  void postFile(std::string const &path, const std::string &data, IdResultCallback callback,
                std::string const &content_type);

  void postObject(std::string const & path,
                  map const & parameters,
                  bool const & isEventually,
                  IdResultCallback callback);

  void deleteObject(std::string const & path,
                    map const & parameters,
                    IdResultCallback callback);

  void deleteObject(std::string const & path,
                    map const & parameters,
                    bool const & isEventually,
                    IdResultCallback callback);

  void deleteObjects(std::vector<ParseObject> const & objects,
                     bool const & isEventually,
                     IdResultCallback callback);

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
  std::string createRequestUrl(std::string const &path,
                              map const &parameters,
                              bool isQuery);

//  void uploadFileToQiniuWithBodyAndCallback(std::string const & body,
//                                            BooleanResultCallback callback);

  void fetchFileDataIntoPathWithUrl(std::string const & path,
                                    std::string const & url);
};

NS_PC_END

#endif  // INCLUDE_REQUEST_PARSEPAASCLIENT_H_
