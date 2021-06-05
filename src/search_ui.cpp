#include "search_ui.h"

#include <algorithm>    // std::sort
#include <memory>
#include <set>

#include <wx/msgdlg.h>
#include <wx/checkbox.h>
#include <nlohmann/json.hpp>

#include <boost/algorithm/string.hpp>

#include "util/file_util.h"
#include "util/gui_util.h"
#include "util/word_util.h"
#include "lucene_api/api.h"
#include "index_ui.h"
#include "logger.h"
#include "scheduler.h"

#include "ui/resources/filter.xpm"
#include "ui/resources/csv.xpm"
#include "ui/resources/doc.xpm"
#include "ui/resources/docx.xpm"
#include "ui/resources/pdf.xpm"
#include "ui/resources/rtf.xpm"
#include "ui/resources/txt.xpm"
#include "ui/resources/xls.xpm"
#include "ui/resources/xlsx.xpm"

#include "duckx.hpp"

using nlohmann::json;

SearchUI::SearchUI(wxWindow* window) : SearchPanel(window)
{
    // Create sacheduler for X miliseconds
    scheduler_ = std::make_shared<Scheduler>(200);


    LoadResources();
    LoadIndexes();

    // Setup
    gui_text_view->SetWrapMode(1);
    gui_excerpts_view->SetWrapMode(1);

    // Rank column
    gui_list_view->AppendTextColumn("Rank");
    gui_list_view->GetColumn(0)->SetWidth(40);

    // Bitmap column
    gui_list_view->AppendBitmapColumn("T", 1, wxDATAVIEW_CELL_INERT, -1,
        wxALIGN_CENTER, wxDATAVIEW_COL_RESIZABLE | wxDATAVIEW_COL_REORDERABLE);
    gui_list_view->GetColumn(1)->SetWidth(30);

    // results info
    gui_list_view->AppendTextColumn("Name");

    gui_list_view->AppendTextColumn("Location");
    gui_list_view->GetColumn(3)->SetWidth(200);

    gui_list_view->AppendTextColumn("Created");
    gui_list_view->AppendTextColumn("Author");
    gui_list_view->AppendTextColumn("ModifiedBy");
    gui_list_view->AppendTextColumn("Modified");

    // temporary
    //NewSearch("american princess", { "C:\\Users\\Vlad\\Documents\\temp\\index", "C:\\Users\\Vlad\\Documents\\temp\\index_docs" });
    //auto source = R"(X:\sap\d1)";
    //auto index = R"(X:\sap\i1)";
    //lucene_api::IndexDocs(source, index, [](std::wstring) {});

}

SearchUI::~SearchUI()
{
}

void SearchUI::NewSearch(std::string query, std::vector<std::string> indexes)
{
    // clear any locks on the index
    results_ = nullptr;

    //
    if (indexes.empty()) {
        return;
    }
    
    results_ = lucene_api::NewSearch(query, indexes);

    // Populate results
    gui_list_view->DeleteAllItems();

    for (auto i = 0; i < results_->Hits(); ++i) {
        wxVector<wxVariant> data;

        auto path = results_->Path(i);
        auto name = file_util::FileNameFromPath(path);
        auto extension = file_util::ExtensionFromPath(path);

        // Rank
        data.push_back(std::to_string(i));
        
        // Icon
        data.push_back(wxVariant(GetBitmapForExtension(extension)));

        // Name, path 
        data.push_back(name);
        data.push_back(path);

        // Created, Author, ModifiedBy, Modified
        data.push_back(results_->Created(i));
        data.push_back(results_->CreatedBy(i));
        data.push_back(results_->ModifiedBy(i));
        data.push_back(results_->Modified(i));

        gui_list_view->AppendItem(data);
    }

    // Create checkboxes
    ClearSearchSession();

    // Split query into separate words
    std::vector<std::string> words = word_util::SplitIntoWords(query);

    // One word per checkbox
    auto sizer = gui_panel_selected_words->GetSizer();
    for (auto w : words) {
        auto checkbox = new wxCheckBox(gui_panel_selected_words, wxID_ANY, w, wxDefaultPosition, wxDefaultSize, 0);
        checkbox->SetValue(1);
        sizer->Add(checkbox, 0, wxALL, 5);
        checkbox->Connect(wxEVT_COMMAND_CHECKBOX_CLICKED, wxCommandEventHandler(SearchUI::OnCheck), NULL, this);

        searchword_to_checkbox_[w] = checkbox;
    }

    // Perform a resize and autofit
    gui_panel_selected_words->Layout();
    sizer->Fit(gui_panel_selected_words);
    gui_panel_selected_words->GetParent()->Layout();
    this->Layout();
}

void SearchUI::OnDoubleClick(wxDataViewEvent& event)
{
    auto row = gui_list_view->GetSelectedRow();
    auto path = results_->Path(row);
    std::thread t([path]() {
        std::stringstream ss;
        ss << "\"" << path << "\"";
        system(ss.str().c_str());
    });
    t.detach();
}

void SearchUI::OnSelectResult(wxDataViewEvent& event)
{
    // Retrieve selection
    auto row = gui_list_view->GetSelectedRow();
    if (row < 0) {
        return;
    }

    // Clear views
    gui_text_view->ClearAll();
    gui_excerpts_view->ClearAll();

    // Populate text content
    auto content = results_->Content(row);
    gui_text_view->SetText(content);

    // Extract excerpts from text view and populate excerpts view
    RefreshExcerpts();
    RefreshHighlights();
}

