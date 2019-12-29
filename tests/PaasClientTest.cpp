#include <string>
#include <unordered_map>
#include "Request/ParsePaasClient.h"

#include <catch2/catch.hpp>

using namespace std;
using namespace parsecloud;

using Json=nlohmann::json ;


TEST_CASE("Post TEST","[POST]"){
  ParsePaasClient *client = ParsePaasClient::sharedInstance();

  client->applicationId = APPID;
  client->clientKey = APPKEY;
  client->baseURL=SERVER_URL;

  Json parameters;
  parameters["name"] = "debugger";

  client->postObject("classes/Post", parameters, [&](Json const &root, PCError const &error) {
        CHECK(error.domain.length() == 0);
        CHECK(root["objectId"].get<std::string>().length() > 0);
  });
}
TEST_CASE("Get Test","[GET]"){
  ParsePaasClient *client = ParsePaasClient::sharedInstance();

  client->applicationId = APPID;
  client->clientKey = APPKEY;
  client->baseURL=SERVER_URL;

  Json parameters;
  client->getObject("classes/Post/D9MgXhJm58", parameters, [&](Json const & root, PCError const & error){
      CHECK(error.domain.length() == 0);

      for (auto it = root.begin(); it != root.end(); ++it)
      {
        CHECK(root["objectId"]=="D9MgXhJm58");
      }
  },
  web::http::status_codes::OK);
}

