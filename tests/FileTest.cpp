#include <cstdio>
#include <fstream>
#include <string>
#include <vector>
#include "ParseCloud.h"
#include "File/ParseFile.h"
#include <catch/catch.hpp>

using namespace std;
using namespace parsecloud;


TEST_CASE("File SaveFile Test", "[File]") {
    ParseCloud::setApplicationConfig(SERVER_URL,APPID,APPKEY);
    ParseFile *file = ParseFile::fileWithPath("/home/tender/Desktop/1.png");

    file->saveInBackgroundWithCallback([&](bool const &succeeded, PCError const &error) {
        CHECK(succeeded);
    });

    file->deleteInBackground();

    file->release();
}

TEST_CASE("File FetchFile Test", "[File]") {
    ParseCloud::setApplicationConfig(std::__cxx11::string(), APPID, APPKEY);
    ParseFile *file = ParseFile::fileWithUrl("http://localhost:1337/parse/files/myAppId/8835d4d42d0c11a83c05fdc16d01fff8_1.png");
    std::string path("/home/tender/Desktop/2.png");
    file->fetchFileDataIntoPath(path);

    std::ifstream ifs(path);
    if (ifs.good()) {
        std::string content;
        ifs >> content;
        ifs.close();

        CHECK(content.length() > 0);
    }

    CHECK(remove(path.c_str()) == 0);

    file->release();
}
