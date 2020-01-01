#include <string>
#include <unordered_map>
#include "Request/ParsePaasClient.h"

#include <catch2/catch.hpp>
#include <ParseCloud.h>

using namespace std;
using namespace parsecloud;

using Json=nlohmann::json;


TEST_CASE("Post TEST", "[POST]") {
    ParseCloud::setApplicationConfig(SERVER_URL, APPID, APPKEY);
    ParsePaasClient *client = ParsePaasClient::sharedInstance();

    Json parameters;
    parameters["name"] = "debugger";
    std::string path = "classes/Post";

    client->postObject(path, parameters).then([client, path](Json root) {
        auto error = ParseErrorUtils::errorFromJSON(root);
        CHECK(error.domain.length() == 0);
        auto objId = root["objectId"].get<std::string>();
        CHECK(objId.length() > 0);
        return client->getObject(path + "/" + root["objectId"].get<std::string>(), {}).then([objId](Json root) {
            auto error = ParseErrorUtils::errorFromJSON(root);
            CHECK(error.domain.length() == 0);
            for (auto it = root.begin(); it != root.end(); ++it) {
                CHECK(root["objectId"] == objId);
            }
        });
    }).get();
}

