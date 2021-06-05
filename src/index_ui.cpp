#include "index_ui.h"

#include <functional>
#include <sstream>
#include <thread>

#include <wx/msgdlg.h>
#include <nlohmann/json.hpp>

#include "app.h"
#include "util/file_util.h"
#include "logger.h"

using json = nlohmann::json;


FiltersUI::FiltersUI(wxWindow* parent) : FilterFrame(parent)
{
    // Create sacheduler for X miliseconds
    scheduler_ = std::make_shared<Scheduler>(50);
}

void FiltersUI::AddCallbackOnCompleted(std::function<void()> callback)
{
    callback_ = callback;
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

    auto t = std::thread([this, source, index, name]() {
        int indexed_docs = 0;
        int failed_docs = 0;

        lucene_api::IndexDocs(source, index, [this,&indexed_docs,&failed_docs](std::wstring msg, bool success) {
            scheduler_->CallLaterOnMainThread([this, &indexed_docs, &failed_docs, msg, success]() {
                gui_console->InsertText(0, msg);

                if (success) {
                    indexed_docs++;
                }
                else {
                    failed_docs++;
                }
            });
        });

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

            std::stringstream ss;
            ss << "SUMMARY: Total docs: " << indexed_docs + failed_docs << ", ";
            ss << "Success: " << indexed_docs << ", ";
            ss << "Failed: " << failed_docs << "\n";
            std::string summary = ss.str();
            gui_console->InsertText(0, summary);

            // Handle callback if any
            if (callback_) {
                callback_();
            }
        }
        catch (std::exception& e) {
            std::stringstream ss;
            ss << "Exception read/writing settings.json: ";
            ss << e.what();
            PopErrorDialog(ss.str());
        }
    });

    t.detach();
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