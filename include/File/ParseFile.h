
#ifndef INCLUDE_PCFILE_AVFILE_H_
#define INCLUDE_PCFILE_AVFILE_H_

#include <string>
#include <fstream>
#include "ParseConstants.h"
#include "Utils/ParsePlatformMacros.h"

NS_PC_BEGIN

class ParseFile {
private:
  ParseFile() {}
  static std::string objectPath(std::string objectId);
public:
  /**
   * \~english The name of the file
   *
   */
  std::string name;

  /**
   * \~english The local path of the file
   *
   */
  std::string path;

  /**
   * \~english The Remote Name of the file
   *
   */
  std::string remoteName;

  /**
   * \~english The id of the file. well the File Object don't have objectId
   *
   */
  std::string objectId;

  /**
   * \~english The url of the file.
   *
   */
  std::string url;

  /**
   * \~english The metadata of the file.
   *
   */
  Json metadata;

public:
  /**
   * \~english Creates a file with the local path of the file.
   *
   * \~english \param path The path of the file that will upload to AVOS Cloud.
   *
   * \~english \return an AVFile
   */
  static ParseFile* fileWithPath(std::string path);

  /**
   * \~english Creates a file with given url.
   *
   * \~english \param url The url of file.
   *
   * \~english \return an AVFile
   */
  static ParseFile* fileWithUrl(std::string url);

  /**
   * \~english Creates a file with given url.
   *
   * \~english \param objectId The objectId of file.
   *
   * \~english \return an AVFile
   */
  static ParseFile* fileWithObjectId(std::string objectId);

  /**
   * \~english release memory.
   *
   */
  void release();

  /**
   * \~english Update metadata of the file.
   *
   * \~english \return
   */
  Json updateMetadata();

  /**
   * \~english Saves the file.
   *
   */
  void saveInBackground();

  /**
   * \~english Saves the file and executes the given callback.
   *
   * \~english \param callback The callback should have the following argument signature: (bool const&, AVError const&)
   */
  void saveInBackgroundWithCallback(BooleanResultCallback callback);

  /**
   * \~english fetch file data from server, and save it into local path.
   *
   * \~english \param path The local path of file.
   */
  void fetchFileDataIntoPath(std::string path);

  //TODO to complete
  /**
   * \~english fetch the file into local path and show the progress
   * @param path
   * @param callback
   */
  void fetchFileDataIntoPath(std::string path,ProgressCallback callback);

  /**
   * \~english Delete the file.
   *
   */
  void deleteInBackground();
};

NS_PC_END

#endif  // INCLUDE_PCFILE_AVFILE_H_
