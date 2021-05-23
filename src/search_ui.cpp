#include "search_ui.h"

#include <wx/msgdlg.h>

#include "file_util/file_util.h"
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

SearchUI::SearchUI(wxWindow* window) : SearchPanel(window)
{
    LoadResources();

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
    auto source = "C:\\Users\\Vlad\\Documents\\temp\\source";
    auto index = "C:\\Users\\Vlad\\Documents\\temp\\index";
    lucene_api::IndexDocs(source, index);

    // Perform basic search and get results
    NewSearch("H4R*", index);
}

SearchUI::~SearchUI()
{
}

void SearchUI::NewSearch(std::string query, std::string index)
{
    // clear any locks on the index
    //results_ = nullptr;
    results_ = lucene_api::NewSearch(index, wxString(query));

    gui_list_view->DeleteAllItems();

    for (auto i = 0; i < results_->Size(); ++i) {
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
        system(path.c_str());
    });
    t.detach();
}

void SearchUI::OnSelect(wxDataViewEvent& event)
{
    auto row = gui_list_view->GetSelectedRow();
    if (row < 0) {
        return;
    }

    auto content = results_->Content(row);
    //auto path = gui_list_view->GetTextValue(i, 1);
    gui_text_view->SetText(content);
}

void SearchUI::OnSearch(wxCommandEvent& event)
{
    auto index = R"#(C:\Users\Vlad\Documents\temp\index)#";
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
