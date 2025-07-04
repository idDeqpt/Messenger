cmake_minimum_required(VERSION 3.5.0)

set(PROJECT_NAME messenger_server)

if (WIN32)
	set(PATH_TO_NETWORK "C:/Projects/C++/libraries/Network/include")
	set(PATH_TO_JSTYPES "C:/Projects/C++/libraries/JSTypes/include")
	set(PATH_TO_SQLITE "C:/Projects/C++/libraries/SQLite3/include")
else()
	set(PATH_TO_NETWORK "/home/deqpt/projects/libs/Network/include")
	set(PATH_TO_JSTYPES "/home/deqpt/projects/libs/JSTypes/include")
	set(PATH_TO_SQLITE "/home/deqpt/projects/libs/SQLite3/include")
endif()

project(${PROJECT_NAME} CXX)

add_executable(${PROJECT_NAME} src/main.cpp)

target_include_directories(${PROJECT_NAME} PRIVATE ${PATH_TO_NETWORK})
target_include_directories(${PROJECT_NAME} PRIVATE ${PATH_TO_JSTYPES})
target_include_directories(${PROJECT_NAME} PRIVATE ${PATH_TO_SQLITE})

if (WIN32)
	target_link_libraries(${PROJECT_NAME} PRIVATE "C:/Projects/C++/libraries/Network/bin/lib/Debug/network.lib")
	target_link_libraries(${PROJECT_NAME} PRIVATE "C:/Projects/C++/libraries/JSTypes/bin/lib/Debug/jstypes.lib")
	target_link_libraries(${PROJECT_NAME} PRIVATE "C:/Projects/C++/libraries/SQLite3/sqlite3.lib")
else()
	target_link_libraries(${PROJECT_NAME} PRIVATE "/home/deqpt/projects/libs/Network/bin/lib/libnetwork.so")
	target_link_libraries(${PROJECT_NAME} PRIVATE "/home/deqpt/projects/libs/JSTypes/bin/lib/libjstypes.so")
	target_link_libraries(${PROJECT_NAME} PRIVATE "/home/deqpt/projects/libs/SQLite3/libsqlite3.so.0.8.6")
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