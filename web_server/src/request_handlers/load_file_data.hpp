#ifndef LOAD_FILE_DATA_HPP
#define LOAD_FILE_DATA_HPP

#include <fstream>
#include <sstream>
#include <string>
#include <memory>
#include <iostream>

std::unique_ptr<std::string> load_file_data_ptr(std::string path)
{
	std::cout << "FILE PATH: |" << path << "|\n";
	std::ifstream file(path);

	if (!file)
		return std::unique_ptr<std::string>(nullptr);


	std::ostringstream oss;
	oss << file.rdbuf();

	std::cout << "FILE TEXT: |" << oss.str() << "|\n";
	return std::make_unique<std::string>(oss.str());//str);
}

#endif //LOAD_FILE_DATA_HPP