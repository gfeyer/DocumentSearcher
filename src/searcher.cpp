#include "searcher.h"

#include "lucene_api/api.h"

Searcher::Searcher(wxWindow* window) : SearchPanel(window)
{
    gui_list_view->AppendTextColumn("Id");
    gui_list_view->AppendTextColumn("Filename");
    gui_list_view->AppendTextColumn("Path");

    //_setmode(_fileno(stdout), _O_U16TEXT);

    // Index documents
    auto source = R"#(C:\Users\Vlad\Documents\temp\source)#";
    auto index = R"#(C:\Users\Vlad\Documents\temp\index)#";

    lucene_api::IndexDocs(source, index);

    // Perform basic search and get results
    results_ = lucene_api::NewSearch(index, L"H4R0K2");

    for (auto i = 0; i < results_->Size(); ++i) {
        auto id = std::to_string(i);
        auto name = results_->Name(i);
        auto path = results_->Path(i);

        InsertResult({id, name, path});
    }

    //logger_info << results_->Size();
    //logger_info << "SCORE at 0 is: " << results_->Score(0);
    //logger_info << "SCORE at 1 is: " << results_->Score(1);
}

Searcher::~Searcher()
{
}

void Searcher::InsertResult(std::vector<std::string> data)
{
    wxVector<wxVariant> d;
    for (auto item : data) {
        d.push_back(item);
    }
    gui_list_view->AppendItem(d);
}
