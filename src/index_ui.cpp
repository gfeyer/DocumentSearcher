#include "index_ui.h"

#include <sstream>
#include <wx/msgdlg.h>
#include <nlohmann/json.hpp>

#include "app.h"
#include "file_util/file_util.h"
#include "logger.h"

using json = nlohmann::json;


FiltersUI::FiltersUI(wxWindow* parent) : FilterFrame(parent)
{
    
}

void FiltersUI::OnStart(wxCommandEvent& event)
{
    // Index documents
    auto source = gui_source_dir->GetTextCtrlValue();
    auto index = gui_index_dir->GetTextCtrlValue();
    auto name = gui_name->GetValue();

    logger_info << index;

    if (source.empty() || index.empty() || name.empty()) {
        PopErrorDialog("Please complete all fields");
        return;
    }

    lucene_api::IndexDocs(source, index);

    try {
        logger_info << "saving to settings";
        auto settings_txt = file_util::ReadText("settings.json");
        if (!settings_txt->empty()) {
            auto settings = json::parse(*settings_txt);

            settings["indexes"].push_back({
                {"source",source}, 
                {"index",index},
                {"name",name},
            });
            
            file_util::WriteText("settings.json", settings.dump(4));
        }
        else {
            
            json settings = {};

            settings["indexes"] = {
                {
                    {"source",source},
                    {"index",index},
                    {"name",name},
                },
            };

            file_util::WriteText("settings.json", settings.dump(4));
        }
        logger_info << "settings saved";
    }
    catch (std::exception& e) {
        std::stringstream ss;
        ss << "Exception read/writing settings.json: ";
        ss << e.what();
        PopErrorDialog(ss.str());
    }

}

void FiltersUI::PopErrorDialog(std::string msg)
{
    std::stringstream ss;
    ss << msg << "\n\n";
    wxString error(ss.str());
    wxMessageDialog dial(NULL,
        error, wxT("Error"), wxOK | wxICON_ERROR);
    dial.ShowModal();
}