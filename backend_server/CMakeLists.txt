cmake_minimum_required(VERSION 3.14)

set(PROJECT_NAME MessengerAPI)

set(BUILD_SHARED_LIBS OFF CACHE BOOL "Build shared libraries" FORCE)
set(DEPS_DIR "${CMAKE_SOURCE_DIR}/deps" CACHE PATH "Path to dependencies")
set(CMAKE_MODULE_PATH "${CMAKE_CURRENT_SOURCE_DIR}/cmake/modules" ${CMAKE_MODULE_PATH})

project(${PROJECT_NAME} CXX)


include(FetchContent)

FetchContent_Declare(
	network
	GIT_REPOSITORY "https://github.com/idDeqpt/Network.git"
	GIT_TAG fc123e0a9f935772ae6928872e45a4ad18ac811f
)

FetchContent_Declare(
	JSTypes
	GIT_REPOSITORY "https://github.com/idDeqpt/JSTypes.git"
	GIT_TAG e1dbc8ffe91760537c1f2999080996dba3b8a9da
)

FetchContent_Declare(
	CLI
	GIT_REPOSITORY "https://github.com/idDeqpt/CommandLineInterface.git"
	GIT_TAG 7a63897b3b96b63574c35b12fd08edbc115c137e
)

FetchContent_MakeAvailable(network)
FetchContent_MakeAvailable(CLI)
FetchContent_MakeAvailable(JSTypes)


find_package(SQLite3 REQUIRED)

add_executable(${PROJECT_NAME} src/main.cpp)

target_link_libraries(${PROJECT_NAME} PRIVATE
	SQLite3::SQLite3
	network
	JSTypes
	CLI
)

target_compile_features(${PROJECT_NAME} PRIVATE cxx_std_17)

set(EXECUTABLE_OUTPUT_PATH ${CMAKE_CURRENT_SOURCE_DIR}/bin)