#include <string>
#include <vector>
#include "ParseCloud.h"
#include "ACL/ParseACL.h"
#include "Object/ParseObject.h"
#include <catch/catch.hpp>

using namespace std;
using namespace parsecloud;


TEST_CASE("ACL SetReadAndWriteAccess Test", "[ACL]") {
    ParseCloud::setApplicationConfig(SERVER_URL,APPID,
            APPKEY);
    ParseObject *object = ParseObject::objectForClassName("Post");
    object->setObjectForKey("myname", "name");
    object->setObjectForKey(21, "age");

    ParseACL *acl = ParseACL::ACL();
    acl->setReadAccess(true, "12345");
    acl->setWriteAccess(true, "321");

    object->setACL(acl);
    object->saveInBackground();

    CHECK(object->objectId.length() > 0);

    object->release();
    acl->release();
}
