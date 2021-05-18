#ifndef SEARCH_H_
#define SEARCH_H_


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
    using namespace Lucene;


    class SearchResults : public IResults {
    private:
        IndexReaderPtr reader_;
        SearcherPtr searcher_;
        TopScoreDocCollectorPtr collector_;
        Collection<ScoreDocPtr> hits_;
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
            reader_ = IndexReader::open(FSDirectory::open(utf8ToUtf16(index)), true);
            searcher_ = newLucene<IndexSearcher>(reader_);
            AnalyzerPtr analyzer = newLucene<StandardAnalyzer>(LuceneVersion::LUCENE_CURRENT);
            QueryParserPtr parser = newLucene<QueryParser>(LuceneVersion::LUCENE_CURRENT, field, analyzer);

            ReaderPtr in;

            QueryPtr query = parser->parse(userquery);
            std::wcout << L"Searching for: " << query->toString(field) << L"\n";

            searcher_->search(query, FilterPtr(), 100);

            //doPagingSearch(searcher, query, hitsPerPage, raw, queries.empty());
            //doPagingSearch(searcher, query, hitsPerPage);

            // Collect enough docs for maxHits
            collector_ = TopScoreDocCollector::create(maxHits, false);
            searcher_->search(query, collector_);
            hits_ = collector_->topDocs()->scoreDocs;

            int32_t numTotalHits = collector_->getTotalHits();
            std::wcout << numTotalHits << L" total matching documents\n";

            int32_t start = 0;
            int32_t end = hits_.size();

            // Get search results metadata
            for (auto i = 0; i < end; ++i) {

                DocumentPtr doc = searcher_->doc(hits_[i]->doc);
                String path = doc->get(L"path");
                String modified = doc->get(L"modified");
                String contents = doc->get(L"contents");

                std::wcout << StringUtils::toString(i + 1) + L". " << path << L"\n";

                std::wcout << "doc=" << hits_[i]->doc << " score=" << hits_[i]->score << "\n";

                //std::wcout << path << L"\n";
                //std::wcout << modified << L"\n";
                //std::wcout << contents << L"\n";
            }

            reader_->close();
        }
        ~SearchResults() {}

        size_t Size() {
            return collector_->getTotalHits();
        }

        double Score(size_t pos) {
            return hits_[pos]->score;
        }
    };

    std::shared_ptr<IResults> NewSearch(std::string index, std::wstring query) {
        std::shared_ptr<IResults> searchResults(new SearchResults(index, query));
        return searchResults;
    }
}

#endif  // SEARCH_H_


