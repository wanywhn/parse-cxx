#include <utility>
#include <string>

#include "Object/ParseRelation.h"


NS_PC_BEGIN

ParseQuery* ParseRelation::query() {
  Json dict;
  Json relatedToDict;
  Json objectDict;
  objectDict["__type"] = "Pointer";
  objectDict["className"] = this->parent->className;
  if (this->parent->hasValidObjectId()) {
    objectDict["objectId"] = this->parent->objectId;
  }

  relatedToDict["object"] = objectDict;
  relatedToDict["key"] = this->key;
  dict["$relatedTo"] = relatedToDict;

  ParseQuery* query = ParseQuery::queryForClassName(this->targetClass);
  query->setWhere(dict);

  return query;
}

void ParseRelation::release() {
  PC_SAFE_DELETE(this);
}

void ParseRelation::addObject(ParseObject* object) {
  if (object->hasValidObjectId()) {
    this->targetClass = object->className;
    this->parent->addRelationForKey(object, this->key);
  }
}

void ParseRelation::removeObject(ParseObject* object) {
  this->parent->removeRelationForKey(object, this->key);
}

ParseQuery* ParseRelation::reverseQueryWithRelationKeyAndChildObject(std::string parentClassName,
                                                               std::string relationKey,
                                                               ParseObject* child) {
  Json objectDict;
  objectDict["__type"] = "Pointer";
  objectDict["className"] = child->className;
  if (child->hasValidObjectId()) {
    objectDict["objectId"] = child->objectId;
  }

  ParseQuery* query = ParseQuery::queryForClassName(std::move(parentClassName));
  query->setWhere(objectDict);

  return query;
}

NS_PC_END
