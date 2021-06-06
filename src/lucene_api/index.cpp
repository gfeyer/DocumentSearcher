#include "api.h"
#include "index.h"

#include <functional>
#include <wx/string.h>
#include "../util/file_util.h"


namespace lucene_api::internal {
    using namespace Lucene;
    
    int32_t docNumber = 0;
    
    Lucene::DocumentPtr fileDocument(const Lucene::String& docFile, bool& success) {
    
        DocumentPtr doc = newLucene<Document>(); 

        wxString path = docFile;
        auto fileDoc = file_util::ReadDocument(path.ToStdWstring());

        // Add metadata
        // Add the path of the file as a field named "path".  Use a field that is indexed (ie. searchable), but
        // don't tokenize the field into words.

        doc->add(newLucene<Field>(FIELD_PATH, docFile, Field::STORE_YES, Field::INDEX_NOT_ANALYZED));
        doc->add(newLucene<Field>(FIELD_CREATED, fileDoc->DateCreated(),Field::STORE_YES, Field::INDEX_NOT_ANALYZED));
        doc->add(newLucene<Field>(FIELD_CREATED_BY, fileDoc->AuthorCreated(),Field::STORE_YES, Field::INDEX_NOT_ANALYZED));
        doc->add(newLucene<Field>(FIELD_MODIFIED, fileDoc->DateModified(), Field::STORE_YES, Field::INDEX_NOT_ANALYZED));
        doc->add(newLucene<Field>(FIELD_MODIFIED_BY, fileDoc->AuthorModified(), Field::STORE_YES, Field::INDEX_NOT_ANALYZED));

        // Add file contents:
        auto content = fileDoc->Content();
        if (content.size() > 5) {
            success = true;
        }
        else {
            success = false;
        }
        doc->add(newLucene<Field>(FIELD_CONTENT, content, Field::STORE_YES, Field::INDEX_ANALYZED));
        
        return doc;
    }
    
    void IndexDocsWithWriter(const Lucene::IndexWriterPtr& writer, const Lucene::String& sourceDir, std::function<void(std::wstring,bool)> callback) {
    
        HashSet<String> dirList(HashSet<String>::newInstance());
        if (!FileUtils::listDirectory(sourceDir, false, dirList)) {
            return;
        }
    
        for (HashSet<String>::iterator dirFile = dirList.begin(); dirFile != dirList.end(); ++dirFile) {
            
            auto path = FileUtils::joinPath(sourceDir, *dirFile);
            String docFile(path);
            if (FileUtils::isDirectory(docFile)) {
                IndexDocsWithWriter(writer, docFile, callback);
            }
            else {
                std::wstringstream ss;

                if (!file_util::IsSupported(path)) {
                    ss << L"Extension not supported, skipping: " << path << L"\n";
                    //callback(ss.str(),true);
                    continue;
                }

                try {
                    //std::wcout << L"addDocument->\n";
                    bool success;
                    auto fdoc = fileDocument(docFile, success);

                    if (success) {
                        ss << L"Adding [" << ++docNumber << L"]: " << path << L"\n";
                    }
                    else {
                        ss << L"Cannot extract text from: " << path << L"\n";
                    }
                    callback(ss.str(), success);

                    writer->addDocument(fdoc);
                    //std::wcout << L"done->\n";
                }
                catch (FileNotFoundException&) {
                }
            }
        }
    }
}

namespace lucene_api {
    int IndexDocs(std::string source, std::string index, std::function<void(std::wstring,bool)> callback) {
        using namespace Lucene;
        String sourceDir(StringUtils::toUnicode(source));
        String indexDir(StringUtils::toUnicode(index));

        if (!FileUtils::isDirectory(sourceDir)) {
            std::wcout << L"Source directory doesn't exist: " << sourceDir << L"\n";
            return 1;
        }

        if (!FileUtils::isDirectory(indexDir)) {
            if (!FileUtils::createDirectory(indexDir)) {
                std::wcout << L"Unable to create directory: " << indexDir << L"\n";
                return 1;
            }
        }

        uint64_t beginIndex = MiscUtils::currentTimeMillis();

        try {
            IndexWriterPtr writer = newLucene<IndexWriter>(
                FSDirectory::open(indexDir),
                newLucene<StandardAnalyzer>(LuceneVersion::LUCENE_CURRENT),
                true,
                IndexWriter::MaxFieldLengthLIMITED);

            std::wcout << L"Indexing to directory: " << indexDir << L"...\n";

            internal::IndexDocsWithWriter(writer, sourceDir, callback);

            uint64_t endIndex = MiscUtils::currentTimeMillis();
            uint64_t indexDuration = endIndex - beginIndex;
            std::wcout << L"Index time: " << indexDuration << L" milliseconds\n";
            std::wcout << L"Optimizing...\n";

            writer->optimize();

            uint64_t optimizeDuration = MiscUtils::currentTimeMillis() - endIndex;
            std::wcout << L"Optimize time: " << optimizeDuration << L" milliseconds\n";

            writer->close();

            std::wcout << L"Total time: " << indexDuration + optimizeDuration << L" milliseconds\n";
        }
        catch (LuceneException& e) {
            std::wcout << L"Exception: " << e.getError() << L"\n";
            return 1;
        }
        return 0;
    }

    int UpdateDocs(std::string source, std::string index) {
        // TODO: update archive if documents are modified
        return 0;
    }
}