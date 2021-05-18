
#include <iostream>

#include "lucene_api/api.h"
#include "lucene_api/search.h"
#include "lucene_api/index.h"

#include "logger.h"

int main()
{
    //_setmode(_fileno(stdout), _O_U16TEXT);

    // Index documents
    auto source = R"#(C:\Users\Vlad\Documents\temp\source)#";
    auto index = R"#(C:\Users\Vlad\Documents\temp\index)#";

    lucene_api::IndexDocs(source, index);

    // Perform basic search and get results
    auto results = lucene_api::NewSearch(index, L"H4R0K2");

    logger_info << results->Size();
    logger_info << "SCORE at 0 is: " << results->Score(0);
    logger_info << "SCORE at 1 is: " << results->Score(1);
}