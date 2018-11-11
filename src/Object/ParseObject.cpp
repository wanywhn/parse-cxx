#include <utility>
#include <string>
#include <vector>
#include <algorithm>
#include <spdlog/spdlog.h>

#include "Object/ParseObject.h"
#include "Object/ParseRelation.h"
#include "Request/ParsePaasClient.h"

NS_PC_BEGIN

    using boost::any_cast;

    ParseObject::ParseObject() : objectId(),
                                 addedRelationData(),
                                 removedRelationData() {
    }

    ParseObject *ParseObject::objectForClassName(std::string const &className) {
        auto *object = new ParseObject();
        object->className = className;

        return object;
    }

    ParseObject *ParseObject::objectForClassNameAndId(std::string const &className,
                                                      std::string const &objectId) {
        auto *object = new ParseObject();
        object->className = className;
        object->objectId = objectId;

        return object;
    }

    void ParseObject::release() {
        PC_SAFE_DELETE(this);
    }

    void ParseObject::setACL(ParseACL *acl) {
        this->acl = acl;
        this->localData["ACL"] = acl->permissionById;
    }

    ParseACL *ParseObject::getACL() {
        return this->acl;
    }

    std::vector<std::string> ParseObject::allKeys() {
        std::vector<std::string> keys;
        for (auto it = this->localData.begin(); it != this->localData.end(); ++it) {
            keys.push_back(it.key());
        }

        return keys;
    }

    Json ParseObject::objectForKey(std::string const &key) {
        return this->localData[key];
    }

    void ParseObject::setObjectForKey(Json const &object,
                                      std::string const &key) {
        this->localData[key] = object;
    }

    void ParseObject::setObjectForKey(ParseObject *const &object,
                                      std::string const &key) {
        if (object->hasValidObjectId()) {
            Json value;
            value["__type"] = "Pointer";
            value["className"] = object->className;
            value["objectId"] = object->objectId;

            this->localData[key] = value;
        }
    }

    void ParseObject::setObjectForKey(ParseGeoPoint *const &geoPoint,
                                      std::string const &key) {
        Json value =geoPoint->toJson();
        this->localData[key] = value;
    }

    void ParseObject::removeObjectForKey(std::string const &key) {
        this->localData.erase(key);
    }

    //!!TODO fetch?
    ParseRelation *ParseObject::relationForKey(std::string key) {
        std::vector<ParseObject *> array;
        if (this->addedRelationData.find(key) != this->addedRelationData.end()) {
            array = this->addedRelationData[key];
        } else {
            this->addedRelationData[key] = array;
        }

        ParseObject *target = nullptr;
        if (!array.empty()) {
            target = array[0];
        }

        auto *relation = new ParseRelation();
        relation->parent = this;
        relation->key = key;

        if (target != nullptr) {
            relation->targetClass = target->className;
        }

        return relation;
    }

    void ParseObject::addRelationForKey(ParseObject *object, std::string key) {
        this->addObjectIntoRelationDataForKey(&(this->addedRelationData), object, std::move(key));
    }

    void ParseObject::removeRelationForKey(ParseObject *object, std::string key) {
        this->addObjectIntoRelationDataForKey(&(this->removedRelationData), object, std::move(key));
    }

    void ParseObject::addObjectIntoRelationDataForKey(
            std::unordered_map<std::string, std::vector<ParseObject *>> *relationData,
            ParseObject *object,
            std::string key) {
        if (object->hasValidObjectId()) {
            auto got = relationData->find(key);
            if (got == relationData->end()) {
                std::vector<ParseObject *> array;
                array.push_back(object);
                (*relationData)[key] = array;
            } else {
                got->second.push_back(object);
            }
        }
    }

    Json ParseObject::generateRelationObjectsByArray(std::vector<ParseObject *> objects,
                                                     bool isAdded) {
        Json root;
        if (isAdded) {
            root["__op"] = "AddRelation";
        } else {
            root["__op"] = "RemoveRelation";
        }

        for (auto &object : objects) {
            Json obj;
            obj["__type"] = "Pointer";
            obj["className"] = object->className;
            obj["objectId"] = object->objectId;
            root["objects"].emplace_back(obj);
        }

        return root;
    }

    void ParseObject::saveInBackground() {
        this->saveInBackgroundWithCallback([&](bool const &succeeded, PCError const &error) {
            // do nothing
        });
    }

    void ParseObject::saveInBackgroundWithCallback(BooleanResultCallback callback) {
        for (auto &kv : this->addedRelationData) {
            this->localData[kv.first] = this->generateRelationObjectsByArray(kv.second, true);
        }

        for (auto &kv : this->removedRelationData) {
            this->localData[kv.first] = this->generateRelationObjectsByArray(kv.second, false);
        }

        this->addedRelationData.clear();
        this->removedRelationData.clear();
        if (this->hasValidObjectId()) {
            std::string sessionToken;
            ParsePaasClient::sharedInstance()->
                    putObject(this->myObjectPath(),
                              this->localData,
                              sessionToken, [&](Json const &root,
                                                PCError const &error) {
                        callback(error.domain.length() == 0, error);
                    });
        } else {
            ParsePaasClient::sharedInstance()->
                    postObject(this->myObjectPath(),
                               this->localData,
                               [&](Json const root,
                                                       PCError const &error) {
                                                       if (error.domain.length() == 0) {
                                                           this->objectId = root["objectId"];
                                                           this->createdAt = root["createdAt"];
                                                           if (root.find("sessionToken") != root.end()) {
                                                               this->localData["sessionToken"] = root["sessionToken"];
                                                           }

                                                           callback(true, error);
                                                       } else {
                                                           callback(false, error);
                                                       }
                                                   });
        }
    }

    void ParseObject::saveAllInBackground(std::vector<ParseObject *> objects) {
        for (auto &object : objects) {
            object->saveInBackground();
        }
    }

    void ParseObject::saveAllInBackgroundWithCallback(std::vector<ParseObject *> objects,
                                                      BooleanResultCallback callback) {
        for (auto &object : objects) {
            object->saveInBackgroundWithCallback(callback);
        }
    }

    void ParseObject::fetch() {
        std::vector<std::string> keys;
        this->fetchWithKeys(keys);
    }

    void ParseObject::fetchWithKeys(std::vector<std::string> keys) {
        if (this->hasValidObjectId()) {
            Json parameters;

            if (!keys.empty()) {
                std::string includeKeys;
                for (auto &key:keys) {
                    includeKeys.append(key);
                    includeKeys.append(",");
                }

                parameters["include"] = includeKeys;
            }

            ParsePaasClient::sharedInstance()->
                    getObject(this->myObjectPath(),
                              parameters,
                              [&](Json const &root,
                                  PCError const &error) {
                                  if (error.domain.length() == 0) {
                                      for (auto it = root.begin(); it != root.end(); ++it) {
                                          std::string key = it.key();
                                          if (key == "createdAt") {
                                              this->createdAt = it.value();
                                          } else if (key == "updatedAt") {
                                              this->updatedAt = it.value();
                                          }
                                      }

                                      this->localData = root;
                                      this->localData.erase("objectId");
                                      this->localData.erase("createdAt");
                                      this->localData.erase("updatedAt");
                                  }
                              });
        }
    }

    void ParseObject::deleteInBackground() {
        this->deleteInBackgroundWithCallback([&](bool const &succeeded, PCError const &error) {
//            if(succeeded){
//                spdlog::get("console")->debug("delete successed");
//            }else{
//                spdlog::get("console")->error("delete in backgroud error");
//            }
            // do nothing
        });
    }

    void ParseObject::deleteInBackgroundWithCallback(BooleanResultCallback callback) {
        if (this->hasValidObjectId()) {
            Json parameters;
            ParsePaasClient::sharedInstance()->
                    deleteObject(this->myObjectPath(),
                                 parameters,
                                 [&](Json const &root,
                                     PCError const &error) {
                                     callback(error.domain.length() == 0, error);
                                 });
        }
    }

    bool ParseObject::hasValidObjectId() {
        return this->objectId.length() > 0;
    }

    std::string ParseObject::myObjectPath() {
        if (this->hasValidObjectId()) {
            return StringUtils::string_format("classes/%s/%s", this->className, this->objectId);
        } else {
            return StringUtils::string_format("classes/%s", this->className);
        }
    }

//    ParseObject::~ParseObject() = default;

NS_PC_END
