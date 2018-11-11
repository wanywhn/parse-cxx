#include <utility>

#ifndef INCLUDE_UTILS_PCERRORUTILS_H_
#define INCLUDE_UTILS_PCERRORUTILS_H_

#include <string>
#include <unordered_map>
#include <nlohmann/json.hpp>
#include "Utils/ParsePlatformMacros.h"

using Json=nlohmann::json ;
NS_PC_BEGIN

extern std::string const kErrorDomain;
extern std::string const kErrorUnknownText;

struct PCError {
  std::string domain;
  int code;
  std::unordered_map<std::string, std::string> userInfo;

 PCError()
 :domain(), code(), userInfo()
  { }

 PCError(std::string _domain, int const & _code)
 :domain(std::move(_domain)), code(_code)
  { }

 PCError(std::string const & _domain, int const & _code, std::unordered_map<std::string, std::string> const & _userInfo)
 :domain(_domain), code(_code), userInfo(_userInfo)
  { }
};

class ParseErrorUtils {
public:
  static PCError errorWithCode(int const & code);
  static PCError errorWithCode(int const & code, std::string const & errorText);
  static PCError internalServerError();
  static PCError errorFromJSON(Json json);
  static std::string errorTextFromError(PCError & error);

private:
  static bool _isDictionaryError(Json const & json);
  static PCError _errorFromDictionary(Json const & json);
};


NS_PC_END

#endif  // INCLUDE_UTILS_PCERRORUTILS_H_
