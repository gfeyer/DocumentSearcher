#include "file_util.h"

#include <string>
#include <iostream>
#include <sstream>
#include <fstream>


#include <ctime>
#include <boost/filesystem/operations.hpp>

namespace file_util {

    std::string epoch_to_date(std::string timestr) {

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

        return std::string(buffer);
    }

    File read(const std::wstring& path)
    {
        File document;

        // Read file contents
        auto ss = std::wostringstream{};
        std::wifstream input_file(path, std::ios::binary);
        if (!input_file.is_open()) {
            std::cerr << "Could not open the file" << std::endl;
            exit(EXIT_FAILURE);
        }
        ss << input_file.rdbuf();
        document.content = std::move(ss.str());

        // Read file attritbutes
        boost::filesystem::path p(path);
        std::time_t t = boost::filesystem::last_write_time(p);
        auto tt = epoch_to_date(std::to_string(t));
        document.modified = tt;

        return std::move(document);
    }


}