#include "File/ParseFile.h"

#include <string>
#include <exception>
#include "File/UploaderManager.h"
#include "Request/ParsePaasClient.h"

NS_PC_BEGIN

const std::string ownerTag = "owner";
const std::string fileSizeTag = "size";
const std::string fileMd5Tag = "_checksum";

ParseFile* ParseFile::fileWithPath(std::string path) {
  auto * file = new ParseFile();
  file->path = path;
  file->name = StringUtils::getFileNameFromPath(path);

  return file;
}

ParseFile* ParseFile::fileWithUrl(std::string url) {
  auto * file = new ParseFile();
  file->url = url;
  file->name = StringUtils::getFileNameFromPath(url);

  return file;
}

ParseFile* ParseFile::fileWithObjectId(std::string objectId) {
  auto * file = new ParseFile();
  file->objectId = objectId;

  return file;
}

void ParseFile::release() {
  PC_SAFE_DELETE(this);
}

Json ParseFile::updateMetadata() {
 // if (this->metadata.find(ownerTag)!=this->metadata.end()) {
 //   // Objective-C code is here
 //   // NSString * objectId = [ParsePaasClient sharedInstance].currentUser.objectId;
 //   // if (objectId.length > 0) {
 //   //     [self.metadata setObject:objectId forKey:ownerTag];
 //   // }
 // }

 // if (this->metadata.find(fileSizeTag)!=this->metadata.end()) {
 //   std::ifstream is(this->path, std::ifstream::binary);
 //   if (is) {
 //     is.seekg(0, is.end);
 //     int length = is.tellg();
 //     is.seekg(0, is.beg);
 //     is.close();

 //     if (length > 0) {
 //       this->metadata[fileSizeTag] = length;
 //     }
 //   }
 // }

 // if (this->metadata.find(fileMd5Tag)!=this->metadata.end()) {
 //   if (this->path.length() > 0) {
 //     std::string md5 = StringUtils::AVMD5String(this->path);
 //     if (md5.length() > 0) {
 //       this->metadata[fileMd5Tag] = md5;
 //     }
 //   }
 // }

  return this->metadata;
}

void ParseFile::saveInBackground() {
  BooleanResultCallback callback;
  this->saveInBackgroundWithCallback(callback);
}

void ParseFile::saveInBackgroundWithCallback(BooleanResultCallback callback) {
  UploaderManager::sharedInstance()->
          uploadWithFileAndCallback(this,
                                    [&](bool const &succeeded, PCError const &error) {
                                        if (!succeeded) {
                                          this->deleteInBackground();
                                        }

                                        callback(succeeded, error);
                                    });
}

void ParseFile::fetchFileDataIntoPath(std::string path) {
  if (this->url.length() > 0) {
    ParsePaasClient::sharedInstance()->fetchFileDataIntoPathWithUrl(path,
                                                                 this->url);
  }
}

void ParseFile::deleteInBackground() {
  if (this->objectId.length() > 0) {
    Json parameters;

    ParsePaasClient::sharedInstance()->
        deleteObject(ParseFile::objectPath(this->objectId),
                     parameters,
                     [&](Json const & root, PCError const & error) {
                     });
  }
}

std::string ParseFile::objectPath(std::string objectId) {
  if (objectId.length() > 0) {
    return StringUtils::string_format("classes/files/%s", objectId);
  }

  return "classes/files";
}


NS_PC_END
