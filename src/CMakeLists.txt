set(LIB_NAME parsecloud)

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

add_library(${LIB_NAME} STATIC
  ${ParseCloud_SRC}
  ${ParseCloud_ACL_SRC}
  ${ParseCloud_Object_SRC}
  ${ParseCloud_File_SRC}
  ${ParseCloud_Geo_SRC}
  ${ParseCloud_Query_SRC}
  ${ParseCloud_Request_SRC}
  ${ParseCloud_User_SRC}
  ${ParseCloud_Utils_SRC}
#        ${RestClient_SRC}
  )
target_include_directories(${LIB_NAME} PUBLIC ${CMAKE_SOURCE_DIR}/include)


target_link_libraries(${LIB_NAME} PUBLIC
        CONAN_PKG::jsonformoderncpp
        CONAN_PKG::spdlog
        CONAN_PKG::Catch2

        CONAN_PKG::boost_format
#        CONAN_PKG::boost_system
        CONAN_PKG::boost_regex
        CONAN_PKG::boost_program_options
        CONAN_PKG::cpprestsdk
  )
set_target_properties(${LIB_NAME}
  PROPERTIES
  ARCHIVE_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/lib"
  LIBRARY_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/lib"
  RUNTIME_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/bin"
  )
