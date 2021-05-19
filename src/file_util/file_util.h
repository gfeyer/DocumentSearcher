#ifndef FILE_UTIL_H_
#define FILE_UTIL_H_

#include <string>
#include <iostream>
#include <string>
#include <iostream>
#include <sstream>
#include <fstream>
#include <ctime>
#include <boost/filesystem/operations.hpp>

namespace file_util {
	struct File {
		std::wstring content;
		std::string name;
		std::string path;
		std::string created;
		std::string modified;
		std::string author;
	};

	File Read(const std::wstring& path);

	std::string EpochToDate(std::string timestr);
	std::string FileNameFromPath(std::string);
	std::string ExtensionFromPath(std::string);
}

#endif // FILE_UTIL_H_