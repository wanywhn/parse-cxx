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

add_definitions( -DAPPID="myAppId" -DAPPKEY="restAPIKey" -DSERVER_URL="http://localhost:1337/parse")
  foreach(test ${TESTS})
    if (${CMAKE_CXX_COMPILER_ID} MATCHES GNU)
      set_source_files_properties(${test}.cpp
        PROPERTIES COMPILE_FLAGS "-Wall")
    endif()

    add_executable(ParseCloud-${test}  TestMain.cpp ${test}.cpp)
    add_dependencies(ParseCloud-${test} parsecloud)
    target_link_libraries(ParseCloud-${test} ${LINK_INTERFACE_LIBRARIES} parsecloud boost_system )

    set_target_properties(ParseCloud-${test}
      PROPERTIES RUNTIME_OUTPUT_DIRECTORY ${ParseCloud_BINARY_DIR}/bin)
    add_test(ParseCloud-${test}
      ${ParseCloud_BINARY_DIR}/bin/ParseCloud-${test})
  endforeach()

