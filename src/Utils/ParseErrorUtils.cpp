#include "Utils/ParseErrorUtils.h"

#include <string>
#include <spdlog/spdlog.h>
#include <iostream>
#include "ParseConstants.h"

NS_PC_BEGIN

std::string const kErrorDomain ="Parse Cloud Error Domain";
std::string const kErrorUnknownText = "Error Infomation Unknown";

PCError ParseErrorUtils::errorWithCode(int const & code) {
  return PCError(kErrorDomain, code);
}

PCError ParseErrorUtils::errorWithCode(int const & code,
                                    std::string const & errorText) {
  std::unordered_map<std::string, std::string> userInfo{{"error", errorText}};
  return PCError(kErrorDomain, code, userInfo);
}

PCError ParseErrorUtils::internalServerError() {
  return PCError(kErrorDomain,500);
}

//!!TODO refac
PCError ParseErrorUtils::errorFromJSON(Json json) {
  PCError returnError;
  if (json.type() == Json::value_t::null) {
    returnError = ParseErrorUtils::internalServerError();
  } else if (json.type() == Json::value_t::object) {
    if (ParseErrorUtils::_isDictionaryError(json)) {
      returnError = ParseErrorUtils::_errorFromDictionary(json);
    } else {
      for (const auto &child : json) {
        if (child.type() == Json::value_t::object&& ParseErrorUtils::_isDictionaryError(child)) {
          returnError = ParseErrorUtils::_errorFromDictionary(child);
          break;
        }
      }
    }
  } else if (json.type() == Json::value_t::array) {
    for (const auto &child : json) {
      returnError = ParseErrorUtils::errorFromJSON(child);
      if (returnError.domain.length() > 0) {
        break;
      }
    }
  }else if(json.is_discarded()){
    returnError=PCError("UNKNOW",1);

  }

  return returnError;
}

std::string ParseErrorUtils::errorTextFromError(PCError & error) {
  return error.userInfo["error"];
}

PCError ParseErrorUtils::_errorFromDictionary(Json const & json) {
  std::string erroString = json["error"];
  int code = json["code"];

  if (erroString.length() == 0) {
    erroString = kErrorUnknownText;
  }

  return ParseErrorUtils::errorWithCode(code, erroString);
}

bool ParseErrorUtils::_isDictionaryError(Json const & json) {
  if (json.find("code")!=json.end()) {
    return true;
  }

  return json.find("error") != json.end();

}

NS_PC_END
