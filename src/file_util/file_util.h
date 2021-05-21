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

	class FileDoc {
	private:
		DocToTextExtractorParams* params;
		DocToTextFormattingStyle* style;
		DocToTextExtractedData* data;
		DocToTextMetadata* metadata;
		std::string path;
	public:
		FileDoc(std::string);
		~FileDoc();
		std::wstring  Content();
		std::wstring  AuthorCreated();
		std::wstring  AuthorModified();
		std::wstring  DateCreated();
		std::wstring  DateModified();
	};

	std::shared_ptr<FileDoc> Read(std::string path);

	std::string EpochToDate(std::string timestr);
	std::string FileNameFromPath(std::string);
	std::string ExtensionFromPath(std::string);
}

#endif // FILE_UTIL_H_