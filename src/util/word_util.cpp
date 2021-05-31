#include "word_util.h"

#include <set>
#include <vector>
#include <string>
#include <sstream>

std::vector<std::string> word_util::SplitIntoWords(std::string sentence)
{
    
    // Used to split string around spaces.
    std::istringstream ss(sentence);

    std::string word; // for storing each word

    std::set<std::string> wordset;

    while (ss >> word)
    {
        wordset.insert(word);
    }

	return std::vector<std::string>(wordset.begin(), wordset.end());
}
