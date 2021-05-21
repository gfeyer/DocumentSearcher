#include "file_util.h"

#include <string>
#include <iostream>
#include <sstream>
#include <fstream>
#include <ctime>
#include <boost/filesystem/operations.hpp>

#include <xlnt/xlnt.hpp>
#include <wx/string.h>
#include "duckx.hpp"

#include "../logger.h"

namespace file_util {
    #define CSV     "csv"
    #define DOC     "doc"
    #define DOCX    "docx"
    #define RTF     "rtf"
    #define TXT     "txt"
    #define XLSX    "xlsx"
    #define XLS     "xls"
}

namespace file_util {
    FileDoc::FileDoc(std::string p) : path(p) {

        //Create style and extractor params objects
        params = doctotext_create_extractor_params();
        style = doctotext_create_formatting_style();
        doctotext_formatting_style_set_url_style(style, DOCTOTEXT_URL_STYLE_EXTENDED);
        //doctotext_extractor_params_set_verbose_logging(params, 0);
        doctotext_extractor_params_set_formatting_style(params, style);

        //Extract text
        data = doctotext_process_file(path.c_str(), params, NULL);

        //Extract metadata
        metadata = doctotext_extract_metadata(path.c_str(), params, NULL);
    }

    std::wstring FileDoc::Content() {
        if (data != NULL) {
            // Extract contents and convert to wise string
            wxString wstr = doctotext_extracted_data_get_text(data);
            return wstr;
        }
        return L"";
    }
    std::wstring FileDoc::AuthorCreated() {
        if (metadata != NULL) {
            wxString wstr = doctotext_metadata_author(metadata);
            return wstr;
        }
        return L"";
    }
    std::wstring FileDoc::AuthorModified() {
        if (metadata != NULL) {
            wxString wstr = doctotext_metadata_last_modify_by(metadata);
            return std::move(wstr);
        }
        return L"";
    }
    std::wstring FileDoc::DateCreated() {
        if (metadata != NULL) {
            char date[64];
            strftime(date, 64, "%Y-%m-%d %H:%M:%S", doctotext_metadata_creation_date(metadata));
            wxString wstr(date);
            return wstr;
        }
        return L"";
    }
    std::wstring FileDoc::DateModified() {
        if (metadata != NULL) {
            char date[64];
            strftime(date, 64, "%Y-%m-%d %H:%M:%S", doctotext_metadata_last_modification_date(metadata));
            wxString wstr(date);
            return wstr;
        }
        return L"";
    }

    FileDoc::~FileDoc() {
        // Release pointers
        doctotext_free_extractor_params(params);
        doctotext_free_formatting_style(style);
        doctotext_free_metadata(metadata);
        doctotext_free_extracted_data(data);
    }
}


namespace file_util {


    std::string EpochToDate(std::string timestr) {

        //string timestr = "1612242000000000";
        timestr += "000000"; // long -> long long

        long long rawtime = stoll(timestr);

        // Convert from micro to seconds
        time_t epochtime = rawtime / 1000000LL;

        struct tm date;
        date = *std::localtime(&epochtime);

        // Uses a fixed-length buffer for `strtftime`
        char buffer[256];
        strftime(buffer, sizeof(buffer), "%Y-%m-%d %H:%M:%S", &date);
        auto time = std::string(buffer);
        return time.substr(0, time.find(' '));;
    }

	std::string FileNameFromPath(std::string path)
	{
        auto filename = path.substr(path.find_last_of("/\\") + 1);
        return filename;
	}

    std::string ExtensionFromPath(std::string path)
    {
        auto ext = path.substr(path.find_last_of("\.") + 1);
        return ext;
    }

    std::shared_ptr<FileDoc> Read(std::string path)
    {
        auto extension = ExtensionFromPath(path);

        // Read plain text files 
        // TODO: add support for csv format
        if (extension == CSV) {
            auto ss = std::wostringstream{};
            std::wifstream input_file(path, std::ios::binary);
            if (!input_file.is_open()) {
                std::cerr << "Could not open the file" << std::endl;
                exit(EXIT_FAILURE);
            }
            ss << input_file.rdbuf();
            //document.content = std::move(ss.str());
            return std::make_shared<FileDoc>("");
        }
        
        auto doc = std::make_shared<FileDoc>(path);
        return doc;
    }
}