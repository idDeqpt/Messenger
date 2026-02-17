cmake_minimum_required(VERSION 3.5.0)

set(PROJECT_NAME messenger_server)

set(BUILD_SHARED_LIBS OFF CACHE BOOL "Build shared libraries" FORCE)
set(DEPS_DIR "${CMAKE_SOURCE_DIR}/deps" CACHE PATH "Path to dependencies")
set(CMAKE_MODULE_PATH "${CMAKE_CURRENT_SOURCE_DIR}/cmake/modules" ${CMAKE_MODULE_PATH})

project(${PROJECT_NAME} CXX)

include(FetchContent)

FetchContent_Declare(
	network
	GIT_REPOSITORY "https://github.com/idDeqpt/Network.git"
	GIT_TAG main
)
FetchContent_MakeAvailable(network)

FetchContent_Declare(
	JSTypes
	GIT_REPOSITORY "https://github.com/idDeqpt/JSTypes.git"
	GIT_TAG main
)
FetchContent_MakeAvailable(JSTypes)

find_package(SQLite3 REQUIRED)


add_executable(${PROJECT_NAME} src/main.cpp)


target_link_libraries(${PROJECT_NAME} PRIVATE
	SQLite3::SQLite3
	network
	JSTypes
)

target_compile_features(${PROJECT_NAME} PRIVATE cxx_std_17)

set(EXECUTABLE_OUTPUT_PATH ${CMAKE_CURRENT_SOURCE_DIR}/bin)