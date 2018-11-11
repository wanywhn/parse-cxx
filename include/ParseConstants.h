#ifndef INCLUDE_PARSECONSTANTS_H_
#define INCLUDE_PARSECONSTANTS_H_

#include <functional>
#include <string>
#include <vector>
#include <json.hpp>
#include "Utils/ParsePlatformMacros.h"
#include "Utils/ParseErrorUtils.h"

NS_PC_BEGIN


using Json=nlohmann::json ;

class ParseObject;
class ParseUser;

//TODO to complete
enum class CachePolicy {
    CACHE_ELSE_NETWORK,
    //The query first tries to load from the cache, but if that fails, it loads results from the network.
    CACHE_ONLY,
    //The query only loads from the cache, ignoring the network.
    CACHE_THEN_NETWORK,
    //The query first loads from the cache, then loads from the network.
    IGNORE_CACHE,
    //The query does not load from the cache or save results to the cache.
    NETWORK_ELSE_CACHE,
    //The query first tries to load from the network, but if that fails, it loads results from the cache.
    NETWORK_ONLY,
    //The query does not load from the cache, but it will save results to the cache.
};

/// A user object without a valid session could not be altered.
const int SessionMissing=206;
/// The password is missing or empty.
const int PasswordMissing=201;



typedef std::function<void(bool const&, PCError const&)> BooleanResultCallback;
typedef std::function<void(int const&, PCError const&)> IntegerResultCallback;
typedef std::function<void(std::istream const&, PCError const&)> DataStreamResultCallback;
typedef std::function<void(std::string const&, PCError const&)> StringResultCallback;
typedef std::function<void(int const&)> ProgressCallback;
typedef std::function<void(Json const&, PCError const&)> IdResultCallback;
typedef std::function<void(std::vector<ParseObject> const&, PCError const&)> VectorResultCallback;
typedef std::function<void(ParseObject const&, PCError const&)> ObjectResultCallback;
typedef std::function<void(ParseUser const&, PCError const&)> UserResultCallback;


NS_PC_END

#endif  // INCLUDE_PARSECONSTANTS_H_
