
#include "ParseCloud.h"
#include "User/ParseUser.h"
#include <fstream>
#include <string>
#include <vector>
#include <nlohmann/json.hpp>
#include <catch/catch.hpp>
#include <restclient-cpp/restclient.h>
#include <restclient-cpp/connection.h>

using namespace std;
using namespace parsecloud;


TEST_CASE("ParseUser SingUp", "[SingUp]") {
    ParseCloud::setApplicationConfig(SERVER_URL, APPID,
                                     APPKEY);

    ParseUser *user = ParseUser::user();
    user->username = "myname";
    user->password = "123456";
    user->email = "myname@example.com";

    user->signUp(parsecloud::BooleanResultCallback());

    CHECK(user->isAuthenticated());
    user->deleteInBackground();

    ParseUser::logOut();
    user->release();

}

TEST_CASE("ParseUser Login", "[Login]") {
    ParseCloud::setApplicationConfig(SERVER_URL, APPID,
                                     APPKEY);
    auto user = ParseUser::user();
    user->username = "myname";
    user->password = "123456";
    user->email = "myname@example.com";
    user->signUp([](bool const & successed,PCError const & error){
       REQUIRE(successed) ;
    });

    ParseUser::logOut();

    ParseUser::loginWithUsernameAndPassword("myname", "123456");

    REQUIRE (ParseUser::currentUser() != nullptr);

    CHECK (ParseUser::currentUser()->isAuthenticated());

    ParseUser::currentUser()->deleteInBackground();

    user->release();
}

TEST_CASE("ParseUser UpdatePassowrd", "[Password]") {
    ParseCloud::setApplicationConfig(SERVER_URL, APPID,
                                     APPKEY);

    ParseUser *user = ParseUser::user();
    user->username = "myname";
    user->password = "123456";
    user->email = "myname@example.com";
    user->signUp([](const bool &successed,PCError const error){
        REQUIRE(successed );
        REQUIRE(error.domain.empty());
    });

    user->updatePasswordWithCallback(
            "123456", "654321", [&](
                    Json const &root, PCError
            const &error) {
                std::cout << error.code <<
                          std::endl;
// do nothing
            });

    ParseUser::logOut();

    ParseUser::loginWithUsernameAndPassword("myname", "654321");

    REQUIRE (ParseUser::currentUser() != nullptr);

    CHECK (ParseUser::currentUser()->isAuthenticated());
    ParseUser::currentUser()->deleteInBackground();

    user->release();
}

//!!TODO need configured server to use this
TEST_CASE("RequestPasswordResetForEmail", "[Password]") {
    ParseCloud::setApplicationConfig(SERVER_URL, APPID,
                                     APPKEY);

    ParseUser *user = ParseUser::user();
    user->username = "myname";
    user->password = "123456";
    user->email = "myname@example.com";
    user->signUp(parsecloud::BooleanResultCallback());

    REQUIRE (ParseUser::currentUser() != nullptr);

    CHECK (ParseUser::currentUser()->isAuthenticated());

    ParseUser::requestPasswordResetForEmail("myname@example.com", [](std::string const &str, const PCError &error){
        REQUIRE(error.domain.empty());
    });
    user->deleteInBackground();

    user->release();
}
