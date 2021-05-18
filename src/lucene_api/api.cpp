
#include "api.h"

#include <codecvt>
#include <iostream>

#include "search.h"

namespace LuceneAPI {
    std::wstring utf8ToUtf16(const std::string& utf8Str)
    {
        std::wstring_convert<std::codecvt_utf8_utf16<wchar_t>> conv;
        return conv.from_bytes(utf8Str);
    }

    std::string utf16ToUtf8(const std::wstring& utf16Str)
    {
        std::wstring_convert<std::codecvt_utf8_utf16<wchar_t>> conv;
        return conv.to_bytes(utf16Str);
    }

    std::shared_ptr<IResults> NewSearch(std::string index, std::wstring query) {
        std::shared_ptr<IResults> searchResults(new SearchResults(index, query));
        return searchResults;
    }
}