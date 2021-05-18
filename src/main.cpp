#include <iostream>

#include "lucene_api/api.h"
#include "lucene_api/search.h"
#include "lucene_api/index.h"

// WxWidgets
#include <wx/wxprec.h>
#ifndef WX_PRECOMP
#include <wx/wx.h>
#endif
#include "ui/noname.h"

#include "logger.h"

#include "ui/app.h"

class MainApp : public wxApp
{
public:
	// test git
	MainApp() {}
	~MainApp() {}

public:
	bool OnInit();

private:
	App* ui_frame = nullptr;
};

//wxIMPLEMENT_APP(MainApp);
wxIMPLEMENT_APP_CONSOLE(MainApp);		// VS2019: if switching to console app, also change under Linker->System->Subsystem

bool MainApp::OnInit()
{
	//logger::SetLevelTrace();
	logger::SetLevelDebug();
	//logger::WriteToFile("log.txt");

	ui_frame = new App(nullptr);
	ui_frame->SetTitle(VERSION);
	ui_frame->Show();

	//wxIcon mainicon;
	//wxBitmap bitmap(logo4_png);
	//mainicon.CopyFromBitmap(bitmap);
	//ui_frame->SetIcon(mainicon);
	return true;
}

/*int main()
{
    //_setmode(_fileno(stdout), _O_U16TEXT);

    // Index documents
    auto source = R"#(C:\Users\Vlad\Documents\temp\source)#";
    auto index = R"#(C:\Users\Vlad\Documents\temp\index)#";

    lucene_api::IndexDocs(source, index);

    // Perform basic search and get results
    auto results = lucene_api::NewSearch(index, L"H4R0K2");

    logger_info << results->Size();
    logger_info << "SCORE at 0 is: " << results->Score(0);
    logger_info << "SCORE at 1 is: " << results->Score(1);
}*/