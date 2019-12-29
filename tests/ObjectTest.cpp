#include <string>
#include <vector>
#include "ParseCloud.h"
#include "Object/ParseObject.h"
#include "Object/ParseRelation.h"
#include <catch2/catch.hpp>

using namespace std;
using namespace parsecloud;


TEST_CASE("Set Object　Test", "[Object]") {
    ParseCloud::setApplicationConfig(SERVER_URL, APPID, APPKEY);
    ParseObject *object = ParseObject::objectForClassName("Post");
    object->setObjectForKey("myname", "name");
    object->setObjectForKey(22, "age");
    vector<string> keys = object->allKeys();
    CHECK(keys.size() == 2);

    object->release();
}

TEST_CASE("Remove Object Test", "[Object]") {
    ParseCloud::setApplicationConfig(SERVER_URL, APPID, APPKEY);
    ParseObject *object = ParseObject::objectForClassName("Post");
    object->setObjectForKey("myname", "name");
    object->setObjectForKey(22, "age");
    object->removeObjectForKey("age");
    vector<string> keys = object->allKeys();
    CHECK(keys.size() == 1);

    object->release();
}


TEST_CASE("SaveObjectInBackgroudWithCallback Test", "[Object]") {
    ParseCloud::setApplicationConfig(SERVER_URL, APPID, APPKEY);
    ParseObject *object = ParseObject::objectForClassName("Post");
    object->setObjectForKey("mname", "name");
    object->setObjectForKey(22, "age");
    object->saveInBackground()
            .then([](PCError error) {
                CHECK(error.code == 0);
            }).get();

    object->deleteInBackgroundWithCallback()
            .then([](PCError error) {
                CHECK(error.code == 0);
            });


}

TEST_CASE("UpdateObject Test", "[Object]") {
    ParseCloud::setApplicationConfig(SERVER_URL, APPID,
                                     APPKEY);
    ParseObject *object = ParseObject::objectForClassName("Post");
    object->objectId = OBJID;
    object->fetch();
    object->setObjectForKey("male", "gender");
    object->setObjectForKey("yahoo", "name");
    object->saveInBackground().then([object](PCError error) {
        CHECK(error.code == 0);
        object->release();
    });

}


TEST_CASE("SaveAllInBackgroudWithCallback Test", "[Object]") {
    ParseCloud::setApplicationConfig(SERVER_URL, APPID,
                                     APPKEY);
    std::vector<ParseObject *> objects;

    for (int i = 10; i < 12; ++i) {
        ParseObject *object = ParseObject::objectForClassName("Post");
        object->setObjectForKey("debug" + std::to_string(i), "name");
        object->setObjectForKey(i, "age");
        objects.push_back(object);
    }

    ParseObject::saveAllInBackgroundWithCallback(objects)
            .then([](std::vector<PCError> vecError) {
                for (auto item:vecError) {
                    CHECK(item.code == 0);
                }

            }).get();

    //TODO deleteAll
    for (auto &object:objects) {
        object->deleteInBackgroundWithCallback().then([](PCError error) {
            CHECK(error.code == 0);
        }).get();
        object->release();
    }
}

TEST_CASE("Object fetch Test", "[Object]") {
    ParseCloud::setApplicationConfig(SERVER_URL, APPID,
                                     APPKEY);
    ParseObject *object = ParseObject::objectForClassName("Post");
    object->objectId = OBJID;
    object->fetch().then([object](PCError error) {
        CHECK(error.code == 0);
        CHECK(object->objectForKey("age") == 22);
        object->release();
    });
}

TEST_CASE("Object PointerType Test", "[Object]") {
    ParseCloud::setApplicationConfig(SERVER_URL, APPID,
                                     APPKEY);
    ParseObject *object = ParseObject::objectForClassName("Post");
    ParseObject *student;
    object->objectId = OBJID;
    object->fetch().then([object, &student](PCError error) {
        CHECK(error.code == 0);
    }).get();

    student = ParseObject::objectForClassName("Student");
    student->setObjectForKey(object, "children");

    student->saveInBackground()
            .then([object, student](PCError error) {
                CHECK(error.code == 0);
                student->release();
                object->release();
            });


}

TEST_CASE("Object DeleteObject Test", "[Object]") {
    ParseCloud::setApplicationConfig(SERVER_URL, APPID, APPKEY);
    ParseObject *object = ParseObject::objectForClassName("Deleter");
    object->saveInBackground().then([object](PCError error) {
        CHECK(error.code == 0);
    }).get();
    object->deleteInBackgroundWithCallback()
            .then([object](PCError error) {
                CHECK(error.code == 0);
                object->release();
            });

}

TEST_CASE("Object GeoType Test", "[Object]") {
    ParseCloud::setApplicationConfig(SERVER_URL, APPID,
                                     APPKEY);

    ParseObject *object = ParseObject::objectForClassName("GeoType");
    ParseGeoPoint *point = ParseGeoPoint::geoPoint(39.9139, 116.3917);

    object->setObjectForKey(point, "location");
    object->saveInBackground()
            .then([](PCError error) {
                CHECK(error.code == 0);
            }).get();

    object->deleteInBackgroundWithCallback().then([](PCError error) {
        CHECK(error.code == 0);
    }).get();
    object->release();
}

TEST_CASE("Object AddRelationForKey Test", "[Object]") {
    ParseCloud::setApplicationConfig(SERVER_URL, APPID,
                                     APPKEY);

    ParseObject *student = ParseObject::objectForClassName("Student");
    student->setObjectForKey("myname", "name");
    student->saveInBackground().then([](PCError error) {
        CHECK(error.code == 0);
    }).get();

    ParseObject *post = ParseObject::objectForClassName("Post");
    post->objectId = OBJID;
    post->fetch().then([](PCError error) {
        CHECK(error.code == 0);
    }).get();

    student->addRelationForKey(post, "like");
    student->saveInBackground()
            .then([](PCError error) {
                CHECK(error.code == 0);
            }).get();


    student->deleteInBackgroundWithCallback().then([](PCError error) {
        CHECK(error.code == 0);
    }).get();

    student->release();
}

TEST_CASE("Object RemoveRelationForKey", "[Object]") {
    ParseCloud::setApplicationConfig(SERVER_URL, APPID,
                                     APPKEY);

    ParseObject *student = ParseObject::objectForClassName("Student");
    student->setObjectForKey("myname", "name");
    student->saveInBackground().then([](PCError error) {
        CHECK(error.code == 0);
    }).get();

    ParseObject *post = ParseObject::objectForClassName("Post");
    post->objectId = OBJID;

    student->addRelationForKey(post, "like");
    student->saveInBackground().then([](PCError error) {
        CHECK(error.code == 0);
    }).get();

    student->removeRelationForKey(post, "like");
    student->saveInBackground().then([](PCError error) {
        CHECK(error.code == 0);
    }).get();


    student->deleteInBackgroundWithCallback()
            .then([](PCError error) {
                CHECK(error.code == 0);
            }).get();

    post->release();
    student->release();
}
