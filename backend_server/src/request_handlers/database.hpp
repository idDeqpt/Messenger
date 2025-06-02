#ifndef DATABASE_FUNCTIONAL_HPP
#define DATABASE_FUNCTIONAL_HPP

#include <JSTypes/JSTypes.hpp>
#include <SQLite3/sqlite3.h>
#include <memory>
#include <string>


namespace db
{
	static int callback(void* data, int argc, char** argv, char** azColName) 
	{
		jst::JSArray* result = static_cast<jst::JSArray*>(data);
		jst::JSObject obj;
		std::cout << "CALLBACK\n";
		for (int i = 0; i < argc; i++)
		{
			std::cout << azColName[i] << " " << argv[i] << std::endl;
			if (argv[i])
				obj.addField(azColName[i], std::make_shared<jst::JSString>(argv[i]));
			else
				obj.addField(azColName[i]);
		}
		result->pushBack(std::make_shared<jst::JSObject>(obj));
		return 0; 
	}

	std::unique_ptr<jst::JSBaseType> exec(std::string request)
	{
	    sqlite3 *db;
	    char *errMessage = 0;
	    int exit = sqlite3_open("resources/database.db", &db);
	    jst::JSArray result;

	    sqlite3_exec(db, request.c_str(), db::callback, (void*)&result, &errMessage);

	    std::cout << result.toString() << std::endl;

	    return std::make_unique<jst::JSArray>(result);
	}
}

#endif //DATABASE_FUNCTIONAL_HPP