#ifndef SEARCH_H_
#define SEARCH_H_

#include <codecvt>
#include <iostream>
#include <memory>


namespace lucene_api::internal {

    class SearchResults : public IResults {
    private:
        Lucene::IndexReaderPtr reader_;
        Lucene::SearcherPtr searcher_;
        Lucene::TopScoreDocCollectorPtr collector_;
        Lucene::Collection<Lucene::ScoreDocPtr> hits_;
    public:
        SearchResults(std::string index, std::wstring userquery);
        ~SearchResults() {}
        size_t Size();
        double Score(size_t pos);
    };
}

#endif  // SEARCH_H_


