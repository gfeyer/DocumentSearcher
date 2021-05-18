#include "app.h"

#include "inspector.h"

App::App(wxWindow* parent) : UIFrame(parent)
{
	inspector_panel_ = new Inspector(auinotebook);
	auinotebook->AddPage(inspector_panel_, wxT("Inspector"), false, wxNullBitmap);
}

void App::OnClose()
{

}
