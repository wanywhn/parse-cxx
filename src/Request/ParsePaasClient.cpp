#include "Request/ParsePaasClient.h"
#include <stdlib.h>
#include <time.h>
#include <string>
#include <fstream>
#include <thread>
#include <exception>

#define SPDLOG_TRACE_ON

#include <spdlog/spdlog.h>
#include <set>
#include <Request/ParsePaasClient.h>
#include <sstream>


#define kDefaultNetworkTimeoutInterval 10

NS_PC_BEGIN

    const std::string API_VERSION = "1.1";
    const std::string QiniuServerPath = "http://up.qiniu.com";

    ParsePaasClient *ParsePaasClient::_instance = nullptr;
    std::recursive_mutex ParsePaasClient::_lock;

    ParsePaasClient::ParsePaasClient()
            :
            apiVersion(API_VERSION),
            apiKeyField("X-Parse-REST-API-Key"),
            applicationIdField("X-Parse-Application-Id"),
            sessionTokenField("X-Parse-Session-Token"),
            currentUser(nullptr),
            defaultACL(nullptr),
            timeoutInterval(kDefaultNetworkTimeoutInterval),
            productionMode(true),
            _headerMap(),
            _responseBody() {
    }
    void ParsePaasClient::setBaseUrl(const std::string &b) {
        if(clientImpl){
            delete clientImpl;
        }
        clientImpl=new http_client(b);
        this->baseURL=b;
    }

    ParsePaasClient::~ParsePaasClient() {
        delete clientImpl;

    }

    ParsePaasClient *ParsePaasClient::sharedInstance() {
        std::lock_guard<std::recursive_mutex> locker(_lock);
        if (_instance == nullptr) {
            _instance = new ParsePaasClient();
        }

        return _instance;
    }

    void ParsePaasClient::release() {
        PC_SAFE_DELETE(this);
    }

    void ParsePaasClient::clearLastModifyCache() {
    }

//    bool ParsePaasClient::isUrulu() {
//        return (this->applicationIdField).compare("X-LC-Id") == 0;
//    }

//    void ParsePaasClient::useServer(std::string const &baseURL) {
//        this->baseURL = baseURL;
//        RestClient::init();
    //TODO Change ID
