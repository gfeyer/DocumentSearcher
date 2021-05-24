#include "search_ui.h"

#include <wx/msgdlg.h>
#include <wx/checkbox.h>
#include <nlohmann/json.hpp>

#include <boost/algorithm/string.hpp>

#include "file_util/file_util.h"
#include "index_ui.h"
#include "lucene_api/api.h"
#include "logger.h"

#include "ui/resources/filter.xpm"
#include "ui/resources/csv.xpm"
#include "ui/resources/doc.xpm"
#include "ui/resources/docx.xpm"
#include "ui/resources/pdf.xpm"
#include "ui/resources/rtf.xpm"
#include "ui/resources/txt.xpm"
#include "ui/resources/xls.xpm"
#include "ui/resources/xlsx.xpm"

/*
    TODO:
        - refactor settings into its own class rather than reading file from everywhere

*/

using nlohmann::json;

namespace colors {
    const std::vector<wxColour> kHighlightColors = {
        wxColour(255,111,140),
        wxColour(223,29,108),
        wxColour(175,0,0),
        wxColour(238,102,10),
        wxColour(255,163,21),
        wxColour(0,15,179),
        wxColour(243,11,54),
        wxColour(138,43,226),
        wxColour(218,250,40),
        wxColour(15,241,206),
    };
}

SearchUI::SearchUI(wxWindow* window) : SearchPanel(window)
{
    LoadResources();
    LoadIndexes();

    // Setup
    gui_text_view->SetWrapMode(1);

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
    
    //_setmode(_fileno(stdout), _O_U16TEXT);
    //auto source = "C:\\Users\\Vlad\\Documents\\temp\\source2";
    //auto index = "C:\\Users\\Vlad\\Documents\\temp\\index";
    //lucene_api::IndexDocs(source, index);

    // Perform basic search and get results
    auto index = gui_choice_index->GetStringSelection();
    if (!index.empty()) {
        NewSearch("H4R*", index);
    }
}

SearchUI::~SearchUI()
{
}

void SearchUI::NewSearch(std::string query, std::string index)
{
    // clear any locks on the index
    //results_ = nullptr;
    results_ = lucene_api::NewSearch(query, index);

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

    // Populate text content
    auto content = results_->Content(row);
    gui_text_view->SetText(content);

    auto sizer = gui_checkboxes->GetSizer();

    // Clear previous results
    sizer->Clear();

    // TODO: Get list of words to display
    std::vector<std::string> words = {"H4R0K2","sold"};

    // Display words in query as checkboxes
    for (auto w : words) {
        auto checkbox = new wxCheckBox(gui_checkboxes, wxID_ANY, w, wxDefaultPosition, wxDefaultSize, 0);
        checkbox->SetValue(1);
        sizer->Add(checkbox, 0, wxALL, 5);
        checkbox->Connect(wxEVT_COMMAND_CHECKBOX_CLICKED, wxCommandEventHandler(SearchUI::OnCheck), NULL, this);

        OnCheck(checkbox);
    }

    // Perform a resize and autofit
    gui_checkboxes->Layout();
    sizer->Fit(gui_checkboxes);
    gui_checkboxes->GetParent()->Layout();
    this->Layout();
}

void SearchUI::OnSelectIndex(wxCommandEvent& event)
{
    auto selection = gui_choice_index->GetStringSelection();
    if (selection == "New...") {
        auto ui_filters = new FiltersUI(this);
        ui_filters->AddCallbackOnCompleted([this]() {
            LoadIndexes();
        });
        ui_filters->Show();
    }
}

void SearchUI::OnSearch(wxCommandEvent& event)
{
    auto index = gui_choice_index->GetStringSelection();
    std::string query = gui_search_query->GetValue();
    
    try {
        NewSearch(query, index);
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

    // Clear indexes/reloading
    gui_choice_index->Clear();

    if (!settings_txt->empty()) {
        auto settings = json::parse(*settings_txt);
        auto indexes = settings["indexes"];

        for (size_t i = 0; i < indexes.size(); ++i) {
            auto index_path = indexes[i]["index"].get<std::string>();
            gui_choice_index->AppendString(index_path);
        }
    }

    if (gui_choice_index->GetCount() > 0) {
        gui_choice_index->SetSelection(0);
    }
    
    gui_choice_index->AppendString("New...");
    
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

wxBitmap SearchUI::GetBitmapForExtension(std::string ext)
{
    if (bitmaps_.find(ext) != bitmaps_.end()) {
        return bitmaps_[ext];
    }
    return wxBitmap();
}

void SearchUI::OnCheck(wxCommandEvent& event)
{
    wxCheckBox* box = wxDynamicCast(event.GetEventObject(), wxCheckBox);
    OnCheck(box);
}

void SearchUI::OnCheck(wxCheckBox* box)
{
    logger_info << "OnCheck: " << box->IsChecked() << ", " << box->GetLabelText();

    HighlightWord(box->GetLabelText());
}

void SearchUI::HighlightWord(std::string word)
{
    auto ctrl = gui_text_view;

    if (word.size() < 3) {
        return;
    }

    // Select highlight color
    // simple hash: get first letter of word, convert to int and mod against the size of color vector
    // and use the resulting color from the vector.
    size_t idx = int(word[0]) % colors::kHighlightColors.size();
    wxColour color = colors::kHighlightColors[idx];
    SetColor(color, 8 + idx);

    // Perform the search
    std::string text = ctrl->GetText();

    boost::algorithm::to_lower(text);
    boost::algorithm::to_lower(word);

    std::vector<size_t> positions; // holds all the positions that sub occurs within str
    size_t pos = text.find(word, 0);
    while (pos != std::string::npos)
    {
        positions.push_back(pos);
        pos = text.find(word, pos + 1);
    }

    // no matches, exit
    if (positions.empty()) {
        return;
    }

    for (auto& p : positions) {
        ctrl->IndicatorFillRange(p, word.size());
    }

    // selection
    auto caret = ctrl->GetSelectionNCaret(0);
    for (auto& p : positions) {
        if (p > caret) {
            auto anchor = p;
            caret = p + word.size();

            ctrl->SetAnchor(anchor);
            ctrl->SetSelectionNCaret(0, caret);

            ctrl->EnsureCaretVisible();
            return;
        }
    }

    // at this point, it means we reached the end. Scroll to beginning
    auto anchor = positions[0];
    ctrl->SetAnchor(anchor);
    ctrl->SetSelectionNCaret(0, anchor + word.size());
    ctrl->EnsureCaretVisible();
}

void SearchUI::SetColor(wxColour colour, int indicator)
{
    // indicator: can be between 8 and 32, sets color only for this indicator.

    // highlight a word
    gui_text_view->SetIndicatorCurrent(indicator);
    //gui_text_view->IndicatorClearRange(0, gui_text_view->GetLength());
    gui_text_view->IndicatorSetStyle(indicator, wxSTC_INDIC_STRAIGHTBOX);
    gui_text_view->IndicatorSetUnder(indicator, true);
    gui_text_view->IndicatorSetForeground(indicator, colour);
    gui_text_view->IndicatorSetOutlineAlpha(indicator, 50);
    gui_text_view->IndicatorSetAlpha(indicator, 50);
}
