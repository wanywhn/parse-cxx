cmake_minimum_required(VERSION 3.11)
#include_directories(
#  ${ParseCloud_SOURCE_DIR}/src
#)
set(CMAKE_CXX_STANDARD 11)

  set(
    TESTS
    PaasClientTest
    ObjectTest
    QueryTest
    GeoPointTest
    FileTest
    UserTest
    ACLTest
    )

add_definitions( -DAPPID="abc" -DAPPKEY="abc" -DSERVER_URL="http://144.202.116.91:1337/parse" -DOBJID="NhzgIpesVa")
  foreach(test ${TESTS})
    if (${CMAKE_CXX_COMPILER_ID} MATCHES GNU)
      set_source_files_properties(${test}.cpp
        PROPERTIES COMPILE_FLAGS "-Wall")
    endif()

    add_executable(ParseCloud-${test}  TestMain.cpp ${test}.cpp)
    add_dependencies(ParseCloud-${test} parsecloud)
    target_link_libraries(ParseCloud-${test}
    PUBLIC
#            ${LINK_INTERFACE_LIBRARIES}
            CONAN_PKG::Catch2
            PRIVATE
        CONAN_PKG::cpprestsdk
        CONAN_PKG::websocketpp
        CONAN_PKG::jsonformoderncpp
            parsecloud )

    set_target_properties(ParseCloud-${test}
      PROPERTIES RUNTIME_OUTPUT_DIRECTORY ${ParseCloud_BINARY_DIR}/bin)
    add_test(ParseCloud-${test}
      ${ParseCloud_BINARY_DIR}/bin/ParseCloud-${test})
  endforeach()

