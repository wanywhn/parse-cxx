#include <string>
#include <vector>
#include "ParseCloud.h"
#include "Object/ParseObject.h"
#include "Object/ParseRelation.h"
#include <catch/catch.hpp>

using namespace std;
using namespace parsecloud;


TEST_CASE("Set Object　Test", "[Object]") {
    ParseCloud::setApplicationConfig(SERVER_URL, APPID,
                                     APPKEY);
    ParseObject *object = ParseObject::objectForClassName("Post");
    object->setObjectForKey("myname", "name");
    object->setObjectForKey(22, "age");
    vector<string> keys = object->allKeys();
    CHECK(keys.size() == 2);

    object->release();
}

TEST_CASE("Remove Object Test", "[Object]") {
    ParseCloud::setApplicationConfig(SERVER_URL, APPID,
                                     APPKEY);
    ParseObject *object = ParseObject::objectForClassName("Post");
    object->setObjectForKey("myname", "name");
    object->setObjectForKey(22, "age");
    object->removeObjectForKey("age");
    vector<string> keys = object->allKeys();
    CHECK(keys.size() == 1);

    object->release();
}


TEST_CASE("SaveInBackgroud Test", "[Object]") {
    ParseCloud::setApplicationConfig(SERVER_URL, APPID,
                                     APPKEY);
    ParseObject *object = ParseObject::objectForClassName("Post");
    object->setObjectForKey("myname", "name");
    object->setObjectForKey(22, "age");

    object->saveInBackground();
    CHECK(object->objectId.length() > 0);
    object->deleteInBackground();

    object->release();
}

TEST_CASE("SaveObjectInBackgroudWithCallback Test", "[Object]") {
    ParseCloud::setApplicationConfig(SERVER_URL, APPID,
                                     APPKEY);
    ParseObject *object = ParseObject::objectForClassName("Post");
    object->setObjectForKey("myname", "name");
    object->setObjectForKey(22, "age");

    object->saveInBackgroundWithCallback([&](bool const &succeeded, PCError const &error) {
        CHECK(succeeded);
    });
    object->deleteInBackground();

    object->release();
}

TEST_CASE("UpdateObject Test", "[Object]") {
    ParseCloud::setApplicationConfig(SERVER_URL, APPID,
                                     APPKEY);
    ParseObject *object = ParseObject::objectForClassName("Post");
    object->objectId = "D9MgXhJm58";
    object->fetch();
    object->setObjectForKey("male", "gender");
    object->setObjectForKey("yahoo", "name");
    object->saveInBackgroundWithCallback([&](bool const &succeeded, PCError const &error) {
        CHECK(succeeded);
    });
    object->release();
}

TEST_CASE("SaveAllInBackgroud Test", "[Object]") {
    ParseCloud::setApplicationConfig(SERVER_URL, APPID,
                                     APPKEY);
    std::vector<ParseObject *> objects;

    for (int i = 0; i < 2; ++i) {
        ParseObject *object = ParseObject::objectForClassName("Post");
        object->setObjectForKey("myname", "name");
        object->setObjectForKey(i, "age");
        objects.push_back(object);
    }

    ParseObject::saveAllInBackground(objects);

    for (auto &object:objects) {
        CHECK(object->objectId.length() > 0);
        object->deleteInBackground();
        object->release();
    }
}

TEST_CASE("SaveAllInBackgroudWithCallback Test", "[Object]") {
    ParseCloud::setApplicationConfig(SERVER_URL, APPID,
                                     APPKEY);
    std::vector<ParseObject *> objects;

    for (int i = 10; i < 12; ++i) {
        ParseObject *object = ParseObject::objectForClassName("Post");
        object->setObjectForKey("debug"+std::to_string(i), "name");
        object->setObjectForKey(i, "age");
        objects.push_back(object);
    }

    ParseObject::saveAllInBackgroundWithCallback(objects, [&](bool const &succeeded, PCError const &error) {
        CHECK(succeeded);
    });

    for (auto &object:objects) {
        object->deleteInBackground();
        object->release();
    }
}

TEST_CASE("Object fetch Test", "[Object]") {
    ParseCloud::setApplicationConfig(SERVER_URL, APPID,
                                     APPKEY);
    ParseObject *object = ParseObject::objectForClassName("Post");
    object->objectId = "D9MgXhJm58";
    object->fetch();
    CHECK(object->objectForKey("age") == 22);

    object->release();
}

TEST_CASE("Object PointerType Test", "[Object]") {
    ParseCloud::setApplicationConfig(SERVER_URL, APPID,
                                     APPKEY);
    ParseObject *object = ParseObject::objectForClassName("Post");
    object->objectId = "D9MgXhJm58";
    object->fetch();

    ParseObject *student = ParseObject::objectForClassName("Student");
    student->setObjectForKey(object, "children");

    student->saveInBackgroundWithCallback([&](bool const &succeeded, PCError const &error) {
        CHECK(succeeded);
    });

    student->release();
}

TEST_CASE("Object DeleteObject Test", "[Object]") {
    ParseCloud::setApplicationConfig(SERVER_URL, APPID,
                                     APPKEY);
    ParseObject *object = ParseObject::objectForClassName("Deleter");
    object->saveInBackground();

    object->deleteInBackgroundWithCallback([&](bool const &succeeded, PCError const &error) {
        CHECK(succeeded);
    });

    object->release();
}

TEST_CASE("Object GeoType Test", "[Object]") {
    ParseCloud::setApplicationConfig(SERVER_URL, APPID,
                                     APPKEY);

    ParseObject *object = ParseObject::objectForClassName("GeoType");
    ParseGeoPoint *point = ParseGeoPoint::geoPoint(39.9139, 116.3917);

    object->setObjectForKey(point, "location");
    object->saveInBackgroundWithCallback([&](bool const &succeeded, PCError const &error) {
        CHECK(succeeded);
    });

    object->deleteInBackground();
    object->release();
}

TEST_CASE("Object AddRelationForKey Test", "[Object]") {
    ParseCloud::setApplicationConfig(SERVER_URL, APPID,
                                     APPKEY);

    ParseObject *student = ParseObject::objectForClassName("Student");
    student->setObjectForKey("myname", "name");
    student->saveInBackground();

    ParseObject *post = ParseObject::objectForClassName("Post");
    post->objectId = "D9MgXhJm58";
    post->fetch();

    student->addRelationForKey(post, "like");

    student->saveInBackgroundWithCallback([&](bool const &succeeded, PCError const &error) {
        CHECK(succeeded);
    });

    student->deleteInBackground();

    student->release();
}

TEST_CASE("Object RemoveRelationForKey", "[Object]") {
    ParseCloud::setApplicationConfig(SERVER_URL, APPID,
                                     APPKEY);

    ParseObject *student = ParseObject::objectForClassName("Student");
    student->setObjectForKey("myname", "name");
    student->saveInBackground();

    ParseObject *post = ParseObject::objectForClassName("Post");
    post->objectId = "D9MgXhJm58";

    student->addRelationForKey(post, "like");

    student->saveInBackgroundWithCallback([&](bool const &succeeded, PCError const &error) {
        CHECK(succeeded);
    });

    student->removeRelationForKey(post, "like");

    student->saveInBackgroundWithCallback([&](bool const &succeeded, PCError const &error) {
        CHECK(succeeded);
    });

    student->deleteInBackground();

    post->release();
    student->release();
}