void SearchUI::OnNewIndex()
{
    auto ui_filters = new FiltersUI(this);
    ui_filters->AddCallbackOnCompleted([this]() {
        LoadIndexes();
    });
    ui_filters->Show();
}

void SearchUI::OnSearch(wxCommandEvent& event)
{
    // Collect all selected indexes 
    std::vector<std::string> indexes;
    for (auto name_checkbox : index_name_to_checkbox_) {
        auto name = name_checkbox.first;
        auto checkbox = name_checkbox.second;

        if (checkbox->IsChecked()) {
            indexes.push_back(index_name_to_path_[name]);
        }
    }
    
    // Collect query
    std::string query = gui_search_query->GetValue();
    
    try {
        NewSearch(query, indexes);
    }
    catch (std::exception& e) {
        PopErrorDialog(e.what());
    }
}

void SearchUI::OnKeyUpFilter(wxKeyEvent& event)
{
    auto key = event.GetKeyCode();
    if (key == WXK_RETURN) {
        wxCommandEvent e;
        OnSearch(e);
    }
}

void SearchUI::PopErrorDialog(std::string msg)
{
    std::stringstream ss;
    ss << msg << "\n\n";
    wxString error(ss.str());
    wxMessageDialog dial(NULL,
        error, wxT("Error"), wxOK | wxICON_ERROR);
    dial.ShowModal();
}

void SearchUI::LoadIndexes()
{
    file_util::ReadText("settings.json");
    auto settings_txt = file_util::ReadText("settings.json");

    // Clear old indexes
    auto sizer = gui_panel_indexes->GetSizer();
    sizer->Clear(true);
    index_name_to_path_.clear();
    index_name_to_checkbox_.clear();

    // Load new indexes from settings file
    if (!settings_txt->empty()) {
        auto settings = json::parse(*settings_txt);
        auto indexes = settings["indexes"];

        for (size_t i = 0; i < indexes.size(); ++i) {
            auto name = indexes[i]["name"].get<std::string>();
            auto path = indexes[i]["index"].get<std::string>();
            index_name_to_path_[name] = path;
        }
    }

    // Add new indexes
    for (auto p : index_name_to_path_) {
        auto name = p.first;
        auto checkbox = new wxCheckBox(gui_panel_indexes, wxID_ANY, name, wxDefaultPosition, wxDefaultSize, 0);
        checkbox->SetValue(1);
        sizer->Add(checkbox, 0, wxALL, 5);
        index_name_to_checkbox_[name] = checkbox;
    }

    // Resize and autofit
    gui_panel_indexes->Layout();
    sizer->Fit(gui_panel_indexes);
    gui_panel_indexes->GetParent()->Layout();
    this->Layout();
}

void SearchUI::LoadResources()
{
    // Load resources/bitmaps
    bitmaps_["csv"] = wxBitmap(csv_xpm);
    bitmaps_["doc"] = wxBitmap(doc_xpm);
    bitmaps_["docx"] = wxBitmap(docx_xpm);
    bitmaps_["pdf"] = wxBitmap(pdf_xpm);
    bitmaps_["rtf"] = wxBitmap(rtf_xpm);
    bitmaps_["txt"] = wxBitmap(txt_xpm);
    bitmaps_["xls"] = wxBitmap(xls_xpm);
    bitmaps_["xlsx"] = wxBitmap(xlsx_xpm);
}

void SearchUI::ClearSearchSession()
{
    auto sizer = gui_panel_selected_words->GetSizer();
    sizer->Clear(true);
    searchword_to_checkbox_.clear();
}

wxBitmap SearchUI::GetBitmapForExtension(std::wstring wext)
{
    std::string ext = wxString(wext).ToStdString();
    if (bitmaps_.find(ext) != bitmaps_.end()) {
        return bitmaps_[ext];
    }
    return wxBitmap();
}

void SearchUI::RefreshHighlights()
{
    for (auto& p : searchword_to_checkbox_) {
        auto word = p.first;
        auto checkbox = p.second;

        if (checkbox->IsChecked()) {
            gui_util::HighlightWord(word, gui_excerpts_view);
            gui_util::HighlightWord(word, gui_text_view);
        }
        else {
            gui_util::HighlightWord(word, gui_excerpts_view, true);
            gui_util::HighlightWord(word, gui_text_view, true);
        }
    }

    gui_excerpts_view->ScrollToStart();
    gui_text_view->ScrollToStart();
}

void SearchUI::RefreshExcerpts()
{
    std::vector<std::string> checked;
    for (auto& p : searchword_to_checkbox_) {
        auto word = p.first;
        auto checkbox = p.second;

        if (checkbox->IsChecked()) {
            checked.push_back(word);
        }
    }
    gui_util::ExtractExcerpts(checked, gui_text_view, gui_excerpts_view);
}

void SearchUI::OnCheck(wxCommandEvent& event)
{
    wxCheckBox* box = wxDynamicCast(event.GetEventObject(), wxCheckBox);
    RefreshExcerpts();
    RefreshHighlights();
}

void SearchUI::OnCheck(wxCheckBox* box)
{
    auto word = box->GetLabelText();
    auto isChecked = box->IsChecked();
     
    gui_util::HighlightWord(word, gui_text_view, !isChecked);
    gui_util::HighlightWord(word, gui_excerpts_view, !isChecked);

}

