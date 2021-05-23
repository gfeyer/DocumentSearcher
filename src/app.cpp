#include "app.h"

#include "index_ui.h"
#include "logger.h"

App::App(wxWindow* parent) : UIFrame(parent)
{
	search_panel_ = new SearchUI(auinotebook);
	auinotebook->AddPage(search_panel_, wxT("Search"), false, wxNullBitmap);
}

void App::OnSelectMenuIndexNew(wxCommandEvent& event)
{
    auto ui_filters = new FiltersUI(this);
    ui_filters->Show();
}

void App::OnSelectMenuIndexView(wxCommandEvent& event)
{
	auto ui_filters = new FiltersUI(this);
	ui_filters->Show();
}

void App::OnClose()
{

}
