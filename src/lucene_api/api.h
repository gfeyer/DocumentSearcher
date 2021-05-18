#ifndef LUCENE_API_H_
#define LUCENE_API_H_

#include <codecvt>
#include <iostream>
#include <memory>

#include "Config.h"
#include "targetver.h"
#include "LuceneHeaders.h"
#include "FileUtils.h"
#include "FilterIndexReader.h"
#include "MiscUtils.h"

namespace LuceneAPI {

    std::wstring utf8ToUtf16(const std::string& utf8Str);
    std::string utf16ToUtf8(const std::wstring& utf16Str);

    class IResults {
    public:
        virtual size_t Size() = 0;
        virtual double Score(size_t) = 0;
    };

    std::shared_ptr<IResults> NewSearch(std::string index, std::wstring query);
}

#endif // LUCENE_API_H_