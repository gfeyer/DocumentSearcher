#include "word_util.h"

#include <set>
#include <vector>
#include <string>
#include <sstream>
#include <regex>

std::vector<std::string> word_util::SplitIntoWords(std::string user_query)
{
    // Remove special characters
    user_query = std::regex_replace(user_query, std::regex(R"(["*~])"), " ");

    // remove keywords
    user_query = std::regex_replace(user_query, std::regex("AND"), "");

    // Used to split string around spaces
    std::istringstream ss(user_query);
    std::string word; // for storing each word
    std::set<std::string> wordset;
    while (ss >> word)
    {
        wordset.insert(word);
    }

	return std::vector<std::string>(wordset.begin(), wordset.end());
}
