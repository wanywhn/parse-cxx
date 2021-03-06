#include <iostream>
#include <string>
#include <vector>
#include <catch2/catch.hpp>
#include "ParseCloud.h"
#include "Object/ParseObject.h"
#include "Query/ParseQuery.h"

using namespace std;
using namespace parsecloud;

TEST_CASE("CreateObject", "[Object]") {
    ParseCloud::setApplicationConfig(SERVER_URL, APPID, APPKEY);
    auto student = ParseObject::objectForClassName("Student");
    student->setObjectForKey(22, "age");
    student->setObjectForKey("myname", "name");
    student->saveInBackground()
            .then([student](const PCError& error) {
                CHECK(error.code == 0);
                REQUIRE(!student->objectId.empty());
            }).get();
}

TEST_CASE("FindObjects", "[Query]") {
    ParseCloud::setApplicationConfig(SERVER_URL, APPID, APPKEY);
    ParseQuery *query = ParseQuery::queryForClassName("Student");
    query->whereKeyEqualTo("age", 22);
    query->limit = 10;
    query->skip = 1;
    query->findObjects()
            .then([query](std::vector<ParseObject *> objects) {
                CHECK(!objects.empty());
                query->release();

            }).get();

}


TEST_CASE("CountObjects", "[Query]") {
    ParseCloud::setApplicationConfig(SERVER_URL, APPID, APPKEY);
    ParseQuery *query = ParseQuery::queryForClassName("Student");
    query->whereKeyEqualTo("age", 22);
    query->limit = 10;
    query->skip = 1;

    query->countObjects().then([query](const std::pair<PCError, int> &pair) {
        CHECK(pair.first.code == 0);

        CHECK(pair.second != -1);

        query->release();
    }).get();

}

TEST_CASE("OrSubQueries", "[Query]") {
    ParseCloud::setApplicationConfig(SERVER_URL, APPID, APPKEY);

    ParseQuery *query1 = ParseQuery::queryForClassName("Student");
    query1->whereKeyEqualTo("age", 22);
    query1->limit = 10;
    query1->skip = 1;

    ParseQuery *query2 = ParseQuery::queryForClassName("Student");
    query2->whereKeyEqualTo("name", "myname");
    query2->limit = 10;
    query2->skip = 1;

    std::vector<ParseQuery *> queries{query1, query2};
    ParseQuery *query3 = ParseQuery::orQueryWithSubqueries(queries);

    query3->findObjects()
            .then([query1, query2, query3](const std::vector<ParseObject *> &objects) {
                CHECK(!objects.empty());
                query1->release();
                query2->release();
                query3->release();

            }).get();
}

TEST_CASE("AndSubQueries", "[Query]") {
    ParseCloud::setApplicationConfig(SERVER_URL, APPID, APPKEY);

    ParseQuery *query1 = ParseQuery::queryForClassName("Student");
    query1->whereKeyEqualTo("age", 22);
    query1->limit = 10;
    query1->skip = 1;

    ParseQuery *query2 = ParseQuery::queryForClassName("Student");
    query2->whereKeyEqualTo("name", "myname");
    query2->limit = 10;
    query2->skip = 1;

    std::vector<ParseQuery *> queries{query1, query2};
    ParseQuery *query3 = ParseQuery::andQueryWithSubqueries(queries);

    query3->findObjects()
            .then([query1, query2, query3](const std::vector<ParseObject *> &objects) {
                CHECK(!objects.empty());
                query1->release();
                query2->release();
                query3->release();

            }).get();
}
