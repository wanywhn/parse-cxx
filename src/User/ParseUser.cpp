#include "User/ParseUser.h"

#include <string>
#include <iostream>
#include <spdlog/spdlog.h>

#include "Request/ParsePaasClient.h"
#include "Utils/ParseStringUtils.h"

NS_PC_BEGIN

    ParseUser::ParseUser() : sessionToken(),
                             username(),
                             password(),
                             email() {
        this->className = "_User";
    }

    ParseUser *ParseUser::currentUser() {
        return ParsePaasClient::sharedInstance()->currentUser;
    }

    bool ParseUser::isAuthenticated() {
        return this->sessionToken.length() > 0;

    }

    ParseUser *ParseUser::user() {
        auto *user = new ParseUser();

        return user;
    }

    void ParseUser::release() {
        PC_SAFE_DELETE(this);
    }

    void ParseUser::verificationEmailRequestWithCallback(std::string email,
                                                         BooleanResultCallback callback) {
        Json parameters;
        parameters["email"] = email;
        ParsePaasClient::sharedInstance()->
                postObject("verificationEmailRequest",
                           parameters,web::http::status_codes::OK)
                           .then([](Json root){

                           });
    }

    pplx::task<PCError> ParseUser::signUp() {
        if (!this->isAuthenticated() &&
            this->username.length() > 0 &&
            this->password.length() > 0) {
            this->setObjectForKey(this->username, "username");
            this->setObjectForKey(this->password, "password");

            if (this->email.length() > 0) {
                this->setObjectForKey(this->email, "email");
            }

            return this->saveInBackground().then([this](PCError error){
                this->sessionToken = this->localData["sessionToken"];
                ParsePaasClient::sharedInstance()->currentUser=this;
                return error;
            });
        }
    }

    pplx::task<PCError> ParseUser::updatePasswordWithCallback(std::string oldPassword, std::string newPassword) {
        if (this->isAuthenticated() &&
            oldPassword.length() > 0 &&
            newPassword.length() > 0) {
            std::string path = StringUtils::string_format("users/%s", this->objectId);
            Json parameters;
            parameters["password"] = newPassword;

            return ParsePaasClient::sharedInstance()->
                    putObject(path,
                              parameters,
                              this->sessionToken)
                              .then([](Json root){
                                  return ParseErrorUtils::errorFromJSON(root);

                              });
        } else {
            if (!this->isAuthenticated()) {
                Json root;
                PCError error(kErrorDomain, SessionMissing);
                return pplx::task_from_result(error);
            } else if (!(oldPassword.length() > 0 && newPassword.length() > 0)) {
                Json root;
                PCError error(kErrorDomain, PasswordMissing);
                return pplx::task_from_result(error);
            }
        }
    }

    ParseUser *ParseUser::loginWithUsernameAndPassword(std::string username,
                                                       std::string password) {
        if (username.length() > 0 && password.length() > 0) {
            std::string path = "login";
            Json parameters;
            parameters["username"] = username;
            parameters["password"] = password;

            ParseUser *user = ParseUser::user();
            user->username = username;
            user->password = password;
            ParsePaasClient::sharedInstance()->
                    postObject(path,
                               parameters)
                               .then([&user](Json root){



                                       user->objectId = root["objectId"];
                                       user->username = root["username"];
                                       user->sessionToken = root["sessionToken"];
                                       user->email = root["email"];

                                       ParsePaasClient::sharedInstance()->currentUser = user;

                               });

            return user;
        }

        return nullptr;
    }

    void ParseUser::logOut() {
        ParsePaasClient::sharedInstance()->logOut();
    }

    pplx::task<PCError> ParseUser::requestPasswordResetForEmail(std::string email) {
        std::string path = "requestPasswordReset";
        Json parameters;
        parameters["email"] = email;

        ParsePaasClient::sharedInstance()->
                        postObject(path,
                                   parameters, web::http::status_codes::OK)
                .then([](Json root) {
                    return ParseErrorUtils::errorFromJSON(root);
                });
    }

    ParseQuery *ParseUser::query() {
        ParseQuery *query = ParseQuery::queryForClassName("_User");

        return query;
    }

NS_PC_END