//        this->applicationIdField = "X-LC-Id";
//        this->apiKeyField = "X-LC-Key";
//        this->sessionTokenField = "X-LC-Session";
//    }

    void ParsePaasClient::addHeaderMap(stringMap &&headerMap) {
        for (const auto &it : headerMap) {
            this->_headerMap[it.first] = it.second;
        }
    }

    void ParsePaasClient::updateHeaders(std::string const &content_type) {

        std::unordered_map<std::string, std::string> _map;
        _map[this->apiKeyField] = this->clientKey;
        _map[this->applicationIdField] = this->applicationId;
//        _map["Accept"] = "application/json";
        _map["Content-Type"] = content_type;
        if (this->currentUser != nullptr && this->currentUser->sessionToken.length() > 0) {
            _map[this->sessionTokenField] = this->currentUser->sessionToken;
        }

        this->addHeaderMap(std::move(_map));
    }

    //TODO complete it
    ParsePaasClient::map ParsePaasClient::batchMethod(std::string const &method,
                                                      std::string const &path,
                                                      ParsePaasClient::map const &body,
                                                      ParsePaasClient::map const &parameters) {
        std::string myPath = StringUtils::string_format("/%s/%s",
                                                        ParsePaasClient::sharedInstance()->apiVersion,
                                                        path);

        ParsePaasClient::map result;
        result["method"] = method;
        result["path"] = path;

        return result;
    }

    void ParsePaasClient::updateBatchMethod(std::string const &method,
                                            std::string const &path,
                                            map &map) {
        std::string myPath = StringUtils::string_format("/%s/%s",
                                                        ParsePaasClient::sharedInstance()->apiVersion,
                                                        path);
        map["method"] = method;
        map["path"] = myPath;
    }

    std::string UrlEncode(const std::string &str) {
        static std::set<char> sep{'{', '[', '}', ']'};
        std::ostringstream oss;
        for (auto c:str) {
            if (c == '"') {
                oss << "%22";
            } else {
                oss << c;
            }
        }
        return oss.str();
    }

    http_request ParsePaasClient::createRequestUrl(std::string const &path,
                                                   map const &parameters,
                                                   bool isQuery) {
        uri_builder builder(path);
//        assert(isQuery != parameters.empty());

        for (auto it = parameters.begin(); it != parameters.end(); ++it) {
            builder.append_query(it.key(), it.value().dump());
        }

        http_request request;
        request.set_request_uri(builder.to_uri());
        spdlog::debug("request URI:"+request.request_uri().to_string());

        auto &headers = request.headers();
        for (auto &it : _headerMap) {
            headers.add(it.first, it.second);
        }


        return request;
    }

    void ParsePaasClient::processResponse(const pplx::task<web::http::http_response> &response,
                                          const IdResultCallback &callback) {


        //!!FIXME use one json lib
        response.then([](http_response response1) {
            if (response1.status_code() != web::http::status_codes::OK) {
                spdlog::critical("Error response:{}", response1.status_code());
                std::cout<<response1.to_string();
            }

            return response1.extract_json();
        }).then([callback](web::json::value jvalue) {
            if (jvalue.is_null()) {
                spdlog::critical("Error parse null json");
            }
            try {
                Json str = Json::parse(jvalue.serialize(), nullptr, false);
                PCError error = ParseErrorUtils::errorFromJSON(str);
                callback(str, error);
            } catch (std::invalid_argument &e) {
                spdlog::critical("Error with:{}", e.what());
            }

        });

    }

    void ParsePaasClient::getObject(std::string const &path,
                                    map const &parameters,
                                    IdResultCallback callback) {
        std::lock_guard<std::recursive_mutex> locker(_lock);

        this->updateHeaders();
        auto requestUrl = this->createRequestUrl(path, parameters, true);
        requestUrl.set_method("GET");

        auto tex = this->clientImpl->request(requestUrl);

        processResponse(tex, callback);
    }

    void ParsePaasClient::putObject(std::string const &path,
                                    map const &parameters,
                                    std::string const &sessionToken,
                                    IdResultCallback callback) {
        std::lock_guard<std::recursive_mutex> locker(_lock);

        try {
            this->updateHeaders();

            ParsePaasClient::map myParameters;
            auto requestUrl = this->createRequestUrl(path, myParameters, false);
            requestUrl.set_method("PUT");

            auto jsonstr = parameters.dump();
            jsonstr = jsonstr == "null" ? "" : jsonstr;
            requestUrl.set_body(jsonstr);
            auto tex = this->clientImpl->request(requestUrl);

            processResponse(tex, callback);
        } catch (std::exception &e) {
            spdlog::critical("Error with:{}", e.what());
        }
    }

    void ParsePaasClient::postObject(std::string const &path, map const &parameters, IdResultCallback callback,
                                     std::string const &content_type) {
        std::lock_guard<std::recursive_mutex> locker(_lock);

        try {
            this->updateHeaders(content_type);
            ParsePaasClient::map myParameters;
            auto requestUrl = this->createRequestUrl(path, myParameters, false);
            requestUrl.set_method("POST");

            auto jsonstr = parameters.dump();
            jsonstr = jsonstr == "null" ? "" : jsonstr;
            requestUrl.set_body(jsonstr);
            auto tex = this->clientImpl->request(requestUrl);


            processResponse(tex, callback);
        } catch (std::exception &e) {

            spdlog::critical("Error with:{}", e.what());
        }
    }

    void ParsePaasClient::postFile(std::string const &path, const std::string &data, IdResultCallback callback,
                                   const std::string &content_type) {
        std::lock_guard<std::recursive_mutex> locker(_lock);
        try {
            this->updateHeaders(content_type);
            ParsePaasClient::map myParameters;
            auto requestUrl = this->createRequestUrl(path, myParameters, false);
            requestUrl.set_method("POST");
            requestUrl.set_body(data);


            auto tex = this->clientImpl->request(requestUrl);
            processResponse(tex, callback);

        } catch (std::exception &e) {

            spdlog::critical("Error with:{}", e.what());


        }
    }

    void ParsePaasClient::deleteObject(std::string const &path,
                                       map const &parameters,
                                       IdResultCallback callback) {
        std::lock_guard<std::recursive_mutex> locker(_lock);

        try {
            this->updateHeaders();
            auto requestUrl = this->createRequestUrl(path, parameters, false);
            requestUrl.set_method("DELETE");
            auto tex = this->clientImpl->request(requestUrl);

            processResponse(tex, callback);
        } catch (std::exception &e) {

        }
    }

//void ParsePaasClient::uploadFileToQiniuWithBodyAndCallback(std::string const & body,
//                                                        BooleanResultCallback callback) {
//  try {
//    http::client client;
//    uri::uri request_uri(QiniuServerPath);
//
//    http::client::request request(request_uri);
//    request << network::header("Content-Type",
//                               "multipart/form-data; boundary=28e84231563f43b08b1cc55659e9b3ac");
//
//    http::client::response response = client.post(request, body);
//
//    std::cout << response.body() << std::endl;
//
//    if (response.status() == 200) {
//      PCError error;
//      callback(true, error);
//    } else {
//      PCError error(kErrorDomain, kAVErrorUnsavedFile);
//      callback(false, error);
//    }
//  } catch (std::exception & e) {
//    PCError error(kErrorDomain, kAVErrorUnsavedFile);
//    callback(false, error);
//  }
//}

    void ParsePaasClient::fetchFileDataIntoPathWithUrl(std::string const &path,
                                                       std::string const &url) {
        /*
        if (url.length() > 0) {
            try {
                RestClient::Connection client(url);

                auto response = client.get("");

                std::ofstream ofs(path);
                ofs << response.body;
                ofs.close();
            } catch (std::exception &e) {
                spdlog::critical("Error with:{}", e.what());
            }
        }
         */
    }

    void ParsePaasClient::logOut() {
        ParsePaasClient::currentUser = nullptr;
        ParsePaasClient::_headerMap.clear();
    }



NS_PC_END
