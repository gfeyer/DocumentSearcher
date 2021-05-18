#include "app.h"

#include "logger.h"

App::App(wxWindow* parent) : UIFrame(parent)
{
	search_panel_ = new SearchUI(auinotebook);
	auinotebook->AddPage(search_panel_, wxT("Search"), false, wxNullBitmap);
}

void App::OnClose()
{

}
