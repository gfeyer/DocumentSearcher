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

#include "doctotext_c_api.h"

namespace file_util {

	class FileDocument {
	private:
		DocToTextExtractorParams* params;
		DocToTextFormattingStyle* style;
		DocToTextExtractedData* data;
		DocToTextMetadata* metadata;
		std::string path;
	public:
		FileDocument(std::string);
		~FileDocument();
		std::string  Content();
		std::wstring WContent();
		std::string  AuthorCreated();
		std::string  AuthorModified();
		std::string  DateCreated();
		std::string  DateModified();
	};

	FileDocument Read(std::string path);

	std::string EpochToDate(std::string timestr);
	std::string FileNameFromPath(std::string);
	std::string ExtensionFromPath(std::string);
}

#endif // FILE_UTIL_H_