#ifndef INDEX_H_
#define INDEX_H_

#include "api.h"

#include <iostream>

#include "Config.h"
#include "targetver.h"
#include "LuceneHeaders.h"
#include "FileUtils.h"
#include "FilterIndexReader.h"
#include "MiscUtils.h"

// TODO: cleanup references and use proper notation for lucene namespace

namespace lucene_api {

    std::string readFileIntoString(const std::wstring& path);
    std::wstring readFileIntoWString(const std::wstring& path);
    Lucene::DocumentPtr fileDocument(const Lucene::String& docFile);
    void IndexDocsWithWriter(const Lucene::IndexWriterPtr& writer, const Lucene::String& sourceDir);
    int IndexDocs(std::string source, std::string index);
}

#endif  // INDEX_H_
