#ifndef WIN32_LEAN_AND_MEAN
#define WIN32_LEAN_AND_MEAN
#endif

#ifndef NOMINMAX
#define NOMINMAX
#endif

#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <codecvt>
#include <io.h>
#include <fcntl.h>
#include <stdio.h>

#include <boost/algorithm/string.hpp>

#include "Config.h"
#include "targetver.h"
#include "LuceneHeaders.h"
#include "FileUtils.h"
#include "FilterIndexReader.h"
#include "MiscUtils.h"

#include "logger.h"


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

    namespace internal {

        using namespace Lucene;

        int32_t docNumber = 0;

        std::string readFileIntoString(const std::wstring& path) {
            auto ss = std::ostringstream{};
            std::ifstream input_file(path);
            if (!input_file.is_open()) {
                std::cerr << "Could not open the file" << std::endl;
                exit(EXIT_FAILURE);
            }
            ss << input_file.rdbuf();
            return ss.str();
        }

        std::wstring readFileIntoWString(const std::wstring& path) {
            auto ss = std::wostringstream{};
            std::wifstream input_file(path, std::ios::binary);
            if (!input_file.is_open()) {
                std::cerr << "Could not open the file" << std::endl;
                exit(EXIT_FAILURE);
            }
            ss << input_file.rdbuf();
            return ss.str();
        }

        DocumentPtr fileDocument(const String& docFile) {

            DocumentPtr doc = newLucene<Document>();

            // Add the path of the file as a field named "path".  Use a field that is indexed (ie. searchable), but
            // don't tokenize the field into words.
            doc->add(newLucene<Field>(L"path", docFile, Field::STORE_YES, Field::INDEX_NOT_ANALYZED));

            // Add the last modified date of the file a field named "modified".  Use a field that is indexed (ie. searchable),
            // but don't tokenize the field into words.
            doc->add(newLucene<Field>(L"modified", DateTools::timeToString(FileUtils::fileModified(docFile), DateTools::RESOLUTION_MINUTE),
                Field::STORE_YES, Field::INDEX_NOT_ANALYZED));

            // Add the contents of the file to a field named "contents".  Specify a Reader, so that the text of the file is
            // tokenized and indexed, but not stored.  Note that FileReader expects the file to be in the system's default
            // encoding.  If that's not the case searching for special characters will fail.

            auto myfile = readFileIntoString(docFile);

            doc->add(newLucene<Field>(L"contents", utf8ToUtf16(myfile), Field::STORE_YES, Field::INDEX_ANALYZED));
            //doc->add(newLucene<Field>(L"contents", newLucene<FileReader>(docFile)));

            return doc;
        }
        void indexDocsWithWriter(const IndexWriterPtr& writer, const String& sourceDir) {
            HashSet<String> dirList(HashSet<String>::newInstance());
            if (!FileUtils::listDirectory(sourceDir, false, dirList)) {
                return;
            }

            for (HashSet<String>::iterator dirFile = dirList.begin(); dirFile != dirList.end(); ++dirFile) {
                String docFile(FileUtils::joinPath(sourceDir, *dirFile));
                if (FileUtils::isDirectory(docFile)) {
                    indexDocsWithWriter(writer, docFile);
                }
                else {
                    std::wcout << L"Adding [" << ++docNumber << L"]: " << *dirFile << L"\n";

                    try {
                        writer->addDocument(fileDocument(docFile));
                    }
                    catch (FileNotFoundException&) {
                    }
                }
            }
        }

        int indexDocs(std::string source, std::string index) {
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
                IndexWriterPtr writer = newLucene<IndexWriter>(FSDirectory::open(indexDir), newLucene<StandardAnalyzer>(LuceneVersion::LUCENE_CURRENT), true, IndexWriter::MaxFieldLengthLIMITED);
                std::wcout << L"Indexing to directory: " << indexDir << L"...\n";

                indexDocsWithWriter(writer, sourceDir);

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
    }     

    int IndexDocs(std::string source, std::string index) {
        return internal::indexDocs(source,index);
    }
}


namespace LuceneAPI {
    class IResults {
    public:
        virtual size_t size() = 0;
        virtual double score(size_t) = 0;
    };

    namespace internal {


        class SearchResults : public IResults {
        private:
            IndexReaderPtr reader;
            SearcherPtr searcher;
            TopScoreDocCollectorPtr collector;
            Collection<ScoreDocPtr> hits;
        public:
            SearchResults(std::string index, std::wstring userquery) {
                // Search
                // String index = L"index";
                String field = L"contents";
                String queries;
                int32_t repeat = 0;
                bool raw = false;
                String normsField;
                bool paging = true;
                int32_t maxHits = 10;

                // only searching, so read-only=true
                reader = IndexReader::open(FSDirectory::open(utf8ToUtf16(index)), true);
                searcher = newLucene<IndexSearcher>(reader);
                AnalyzerPtr analyzer = newLucene<StandardAnalyzer>(LuceneVersion::LUCENE_CURRENT);
                QueryParserPtr parser = newLucene<QueryParser>(LuceneVersion::LUCENE_CURRENT, field, analyzer);

                ReaderPtr in;

                QueryPtr query = parser->parse(userquery);
                std::wcout << L"Searching for: " << query->toString(field) << L"\n";

                searcher->search(query, FilterPtr(), 100);

                //doPagingSearch(searcher, query, hitsPerPage, raw, queries.empty());
                //doPagingSearch(searcher, query, hitsPerPage);

                // Collect enough docs for maxHits
                collector = TopScoreDocCollector::create(maxHits, false);
                searcher->search(query, collector);
                hits = collector->topDocs()->scoreDocs;

                int32_t numTotalHits = collector->getTotalHits();
                std::wcout << numTotalHits << L" total matching documents\n";

                int32_t start = 0;
                int32_t end = hits.size();

                // Get search results metadata
                for (auto i = 0; i < end; ++i) {

                    DocumentPtr doc = searcher->doc(hits[i]->doc);
                    String path = doc->get(L"path");
                    String modified = doc->get(L"modified");
                    String contents = doc->get(L"contents");

                    std::wcout << StringUtils::toString(i + 1) + L". " << path << L"\n";

                    std::wcout << "doc=" << hits[i]->doc << " score=" << hits[i]->score << "\n";

                    //std::wcout << path << L"\n";
                    //std::wcout << modified << L"\n";
                    //std::wcout << contents << L"\n";
                }

                reader->close();
            }
            ~SearchResults() {}

            size_t size() {
                return collector->getTotalHits();
            }

            double score(size_t pos) {
                return hits[pos]->score;
            }
        };
    }

    std::shared_ptr<IResults> NewSearch(std::string index, std::wstring query) {
        std::shared_ptr<IResults> searchResults(new internal::SearchResults(index, query));
        return searchResults;
    }
}

int main()
{
    //_setmode(_fileno(stdout), _O_U16TEXT);

    // Index some documents
    auto source = R"#(C:\Users\Vlad\Documents\temp\source)#";
    auto index = R"#(C:\Users\Vlad\Documents\temp\index)#";

    LuceneAPI::IndexDocs(source, index);

    // Perform a basic search and get some results
    auto results = LuceneAPI::NewSearch(index, L"H4R0K2");

    //std::wcout << search.TotalHits();
    logger_info << results->size();

    logger_info << "SCORE at 0 is: " << results->score(0);
    logger_info << "SCORE at 1 is: " << results->score(1);
    
}