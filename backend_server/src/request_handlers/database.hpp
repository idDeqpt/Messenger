#ifndef DATABASE_FUNCTIONAL_HPP
#define DATABASE_FUNCTIONAL_HPP

#include <JSTypes/JSTypes.hpp>
#include <SQLite3/sqlite3.h>
#include <unordered_map>
#include <memory>
#include <string>
#include <vector>


namespace db
{
	typedef std::vector<std::unordered_map<std::string, std::string>> DataBuffer;

	static int callback(void* data, int argc, char** argv, char** azColName) 
	{
		DataBuffer* result = static_cast<DataBuffer*>(data);
		result->emplace_back();
		for (int i = 0; i < argc; i++)
			if (argv[i])
				result->back().emplace(azColName[i], argv[i]);
			else
				result->back().emplace(azColName[i], "");
		return 0; 
	}

	std::shared_ptr<DataBuffer> exec(std::string request)
	{
	    sqlite3 *db;
	    char *errMessage = 0;
	    int exit = sqlite3_open("resources/database.db", &db);
	    DataBuffer result;

	    sqlite3_exec(db, request.c_str(), db::callback, (void*)&result, &errMessage);

	    return std::make_shared<DataBuffer>(result);
	}
}

#endif //DATABASE_FUNCTIONAL_HPP