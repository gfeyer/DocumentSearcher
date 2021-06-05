#include "file_util.h"

#include <string>
#include <iostream>
#include <sstream>
#include <fstream>
#include <ctime>
#include <boost/filesystem/operations.hpp>

#include <xlnt/xlnt.hpp>
#include <wx/string.h>
#include <wx/textfile.h>

#include "../logger.h"

namespace file_util {
    #define CSV     "csv"
    #define DOC     "doc"
    #define DOCX    "docx"
    #define PDF     "pdf"
    #define RTF     "rtf"
    #define TXT     "txt"
    #define XLSX    "xlsx"
    #define XLS     "xls"

    #define PDF_READER   "pdftotext.exe"
    #define DOCX_READER  "doctotext.exe"
    #define TEMPFILE     "temp.txt"
}

namespace file_util {
    FileDoc::FileDoc(std::string p) : path(p) {
        
        is_pdf = ExtensionFromPath(path) == PDF;
        is_csv = ExtensionFromPath(path) == CSV;
        is_docx = ExtensionFromPath(path) == DOCX;
        
        //Create style and extractor params objects
        params = doctotext_create_extractor_params();
        style = doctotext_create_formatting_style();
        doctotext_formatting_style_set_url_style(style, DOCTOTEXT_URL_STYLE_EXTENDED);
        doctotext_extractor_params_set_formatting_style(params, style);
        //doctotext_extractor_params_set_verbose_logging(params, 0);

        // PDF data is extracted using another library due to a memory leak in the doctotext lib
        // CSV files always use text parser
        if (is_csv || is_pdf || is_docx) {
            doctotext_extractor_params_set_parser_type(params, DOCTOTEXT_PARSER_TXT);
        }

        //Extract contents
        if (!is_pdf && !is_docx) {
            data = doctotext_process_file(path.c_str(), params, NULL);
        }
        //Extract metadata
        metadata = doctotext_extract_metadata(path.c_str(), params, NULL);
    }

    std::wstring FileDoc::Content() {

        if (ExtensionFromPath(path) == PDF) {
            std::stringstream readCmd;
            readCmd << PDF_READER;
            readCmd << " \"";
            readCmd << path;
            readCmd << "\" ";
            readCmd << TEMPFILE;
            auto read = readCmd.str();

            std::stringstream delCmd;
            delCmd << "del ";
            delCmd << TEMPFILE;
            auto delTempFile = delCmd.str();

            // Read file
            logger_info << read;

            system(read.c_str());
            auto content = ReadText(TEMPFILE);

            // Delete temporary file
            logger_info << delTempFile;
            system(delTempFile.c_str());

            wxString wstr = *content;
            return wstr;
        }

        if (is_docx) {
            std::stringstream readCmd;
            readCmd << PDF_READER;
            readCmd << " \"";
            readCmd << path;
            readCmd << "\" ";
            readCmd << TEMPFILE;
            auto read = readCmd.str();

            std::stringstream delCmd;
            delCmd << "del ";
            delCmd << TEMPFILE;
            auto delTempFile = delCmd.str();
            
            // Read file
            logger_info << read;
            system(read.c_str());
            auto content = ReadWText(TEMPFILE);

            // Delete temporary file
            logger_info << delTempFile;
            system(delTempFile.c_str());

            return *content;

        }

        if (data != NULL) {
            // Extract contents and convert to wide string
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
            memset(date, 0, sizeof date);
            auto tm_ptr = doctotext_metadata_creation_date(metadata);
            
            if (!IsTimeValid(tm_ptr)) {
                return L"";
            }
            
            strftime(date, 64, "%Y-%m-%d", tm_ptr);
            std::cout << "Using date: " << date << std::endl;
            wxString wstr(date);
            return wstr;
           
        }
        return L"";
    }
    std::wstring FileDoc::DateModified() {
        if (metadata != NULL) {
            char date[64];
            auto tm_ptr = doctotext_metadata_last_modification_date(metadata);

            if (!IsTimeValid(tm_ptr)) {
                return L"";
            }

            strftime(date, 64, "%Y-%m-%d", tm_ptr);
            wxString wstr(date);
            return wstr;
        }
        return L"";
    }

    bool FileDoc::IsTimeValid(const tm* tm_ptr)
    {
        if (tm_ptr->tm_year<0 || tm_ptr->tm_year > 200 ||
            tm_ptr->tm_mon < 0 || tm_ptr->tm_mon > 12 ||
            tm_ptr->tm_mday < 0 || tm_ptr->tm_mday > 31) {
            return false;
        }

        return true;
    }

    FileDoc::~FileDoc() {
        // Release pointers
        if (params != NULL) {
            doctotext_free_extractor_params(params);
        }
        if (style != NULL) {
            doctotext_free_formatting_style(style);
        }
        if (metadata != NULL) {
            doctotext_free_metadata(metadata);
        }
        if (data != NULL) {
            doctotext_free_extracted_data(data);
        }
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

    bool IsSupported(std::string path)
    {
        auto ext = ExtensionFromPath(path);
        return ext == CSV || ext == DOC || ext == DOCX || ext == PDF || ext == RTF || ext == TXT || ext == XLSX || ext == XLS;
    }

    bool IsSupported(std::wstring path)
    {
        wxString wstr = path;
        return IsSupported(wstr.ToStdString());
    }

    std::shared_ptr<FileDoc> ReadDocument(std::string path)
    {
        auto doc = std::make_shared<FileDoc>(path);
        return doc;
    }
    std::shared_ptr<std::string> ReadText(std::string path)
    {
       try {
           std::ifstream instream(path);
           std::stringstream buffer;
           buffer << instream.rdbuf();

           auto contents = std::make_shared<std::string>(buffer.str());
           return contents;

       }
       catch (...) {
           logger_error << "Could not read file: " << path;
          
       }
       return std::shared_ptr<std::string>();
    }
    std::shared_ptr<std::wstring> ReadWText(std::string path)
    {
        wxTextFile tfile;
        tfile.Open(path);

        // read the first line
        std::wstringstream content;
        content << tfile.GetFirstLine();

        while (!tfile.Eof())
        {
            content << tfile.GetNextLine();
            content << '\n';
        }
        
        auto result = std::make_shared<std::wstring>(content.str());
        return result;
    }
    void WriteText(std::string path, std::string data)
    {
        
        std::ofstream outstream;
        
        outstream.open(path);
        
        if (!outstream) {
            logger_error << "could not open file for writing, " << path;
            return;
        }
        
        outstream << data;
        outstream.close();
        
    }
}