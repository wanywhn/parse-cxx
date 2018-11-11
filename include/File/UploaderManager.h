#ifndef INCLUDE_PCFILE_UPLOADERMANAGER_H_
#define INCLUDE_PCFILE_UPLOADERMANAGER_H_

#include <string>
#include <mutex>
#include <condition_variable>
#include "ParseFile.h"
#include "ParseConstants.h"
#include "Utils/ParsePlatformMacros.h"

NS_PC_BEGIN

class UploaderManager {
private:
  static UploaderManager* _instance;
  static std::mutex _lock;

private:
  UploaderManager() = default;

    void uploadFileToBucketWithTokenAndCallback(std::string bucket,
                                              std::string token,
                                              ParseFile* file,
                                              std::string key,
                                              BooleanResultCallback resultCallback);

  std::string generateQiniuMultipartBodyString(std::string bucket,
                                               std::string token,
                                               ParseFile* file,
                                               std::string key);

public:
  static UploaderManager* sharedInstance();
  static std::string generateRandomString(int length);
  static std::string generateQiniuKey();
  void uploadWithFileAndCallback(ParseFile *file,
                                 BooleanResultCallback resultCallback);
  void release();
};
    class Base64{
    private:
        std::string _base64_table;
        static const char base64_pad = '=';public:
        Base64()
        {
            _base64_table = "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789+/"; /*这是Base64编码使用的标准字典*/
        }
        /**
         * 这里必须是unsigned类型，否则编码中文的时候出错
         */
        std::string Encode(const unsigned char * str,int bytes);
        std::string Decode(const char *str,int bytes);
        void Debug(bool open = true);
    };

NS_PC_END

#endif  // INCLUDE_PCFILE_UPLOADERMANAGER_H_
