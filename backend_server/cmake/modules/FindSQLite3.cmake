set(SQLITE3_INCLUDE_DIR "${DEPS_DIR}/includes/SQLite3")

if(WIN32)
	set(SQLITE3_LIB_FILE "${DEPS_DIR}/libs/SQLite3/sqlite3.lib")
else()
	set(SQLITE3_LIB_FILE "${DEPS_DIR}/libs/SQLite3/libsqlite3.so")
endif()