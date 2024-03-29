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
		DocToTextExtractorParams* params = NULL;
		DocToTextFormattingStyle* style = NULL;
		DocToTextExtractedData* data = NULL;
		DocToTextMetadata* metadata = NULL;
		std::wstring path;
	public:
		FileDoc(std::wstring);
		~FileDoc();
		std::wstring  Content();
		std::wstring  AuthorCreated();
		std::wstring  AuthorModified();
		std::wstring  DateCreated();
		std::wstring  DateModified();
	private:
		bool IsTimeValid(const tm* tm_ptr);
		bool IsType(std::string);
	};

	std::shared_ptr<FileDoc> ReadDocument(std::wstring path);
	std::shared_ptr<std::string> ReadText(std::string path);
	std::shared_ptr<std::wstring> ReadWText(std::string path);
	void WriteText(std::string path, std::string data);

	std::string EpochToDate(std::string timestr);
	std::wstring FileNameFromPath(std::wstring);
	std::wstring ExtensionFromPath(std::wstring);

	bool IsSupported(std::wstring path);
	bool IsSupported(std::wstring path);


}

#endif // FILE_UTIL_H_