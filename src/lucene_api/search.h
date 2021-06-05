#ifndef SEARCH_H_
#define SEARCH_H_

#include <codecvt>
#include <iostream>
#include <memory>
#include <vector>

namespace lucene_api::internal {

    class SearchResults : public IResults {
    private:
        Lucene::IndexReaderPtr reader_;
        Lucene::SearcherPtr searcher_;
        Lucene::TopScoreDocCollectorPtr collector_;
        Lucene::Collection<Lucene::ScoreDocPtr> hits_;
    public:
        SearchResults(std::wstring userquery, std::vector<std::string> index);
        ~SearchResults();
        std::wstring Content(size_t);
        std::string Created(size_t);
        std::string CreatedBy(size_t);
        std::string Modified(size_t);
        std::string ModifiedBy(size_t);
        std::string Name(size_t);
        std::wstring Path(size_t);
        size_t Hits();
        double Score(size_t pos);
    };
}

#endif  // SEARCH_H_


