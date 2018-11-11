#include "ACL/ParseACL.h"

#include <string>
#include "ACL/ParseRole.h"
#include "User/ParseUser.h"
#include "Request/ParsePaasClient.h"
#include "Utils/ParseStringUtils.h"

NS_PC_BEGIN

    const static std::string readTag = "read";
    const static std::string writeTag = "write";
    const static std::string publicTag = "*";

    ParseACL::ParseACL() {
        if (ParsePaasClient::sharedInstance()->defaultACL != nullptr) {
            this->permissionById = ParsePaasClient::sharedInstance()->
                    defaultACL->permissionById;
        } else {
            this->permissionById = Json();
        }
    }

    ParseACL *ParseACL::ACL() {
        return new ParseACL();
    }

    ParseACL *ParseACL::ACLForUser(ParseUser *user) {
        auto *result = new ParseACL();
        result->setReadAccess(true, user);
        result->setWriteAccess(true, user);
        return result;
    }

    void ParseACL::release() {
        PC_SAFE_DELETE(this);
    }

    Json ParseACL::dictionaryToReadAndWrite(bool read, bool write) {
        Json dictionary;
        dictionary[readTag] = read;
        dictionary[writeTag] = write;
        return dictionary;
    }

    Json ParseACL::dictionaryForKeyIfCreate(std::string key, bool create) {
        Json data;

        if (this->permissionById.find(key)!=this->permissionById.end()) {
            data = this->permissionById[key];
        }

        if (data.is_null() && create) {
            data = Json();
            this->permissionById[key] = data;
        }

        return data;
    }

    void ParseACL::setAllowReadForKey(bool allowed, std::string key) {
        Json data = this->dictionaryForKeyIfCreate(key, allowed);
        if (allowed) {
            data[readTag] = true;
        } else {
            data.erase(readTag);
        }

        this->permissionById[key] = data;
    }

    bool ParseACL::isReadAllowedForKey(std::string key) {
        Json data = this->dictionaryForKeyIfCreate(key, false);
        return data[readTag] == true;
    }

    void ParseACL::setAllowWriteForyKey(bool allowed, std::string key) {
        Json data = this->dictionaryForKeyIfCreate(key, allowed);
        if (allowed) {
            data[writeTag] = true;
        } else {
            data.erase(writeTag);
        }

        this->permissionById[key] = data;
    }

    bool ParseACL::isWriteAllowedForyKey(std::string key) {
        Json data = this->dictionaryForKeyIfCreate(key, false);
        return data[writeTag] == 1;
    }

    void ParseACL::setPublicReadAccess(bool allowed) {
        this->setAllowReadForKey(allowed, publicTag);
    }

    bool ParseACL::getPublicReadAccess() {
        return this->isReadAllowedForKey(publicTag);
    }

    void ParseACL::setPublicWriteAccess(bool allowed) {
        this->setAllowWriteForyKey(allowed, publicTag);
    }

    bool ParseACL::getPublicWriteAccess() {
        return this->isWriteAllowedForyKey(publicTag);
    }

    void ParseACL::setReadAccess(bool allowed, std::string userId) {
        this->setAllowReadForKey(allowed, userId);
    }

    bool ParseACL::getReadAccess(std::string userId) {
        return this->isReadAllowedForKey(userId);
    }

    void ParseACL::setWriteAccess(bool allowed, std::string userId) {
        this->setAllowWriteForyKey(allowed, userId);
    }

    bool ParseACL::getWriteAccess(std::string userId) {
        return this->isWriteAllowedForyKey(userId);
    }

    void ParseACL::setReadAccess(bool allowed, ParseUser *user) {
        if (user->hasValidObjectId()) {
            this->setAllowReadForKey(allowed, user->objectId);
        }
    }

    bool ParseACL::getReadAccess(ParseUser *user) {
        if (user->hasValidObjectId()) {
            return this->isReadAllowedForKey(user->objectId);
        } else {
            // Default value is true
            return true;
        }
    }

    void ParseACL::setWriteAccess(bool allowed, ParseUser *user) {
        if (user->hasValidObjectId()) {
            this->setAllowWriteForyKey(allowed, user->objectId);
        }
    }

    bool ParseACL::getWriteAccess(ParseUser *user) {
        if (user->hasValidObjectId()) {
            return this->isWriteAllowedForyKey(user->objectId);
        } else {
            // Default value is true
            return true;
        }
    }

    std::string ParseACL::roleName(std::string name) {
        return StringUtils::string_format("role:%s", name);
    }


    void ParseACL::setReadAccessForRole(bool allowed, std::string name) {
        this->setAllowReadForKey(allowed, this->roleName(name));
    }

    bool ParseACL::getReadAccessForRole(std::string name) {
        return this->isReadAllowedForKey(this->roleName(name));
    }

    void ParseACL::setWriteAccessForRole(bool allowed, std::string name) {
        this->setAllowWriteForyKey(allowed, this->roleName(name));
    }

    bool ParseACL::getWriteAccessForRole(std::string name) {
        return this->isWriteAllowedForyKey(this->roleName(name));
    }

    void ParseACL::setReadAccessForRole(bool allowed, ParseRole *role) {
        this->setAllowReadForKey(allowed, this->roleName(role->name));
    }

    bool ParseACL::getReadAccessForRole(ParseRole *role) {
        return this->isReadAllowedForKey(this->roleName(role->name));
    }

    void ParseACL::setWriteAccessForRole(bool allowed, ParseRole *role) {
        this->setAllowWriteForyKey(allowed, this->roleName(role->name));
    }

    bool ParseACL::getWriteAccessForRole(ParseRole *role) {
        return isWriteAllowedForyKey(this->roleName(role->name));
    }

    void ParseACL::setDefaultACLWithCurrentUser(ParseACL *acl, bool currentUserAccess) {
        ParsePaasClient::sharedInstance()->defaultACL = acl;
        ParsePaasClient::sharedInstance()->
                currentUserAccessForDefaultACL = currentUserAccess;
    }

NS_PC_END
