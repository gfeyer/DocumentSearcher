#include "search_ui.h"

#include "lucene_api/api.h"
#include "logger.h"

SearchUI::SearchUI(wxWindow* window) : SearchPanel(window)
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
    UpdateResults();
}

SearchUI::~SearchUI()
{
}

void SearchUI::UpdateResults()
{
    gui_list_view->DeleteAllItems();

    for (auto i = 0; i < results_->Size(); ++i) {
        wxVector<wxVariant> data;
        data.push_back(std::to_string(i));
        data.push_back(results_->Name(i));
        data.push_back(results_->Path(i));

        //d.push_back(item);
        gui_list_view->AppendItem(data);
    }
}

void SearchUI::OnSelect(wxDataViewEvent& event)
{
    auto row = gui_list_view->GetSelectedRow();
    auto content = results_->Content(row);
    //auto path = gui_list_view->GetTextValue(i, 1);
    gui_text_view->SetText(content);
}

void SearchUI::OnSearch(wxCommandEvent& event)
{
    auto index = R"#(C:\Users\Vlad\Documents\temp\index)#";
    std::wstring query = gui_search_query->GetLabelText().ToStdWstring();
    results_ = nullptr; // clear any locks on the index

    // query gets destroyed and is not copied - hence crash 
    results_ = lucene_api::NewSearch(index, query);
    UpdateResults();
}

void SearchUI::OnKeyUpFilter(wxKeyEvent& event)
{
}
