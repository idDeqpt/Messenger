cmake_minimum_required(VERSION 3.4.0)

set(PROJECT_NAME messenger_server)
if (WIN32)
	set(PATH_TO_NETWORK "C:/Projects/C++/libraries/Network/include")
else()
	set(PATH_TO_NETWORK "/home/deqpt/projects/libs/Network/include")
endif()


project(${PROJECT_NAME} CXX)

add_executable(${PROJECT_NAME} src/main.cpp)

target_include_directories(${PROJECT_NAME} PRIVATE ${PATH_TO_NETWORK})

if (WIN32)
	target_link_libraries(${PROJECT_NAME} PRIVATE "C:/Projects/C++/libraries/Network/bin/lib/Debug/network.lib")
else()
	target_link_libraries(${PROJECT_NAME} PRIVATE "/home/deqpt/projects/libs/Network/bin/lib/libnetwork.so")
endif()

target_compile_features(${PROJECT_NAME} PRIVATE cxx_std_17)

set(dir ${CMAKE_CURRENT_SOURCE_DIR}/bin/)
set(EXECUTABLE_OUTPUT_PATH ${dir} CACHE PATH "Build directory" FORCE)
set(LIBRARY_OUTPUT_PATH ${dir} CACHE PATH "Build directory" FORCE)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${dir})
set(CMAKE_BUILD_FILES_DIRECTORY ${dir})
set(CMAKE_BUILD_DIRECTORY ${dir})
set(CMAKE_BINARY_DIR  ${dir})
SET(EXECUTABLE_OUTPUT_PATH ${dir})
SET(LIBRARY_OUTPUT_PATH ${dir}lib)
SET(CMAKE_CACHEFILE_DIR ${dir})