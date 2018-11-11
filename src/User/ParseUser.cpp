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
        ParseUser *user = ParsePaasClient::sharedInstance()->currentUser;
        if (user != nullptr) {
            return user;
        } else {
            user = ParseUser::user();
            ParsePaasClient::sharedInstance()->currentUser = user;

            return user;
        }
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
                           parameters, [&](Json const &root, PCError const &error) {
                                    callback(error.domain.length() == 0, error);
                                });
    }

    void ParseUser::signUp(BooleanResultCallback callback) {
        if (!this->isAuthenticated() &&
            this->username.length() > 0 &&
            this->password.length() > 0) {
            this->setObjectForKey(this->username, "username");
            this->setObjectForKey(this->password, "password");

            if (this->email.length() > 0) {
                this->setObjectForKey(this->email, "email");
            }

            this->saveInBackgroundWithCallback([&](bool const &succeeded, PCError const &error) {
                if (succeeded) {
                    this->sessionToken = this->localData["sessionToken"];
                    ParsePaasClient::sharedInstance()->currentUser = this;
                } else {
                    std::cout << error.code << ":" << error.domain << std::endl;
                }
                callback(succeeded,error);
            });
        }
    }

    void ParseUser::updatePasswordWithCallback(std::string oldPassword,
                                               std::string newPassword,
                                               IdResultCallback callback) {
        if (this->isAuthenticated() &&
            oldPassword.length() > 0 &&
            newPassword.length() > 0) {
            std::string path = StringUtils::string_format("users/%s", this->objectId);
            Json parameters;
            parameters["password"] = newPassword;

            ParsePaasClient::sharedInstance()->
                    putObject(path,
                              parameters,
                              this->sessionToken,
                              [&](Json const &root, PCError const &error) {
                                  callback(root, error);
                              });
        } else {
            if (!this->isAuthenticated()) {
                Json root;
                PCError error(kErrorDomain,SessionMissing);
                callback(root, error);
            } else if (!(oldPassword.length() > 0 && newPassword.length() > 0)) {
                Json root;
                PCError error(kErrorDomain,PasswordMissing);
                callback(root, error);
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
                               parameters,
                               [&](Json const &root, PCError const &error) {
                                                       if (error.domain.length() == 0) {
                                                           user->objectId = root["objectId"];
                                                           user->username = root["username"];
                                                           user->sessionToken = root["sessionToken"];
                                                           user->email = root["email"];

                                                           ParsePaasClient::sharedInstance()->currentUser = user;
                                                       } else {
                                                           user = nullptr;
                                                       }
                                                   });

            return user;
        }

        return nullptr;
    }

    void ParseUser::logOut() {
        ParsePaasClient::sharedInstance()->logOut();
    }

    void ParseUser::requestPasswordResetForEmail(std::string email, StringResultCallback callback) {
        std::string path = "requestPasswordReset";
        Json parameters;
        parameters["email"] = email;

        ParsePaasClient::sharedInstance()->
                postObject(path,
                           parameters,
                           [&](Json const &root, PCError const &error) {
                                               // TODO if error return error mesg
                                               if (!error.domain.empty()) {
                                                   spdlog::critical("requestPasswordResetForEmailError:{}", error.domain);
                                               }
                                               callback(root.dump(), error);
                                           } );
    }

    ParseQuery *ParseUser::query() {
        ParseQuery *query = ParseQuery::queryForClassName("_User");

        return query;
    }

NS_PC_END
