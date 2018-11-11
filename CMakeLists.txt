cmake_minimum_required(VERSION 3.10)
project(ParseCloud)
set(CMAKE_CXX_STANDARD 11)

    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -std=c++11")

option(BUILD_CppTests "Only build TestCpp sample" ON)

find_package(spdlog)

# nlohmann-json-dev
#set(nlohmann_json_DIR "/usr/lib/cmake")
find_package(CURL REQUIRED)

# well ,these packets in system's repo are too old to use
include_directories("./lib/restclient-cpp/include/")
include_directories("./lib/json/include/nlohmann")
include_directories("./lib/json/include/")


    include_directories(
            ${CMAKE_CURRENT_SOURCE_DIR}/include

    )
add_subdirectory(lib/json)

# ParseCloud library
add_subdirectory(src)

# ParseCloud headers
install(DIRECTORY include DESTINATION ${CMAKE_BINARY_DIR})

# cpp test
if (BUILD_CppTests)
    enable_testing()
    add_subdirectory(tests)
endif (BUILD_CppTests)
