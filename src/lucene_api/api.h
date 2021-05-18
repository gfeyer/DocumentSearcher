#ifndef LUCENE_API_H_
#define LUCENE_API_H_

#ifndef WIN32_LEAN_AND_MEAN
#define WIN32_LEAN_AND_MEAN
#endif

#ifndef NOMINMAX
#define NOMINMAX
#endif

#include <memory>

#include "LuceneHeaders.h"

namespace lucene_api {

    // Interface for interacting with lucene results
    class IResults {
    public:
        virtual size_t Size() = 0;
        virtual double Score(size_t) = 0;
        virtual std::string Path(size_t) = 0;
        virtual std::string Name(size_t) = 0;
    };

    std::shared_ptr<IResults> NewSearch(std::string index, std::wstring query);

    // Interface for indexing directory and updating the index
    int IndexDocs(std::string source, std::string index);
    int UpdateDocs(std::string source, std::string index);

    // Utility functions
    std::wstring utf8ToUtf16(const std::string& utf8Str);
    std::string utf16ToUtf8(const std::wstring& utf16Str);
    
}

#endif // LUCENE_API_H_