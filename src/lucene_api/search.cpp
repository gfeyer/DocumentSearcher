#include "api.h"
#include "search.h"


namespace lucene_api::internal {
    using namespace Lucene;

    SearchResults::SearchResults(std::string index, std::wstring userquery) {
        // Search
        // String index = L"index";
        String field = FIELD_CONTENT;
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
        
        //std::wcout << L"User query: " << userquery << L"\n";
        QueryPtr query = parser->parse(userquery);
        //std::wcout << L"Searching for: " << query->toString(field) << L"\n";
    
        searcher_->search(query, FilterPtr(), 100);
    
        //doPagingSearch(searcher, query, hitsPerPage, raw, queries.empty());
        //doPagingSearch(searcher, query, hitsPerPage);
    
        // Collect enough docs for maxHits
        collector_ = TopScoreDocCollector::create(maxHits, false);
        searcher_->search(query, collector_);
        hits_ = collector_->topDocs()->scoreDocs;
    
        int32_t numTotalHits = collector_->getTotalHits();
        //std::wcout << numTotalHits << L" total matching documents\n";
    
        int32_t start = 0;
        int32_t end = hits_.size();
    
        // Get search results metadata
        /*for (auto i = 0; i < end; ++i) {
    
            DocumentPtr doc = searcher_->doc(hits_[i]->doc);
            String path = doc->get(L"path");
            String modified = doc->get(L"modified");
            String contents = doc->get(L"contents");
    
            //std::wcout << StringUtils::toString(i + 1) + L". " << path << L"\n";
            //std::wcout << "doc=" << hits_[i]->doc << " score=" << hits_[i]->score << "\n";
            //std::wcout << path << L"\n";
            //std::wcout << modified << L"\n";
            //std::wcout << contents << L"\n";
        }*/
    }

    SearchResults::~SearchResults() {
        reader_->close();
    }
    std::wstring SearchResults::Content(size_t index)
    {
        DocumentPtr doc = searcher_->doc(hits_[index]->doc);
        auto content = doc->get(FIELD_CONTENT);
        return content;
    }
    std::string SearchResults::Created(size_t index)
    {
        DocumentPtr doc = searcher_->doc(hits_[index]->doc);
        auto modified = utf16ToUtf8(doc->get(FIELD_CREATED));
        return modified;
    }
    std::string SearchResults::CreatedBy(size_t index)
    {
        DocumentPtr doc = searcher_->doc(hits_[index]->doc);
        auto modified = utf16ToUtf8(doc->get(FIELD_CREATED_BY));
        return modified;
    }
    std::string SearchResults::Modified(size_t index)
    {
        DocumentPtr doc = searcher_->doc(hits_[index]->doc);
        auto modified = utf16ToUtf8(doc->get(FIELD_MODIFIED));
        return modified;
    }
    std::string SearchResults::ModifiedBy(size_t index)
    {
        DocumentPtr doc = searcher_->doc(hits_[index]->doc);
        auto modified = utf16ToUtf8(doc->get(FIELD_MODIFIED_BY));
        return modified;
    }
    std::string SearchResults::Name(size_t index)
    {
        DocumentPtr doc = searcher_->doc(hits_[index]->doc);
        auto path = utf16ToUtf8(doc->get(FIELD_PATH));
        std::string filename = path.substr(path.find_last_of("/\\") + 1);
        return filename;
    }
    std::string SearchResults::Path(size_t index)
    {
        DocumentPtr doc = searcher_->doc(hits_[index]->doc);
        String path = doc->get(FIELD_PATH);
        return utf16ToUtf8(path);
    }
    size_t SearchResults::Size() {
        return collector_->getTotalHits();
    }
    double SearchResults::Score(size_t pos) {
        return hits_[pos]->score;
    }
}