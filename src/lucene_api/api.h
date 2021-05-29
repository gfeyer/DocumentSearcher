#ifndef LUCENE_API_H_
#define LUCENE_API_H_

#ifndef WIN32_LEAN_AND_MEAN
#define WIN32_LEAN_AND_MEAN
#endif

#ifndef NOMINMAX
#define NOMINMAX
#endif

#include <functional>
#include <memory>
#include <vector>

#include "LuceneHeaders.h"

namespace lucene_api {

    // Interface for interacting with lucene results
    class IResults {
    public:
        virtual std::wstring Content(size_t) = 0;
        virtual std::string  Created(size_t) = 0;
        virtual std::string  CreatedBy(size_t) = 0;
        virtual std::string  Modified(size_t) = 0;
        virtual std::string  ModifiedBy(size_t) = 0;
        virtual std::string  Name(size_t) = 0;
        virtual std::string  Path(size_t) = 0;
        virtual size_t Hits() = 0;
        virtual double Score(size_t) = 0;
    };

    std::shared_ptr<IResults> NewSearch(std::string query, std::vector<std::string> indexes);

    // Interface for indexing directory and updating the index
    int IndexDocs(std::string source, std::string index, std::function<void(std::wstring)>);
    int UpdateDocs(std::string source, std::string index);

    // Utility functions
    std::wstring utf8ToUtf16(const std::string& utf8Str);
    std::string utf16ToUtf8(const std::wstring& utf16Str);
    
}

namespace lucene_api::internal {
    #define FIELD_PATH  L"path"
    #define FIELD_CREATED L"created"
    #define FIELD_CREATED_BY  L"createdby"
    #define FIELD_MODIFIED L"modified"
    #define FIELD_MODIFIED_BY L"modifiedby"
    #define FIELD_CONTENT L"content"
}

#endif // LUCENE_API_H_