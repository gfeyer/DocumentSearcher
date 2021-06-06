#ifndef INDEX_H_
#define INDEX_H_

#include "api.h"

#include <functional>
#include <iostream>

#include "Config.h"
#include "targetver.h"
#include "LuceneHeaders.h"
#include "FileUtils.h"
#include "FilterIndexReader.h"
#include "MiscUtils.h"

namespace lucene_api::internal {
    std::string readFileIntoString(const std::wstring& path);
    std::wstring readFileIntoWString(const std::wstring& path);
    Lucene::DocumentPtr fileDocument(const Lucene::String& docFile);
    void IndexDocsWithWriter(const Lucene::IndexWriterPtr& writer, const Lucene::String& sourceDir, std::function<void(std::wstring)> callback);
}

#endif  // INDEX_H_
