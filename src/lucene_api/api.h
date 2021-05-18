#ifndef LUCENE_API_H_
#define LUCENE_API_H_

#include <memory>

#include "LuceneHeaders.h"

namespace LuceneAPI {

    // Interface for interacting with lucene results
    class IResults {
    public:
        virtual size_t Size() = 0;
        virtual double Score(size_t) = 0;
    };

    std::shared_ptr<IResults> NewSearch(std::string index, std::wstring query);

    // Utility functions
    std::wstring utf8ToUtf16(const std::string& utf8Str);
    std::string utf16ToUtf8(const std::wstring& utf16Str);
    
}

#endif // LUCENE_API_H_