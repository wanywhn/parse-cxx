
#include "ACL/ParseRole.h"
#include <string>
#include <vector>

#include "ACL/ParseACL.h"
#include "Object/ParseRelation.h"
#include "Query/ParseQuery.h"

NS_PC_BEGIN

ParseRole::ParseRole():name() {
  this->className = "roles";
}

ParseRole* ParseRole::roleWithName(std::string name) {
  auto * role = new ParseRole();
  role->name = name;

  return role;
}

ParseRole* ParseRole::roleWithNameAndACL(std::string name, ParseACL* acl) {
  auto * role = new ParseRole();
  role->name = name;
  role->setACL(acl);

  return role;
}

void ParseRole::release() {
  PC_SAFE_DELETE(this);
}

ParseRelation* ParseRole::getUsers() {
  std::string const usersTag = "users";
  std::vector<ParseObject*> objects = this->addedRelationData[usersTag];
  ParseObject* target = nullptr;

  if (!objects.empty()) {
    target = objects[0];
  }

  auto * relation = new ParseRelation();
  relation->parent = this;
  relation->key = usersTag;

  if (target != nullptr) {
    relation->targetClass = target->className;
  }

  return relation;
}

ParseRelation* ParseRole::getRoles() {
  return nullptr;
}

ParseQuery* ParseRole::getQuery() {
  ParseQuery* query = ParseQuery::queryForClassName("roles");
  return query;
}

NS_PC_END
