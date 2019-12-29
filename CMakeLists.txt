cmake_minimum_required(VERSION 3.10)
#add_definitions(-DCMAKE_TOOLCHAIN_FILE=${CMAKE_BINARY_DIR}/conan_paths.cmake)
#set(CMAKE_TOOLCHAIN_FILE "/home/tender/workplace/git/vcpkg/scripts/buildsystems/vcpkg.cmake")
#add_definitions(-DCMAKE_TOOLCHAIN_FILE=${CMAKE_BINARY_DIR}/conan_paths.cmake)
#set(CMAKE_TOOLCHAIN_FILE '${CMAKE_BINARY_DIR}/conan_paths.cmake')
project(ParseCloud)
set(CMAKE_CXX_STANDARD 11)

if (${CMAKE_CXX_COMPILER_ID} MATCHES GNU)
  set (CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -std=c++11")
elseif (${CMAKE_CXX_COMPILER_ID} MATCHES Clang)
    set (CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -ftemplate-depth=256")
endif()

option(BUILD_CppTests "Only build TestCpp sample" ON)

add_definitions( -D_NO_ASYNCRTIMP )
add_compile_definitions(_NO_ASYNCRTIMP)
set(CMAKE_EXPORT_COMPILE_COMMANDS TRUE)


include(${CMAKE_BINARY_DIR}/conanbuildinfo.cmake)
conan_basic_setup(TARGETS)
#find_package(Threads)

# find OpenSSL, Threads, Boost, cppnetlib, jsoncpp
# set(Boost_USE_STATIC_LIBS ON)
#set(Boost_USE_MULTI_THREAD ON)

#find_package(Boost 1.69.0 REQUIRED
#  REQUIRED unit_test_framework
#        system
#        regex date_time thread filesystem
#  program_options chrono atomic log)



#if (Boost_FOUND)
  # include some dirs
#  if(BUILD_FOR_ARM)
#    include_directories(
#      ${CMAKE_CURRENT_SOURCE_DIR}/include
#      )
#  else()
#    include_directories(
#      ${CMAKE_CURRENT_SOURCE_DIR}/include
#      ${Boost_INCLUDE_DIRS}
#      ${CPPNETLIB_INLUDE_DIRS}
#      )
#  endif()


# ParseCloud library
add_subdirectory(src)

# ParseCloud headers
install(DIRECTORY include DESTINATION ${CMAKE_BINARY_DIR})

# cpp test
if (BUILD_CppTests)
    enable_testing()
    add_subdirectory(tests)
endif (BUILD_CppTests)
#endif()
