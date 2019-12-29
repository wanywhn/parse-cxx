#include <utility>

#include "Query/ParseQuery.h"

#include <string>
#include <vector>

#include "Request/ParsePaasClient.h"
#include <iostream>

NS_PC_BEGIN

    ParseQuery::ParseQuery() : parameters(),
                               where(),
                               selectedKeys(),
                               include(),
                               order(),
                               className(),
                               limit(0),
                               skip(0) {
    }

    ParseQuery *ParseQuery::orQueryWithSubqueries(std::vector<ParseQuery *> queries) {
        if (queries.empty()) {
            return nullptr;
        }

        std::string className;

        Json input;

        for (auto &query : queries) {
            input.push_back(query->where);
            if (className.length() != 0 && query->className != className) {
                continue;
            }

            className = query->className;
        }

        ParseQuery *result = ParseQuery::queryForClassName(className);
        result->where["$or"] = input;
        return result;
    }

    ParseQuery *ParseQuery::andQueryWithSubqueries(std::vector<ParseQuery *> queries) {
        if (queries.empty()) {
            return nullptr;
        }

        std::string className;

        Json input;

        for (auto &query : queries) {
            input.push_back(query->where);
            if (className.length() != 0 && query->className != className) {
                continue;
            }

            className = query->className;
        }

        ParseQuery *result = ParseQuery::queryForClassName(className);
        if (input.size() > 1) {
            result->where["$and"] = input;
        } else {
            result->where = input[0];
        }

        return result;
    }

    ParseQuery *ParseQuery::queryForClassName(std::string className) {
        auto *query = new ParseQuery();
        query->className = std::move(className);

        return query;
    }

    void ParseQuery::release() {
        PC_SAFE_DELETE(this);
    }

    void ParseQuery::includeKey(std::string key) {
        this->include.insert(key);
    }

    void ParseQuery::selectKeys(std::vector<std::string> keys) {
        this->selectedKeys.insert(keys.begin(), keys.end());
    }

    void ParseQuery::whereKeyExists(std::string key) {
        Json dict;
        dict["$exists"] = 1;
        this->addWhereItemForKey(dict, key);
    }

    void ParseQuery::whereKeyDoesNotExist(std::string key) {
        Json dict;
        dict["$exists"] = 0;
        this->addWhereItemForKey(dict, key);
    }

    void ParseQuery::whereKeyEqualTo(std::string key, const Json &object) {
//        Json dict=Json::object();
//        dict.emplace("$eq",object);
        this->addWhereItemForKey(object, key);
    }

    void ParseQuery::whereKeyEqualTo(std::string key, ParseObject *object) {
        Json dict;
        Json objectDict;
        objectDict["__type"] = "Pointer";
        objectDict["className"] = object->className;
        if (object->hasValidObjectId()) {
            objectDict["objectId"] = object->objectId;
        }

        dict["$eq"] = objectDict;
        this->addWhereItemForKey(dict, key);
    }

    void ParseQuery::whereKeyLessThan(std::string key, Json const &object) {
        Json dict;
        dict["$lt"] = object;
        this->addWhereItemForKey(dict, key);
    }

    void ParseQuery::whereKeyLessThanOrEqualTo(std::string key,
                                               Json const &object) {
        Json dict;
        dict["$lte"] = object;
        this->addWhereItemForKey(dict, key);
    }

    void ParseQuery::whereKeyGreaterThan(std::string key, Json const &object) {
        Json dict;
        dict["$gt"] = object;
        this->addWhereItemForKey(dict, key);
    }

    void ParseQuery::whereKeyGreaterThanOrEqualTo(std::string key,
                                                  Json const &object) {
        Json dict;
        dict["$gte"] = object;
        this->addWhereItemForKey(dict, key);
    }

    void ParseQuery::whereKeyNotEqualTo(std::string key, Json const &object) {
        Json dict;
        dict["$ne"] = object;
        this->addWhereItemForKey(dict, key);
    }

    void ParseQuery::whereKeyContainedIn(std::string key,
                                         std::vector<Json> const &array) {
        Json jsonArray;
        for (auto &object : array) {
            jsonArray.push_back(object);
        }

        Json dict;
        dict["$in"] = jsonArray;

        this->addWhereItemForKey(dict, key);
    }

    void ParseQuery::whereKeyNotContainedIn(std::string key,
                                            std::vector<Json> const &array) {
        Json jsonArray;
        for (auto &object : array) {
            jsonArray.push_back(object);
        }

        Json dict;
        dict["$nin"] = jsonArray;

        this->addWhereItemForKey(dict, key);
    }

    void ParseQuery::whereKeyContainsAllObjectsInArray(std::string key,
                                                       std::vector<Json> const &array) {
        Json jsonArray;
        for (auto &object : array) {
            jsonArray.push_back(object);
        }

        Json dict;
        dict["$all"] = jsonArray;

        this->addWhereItemForKey(dict, key);
    }

    void ParseQuery::whereKeyNearGeoPoint(std::string key, ParseGeoPoint *geoPoint) {
        Json dict;
        dict["$nearSphere"] = geoPoint->toJson();
        this->addWhereItemForKey(dict, key);
    }

    void ParseQuery::whereKeyNearGeoPointWithinMiles(std::string key,
                                                     ParseGeoPoint *geoPoint,
                                                     double maxDistance) {
        Json dict;
        dict["$nearSphere"] = geoPoint->toJson();
        dict["$maxDistanceInMiles"] = maxDistance;
        this->addWhereItemForKey(dict, key);
    }

    void ParseQuery::whereKeyNearGeoPointWithinKilometers(std::string key,
                                                          ParseGeoPoint *geoPoint,
                                                          double maxDistance) {
        Json dict;
        dict["$nearSphere"] = geoPoint->toJson();
        dict["$maxDistanceInKilometers"] = maxDistance;
        this->addWhereItemForKey(dict, key);
    }

    void ParseQuery::whereKeyNearGeoPointWithinRadians(std::string key,
                                                       ParseGeoPoint *geoPoint,
                                                       double maxDistance) {
        Json dict;
        dict["$nearSphere"] = geoPoint->toJson();
        dict["$maxDistanceInRadians"] = maxDistance;
        this->addWhereItemForKey(dict, key);
    }
    //TODO $polygon

    void ParseQuery::whereKeyMatchesRegex(std::string key, std::string regex) {
        Json dict;
        dict["$regex"] = regex;
        this->addWhereItemForKey(dict, key);
    }

    void ParseQuery::whereKeyContainsString(std::string key, std::string substring) {
        this->whereKeyMatchesRegex(key,
                                   StringUtils::string_format(".*%s.*", substring));
    }

    void ParseQuery::whereKeyHasPrefix(std::string key, std::string prefix) {
        this->whereKeyMatchesRegex(key, StringUtils::string_format("^%s.*", prefix));
    }

    void ParseQuery::whereKeyHasSuffix(std::string key, std::string suffix) {
        this->whereKeyMatchesRegex(key, StringUtils::string_format(".*%s$", suffix));
    }

    void ParseQuery::whereKeySizeEqualTo(std::string key, int count) {
        Json dict;
        dict["$size"] = count;
        this->addWhereItemForKey(dict, key);
    }

    void ParseQuery::orderByAscending(std::string key) {
        this->order = key;
    }

    void ParseQuery::addAscendingOrder(std::string key) {
        if (this->order.length() <= 0) {
            this->orderByAscending(key);
            return;
        }

        this->order = StringUtils::string_format("%s,%s", this->order, key);
    }

    void ParseQuery::orderByDescending(std::string key) {
        this->order = StringUtils::string_format("-%s", key);
    }

    void ParseQuery::addDescendingOrder(std::string key) {
        if (this->order.length() <= 0) {
            this->orderByDescending(key);
            return;
        }

        this->order = StringUtils::string_format("%s,-%s", this->order, key);
    }

    ParseObject *ParseQuery::getObjectWithId(std::string objectId) {
        ParseObject *object = ParseObject::objectForClassName(this->className);
        object->objectId = objectId;
        object->fetch();

        return object;
    }

    std::vector<ParseObject *> ParseQuery::findObjects() {
        std::vector<ParseObject *> objects;

        std::string path = StringUtils::string_format("classes/%s", this->className);
        this->assembleParameters();

        ParsePaasClient::sharedInstance()->
                        getObject(path, this->parameters)
                .then([this](Json root) {
                    std::vector<ParseObject *> objects;
                    Json results = root["results"];
                    for (auto jsonValue : results) {
                        ParseObject *object = ParseObject::objectForClassName(this->className);

                        for (auto objIt = jsonValue.begin(); objIt != jsonValue.end(); ++objIt) {
                            std::string key = objIt.key();

                            if (key == "objectId") {
                                object->objectId = objIt.value();
                            } else if (key == "createdAt") {
                                object->createdAt = objIt.value();
                            } else if (key == "updatedAt") {
                                object->updatedAt = objIt.value();
                            }
                        }

                        object->localData = jsonValue;
                        object->localData.erase("objectId");
                        object->localData.erase("createdAt");
                        object->localData.erase("updatedAt");

                        objects.push_back(object);
                    }
                });

        return objects;
    }

    int ParseQuery::countObjects() {
        int count = 0;
        this->countObjectsInBackgroundWithCallback([&](int const &result, PCError const &error) {
            count = result;
        });

        return count;
    }

    void ParseQuery::countObjectsInBackgroundWithCallback(IntegerResultCallback callback) {
        std::string path = StringUtils::string_format("classes/%s", this->className);
        this->assembleParameters();

        this->parameters["limit"] = 0;
        this->parameters["count"] = 1;

        ParsePaasClient::sharedInstance()->
                        getObject(path, this->parameters)
                .then([](Json root) {
                    //TODO FIXME no count
                    int count = root["count"];


                });
    }


    void ParseQuery::addWhereItemForKey(Json const &dict, std::string key) {
        if (dict.find("$eq") != dict.end()) {
            if (this->where.find("$and") != this->where.end()) {
                Json eqDict;
                eqDict[key] = dict["$eq"];

                this->where["$and"].push_back(eqDict);
            } else {
                Json eqArray;

                Json eqDict;
                //FIXME getQuery organization
                eqDict[key] = dict;

                eqArray.push_back(eqDict);
                this->where["$and"] = eqArray;
            }
        } else {
            if (this->where.find(key) != this->where.end()) {
                this->where[key].push_back(dict);
            } else {
                this->where[key] = dict;
            }
        }
    }

    void ParseQuery::assembleParameters() {
        this->parameters = nullptr;
        if (!this->where.empty()) {
            this->parameters["where"] = this->where;
        }

        if (this->limit > 0) {
            this->parameters["limit"] = this->limit;
        }

        if (this->skip > 0) {
            this->parameters["skip"] = this->skip;
        }

        if (this->order.length() > 0) {
            this->parameters["order"] = this->order;
        }

        if (!this->include.empty()) {
            std::string includeStr;

            for (auto &item : this->include) {
                includeStr.append(item);
                includeStr.append(",");
            }

            this->parameters["include"] = includeStr;
        }

        if (!this->selectedKeys.empty()) {
            Json selectedKeysArray;
            for (auto &item : this->include) {
                selectedKeysArray.push_back(item);
            }

            this->parameters["keys"] = selectedKeysArray;
        }
    }

    void ParseQuery::setWhere(Json where) {
        this->where = std::move(where);
    }

NS_PC_END
