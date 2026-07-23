cmake_minimum_required(VERSION 3.14)

set(PROJECT_NAME Messenger)

project(${PROJECT_NAME} CXX)


include(FetchContent)

FetchContent_Declare(
	network
	GIT_REPOSITORY "https://github.com/idDeqpt/Network.git"
	GIT_TAG fc123e0a9f935772ae6928872e45a4ad18ac811f
	PATCH_COMMAND sed -i "s/(!file) ? 0 : file.tellg()/(!file) ? 0 : static_cast<int>(file.tellg())/g" src/WebServer.cpp
)

FetchContent_Declare(
	CLI
	GIT_REPOSITORY "https://github.com/idDeqpt/CommandLineInterface.git"
	GIT_TAG db2c0a12cf15abf85afcc7225a93abf9cd6e5cc8
)


set(BUILD_SHARED_LIBS OFF CACHE BOOL "Build shared library" FORCE)

FetchContent_MakeAvailable(network)
FetchContent_MakeAvailable(CLI)

add_executable(${PROJECT_NAME} src/main.cpp)

target_link_libraries(${PROJECT_NAME} PRIVATE network)
target_link_libraries(${PROJECT_NAME} PRIVATE CLI)

target_compile_features(${PROJECT_NAME} PRIVATE cxx_std_17)

set_target_properties(${PROJECT_NAME} PROPERTIES
    RUNTIME_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/bin/$<CONFIG>"
)