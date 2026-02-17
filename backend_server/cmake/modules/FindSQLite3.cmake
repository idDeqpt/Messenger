include(ExternalProject)

set(SQLITE3_SOURCE_DIR ${CMAKE_BINARY_DIR}/sqlite3-src/SQLite3)
set(SQLITE3_BINARY_DIR ${CMAKE_BINARY_DIR}/sqlite3-bin)
set(SQLITE3_INCLUDE_DIR ${CMAKE_BINARY_DIR}/sqlite3-src)
set(SQLITE3_LIBRARY ${SQLITE3_BINARY_DIR}/sqlite3.lib)

ExternalProject_Add(sqlite3_external
    URL https://www.sqlite.org/2024/sqlite-amalgamation-3450200.zip
    SOURCE_DIR ${SQLITE3_SOURCE_DIR}
    BINARY_DIR ${SQLITE3_BINARY_DIR}
    CONFIGURE_COMMAND ""
    BUILD_COMMAND cl /c /nologo /Ox /MT /EHsc /Fo${SQLITE3_BINARY_DIR}/sqlite3.obj ${SQLITE3_SOURCE_DIR}/sqlite3.c
    COMMAND lib /nologo /out:${SQLITE3_BINARY_DIR}/sqlite3.lib ${SQLITE3_BINARY_DIR}/sqlite3.obj
    INSTALL_COMMAND ""
    BUILD_BYPRODUCTS ${SQLITE3_LIBRARY}
)

add_library(SQLite3::SQLite3 STATIC IMPORTED GLOBAL)
set_target_properties(SQLite3::SQLite3 PROPERTIES
    IMPORTED_LOCATION ${SQLITE3_LIBRARY}
    INTERFACE_INCLUDE_DIRECTORIES ${SQLITE3_INCLUDE_DIR}
)

add_dependencies(SQLite3::SQLite3 sqlite3_external)

set(SQLite3_FOUND TRUE)
set(SQLite3_INCLUDE_DIRS ${SQLITE3_INCLUDE_DIR})
set(SQLite3_LIBRARIES SQLite3::SQLite3)