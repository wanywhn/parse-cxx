set(ParseCloud_SRC
        ParseCloud.cpp
  )

include(ACL/CMakeLists.txt)
include(File/CMakeLists.txt)
include(Object/CMakeLists.txt)
include(Geo/CMakeLists.txt)
include(Query/CMakeLists.txt)
include(Request/CMakeLists.txt)
include(User/CMakeLists.txt)
include(Utils/CMakeLists.txt)

set(RestClient_SRC
        ../lib/restclient-cpp/source/connection.cc
        ../lib/restclient-cpp/source/helpers.cc
        ../lib/restclient-cpp/source/restclient.cc )

add_library(parsecloud SHARED
  ${ParseCloud_SRC}
  ${ParseCloud_ACL_SRC}
  ${ParseCloud_Object_SRC}
  ${ParseCloud_File_SRC}
  ${ParseCloud_Geo_SRC}
  ${ParseCloud_Query_SRC}
  ${ParseCloud_Request_SRC}
  ${ParseCloud_User_SRC}
  ${ParseCloud_Utils_SRC}
        ${RestClient_SRC}
  )


target_link_libraries( parsecloud PRIVATE
        ${CURL_LIBRARIES}
        nlohmann_json::nlohmann_json
  )

set_target_properties( parsecloud
  PROPERTIES
  ARCHIVE_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/lib"
  LIBRARY_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/lib"
  RUNTIME_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/bin"
  )
