#include "file_util.h"

#include <string>
#include <iostream>
#include <sstream>
#include <fstream>
#include <ctime>
#include <boost/filesystem/operations.hpp>

#include <xlnt/xlnt.hpp>
#include <wx/string.h>

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

    File Read(std::string path)
    {
        File document;

        // Read file contents
        auto extension = ExtensionFromPath(path);
        
        // Read plain text files 
        if (extension == "txt" || extension == "csv") {
            auto ss = std::wostringstream{};
            std::wifstream input_file(path, std::ios::binary);
            if (!input_file.is_open()) {
                std::cerr << "Could not open the file" << std::endl;
                exit(EXIT_FAILURE);
            }
            ss << input_file.rdbuf();
            document.content = std::move(ss.str());
        }
        
        // Read Excel files
        else if (extension == "xlsx"){
            auto ss = std::ostringstream{};

            xlnt::workbook workbook;
            workbook.load(path);

            for (int s = 0; s < workbook.sheet_count(); ++s) {
                auto worksheet = workbook.sheet_by_index(s);

                for (auto row : worksheet.rows(false)) {
                    for (auto cell : row) {
                        ss << cell.to_string() << " ";
                    }
                    ss << "\n";
                }
                ss << "\n";
            }
            
            wxString content(std::move(ss.str()));

            document.content = content;
        }
        else {

        }
        

        // Read file attritbutes
        boost::filesystem::path p(path);
        std::time_t t = boost::filesystem::last_write_time(p);
        auto time = EpochToDate(std::to_string(t));
        document.modified = time;

        return std::move(document);
    }
}