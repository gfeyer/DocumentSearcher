#include "app.h"

#include "index_ui.h"
#include "logger.h"

App::App(wxWindow* parent) : UIFrame(parent)
{
	search_panel_ = new SearchUI(auinotebook);
	auinotebook->AddPage(search_panel_, wxT("Search"), false, wxNullBitmap);
}

void App::OnSelectMenuIndexView(wxCommandEvent& event)
{
	
}

void App::OnSelectMenuIndexNew(wxCommandEvent& event)
{
	search_panel_->OnNewIndex();
}

void App::OnClose()
{

}
