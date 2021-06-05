#ifndef SEARCH_UI_H
#define SEARCH_UI_H

#include "ui/noname.h"

#include <memory>
#include <string>
#include <vector>
#include <tuple>

#include <wx/checkbox.h>

#include "lucene_api/api.h"

#include "scheduler.h"

class SearchUI : public SearchPanel {
public:
	SearchUI(wxWindow* window);
	void OnNewIndex();
	~SearchUI();

private:

	// Events
	void OnDoubleClick(wxDataViewEvent& event);
	void OnSelectResult(wxDataViewEvent& event);

	void OnSearch(wxCommandEvent& event);
	void OnKeyUpFilter(wxKeyEvent& event);

	void OnCheck(wxCommandEvent& event);
	void OnCheck(wxCheckBox* checkbox);

	// Utility Functions
	void LoadIndexes();
	void LoadResources();
	void ClearSearchSession();
	wxBitmap GetBitmapForExtension(std::wstring ext);

	void RefreshHighlights();
	void RefreshExcerpts();

	void NewSearch(std::string, std::vector<std::string>);
	void PopErrorDialog(std::string);

	// Session data
	std::unordered_map<std::string, std::string> index_name_to_path_;
	std::unordered_map<std::string, wxCheckBox*> index_name_to_checkbox_;
	std::unordered_map<std::string, wxCheckBox*> searchword_to_checkbox_;

	std::shared_ptr<lucene_api::IResults> results_;

	// Preloaded bitmaps storage
	std::unordered_map<std::string, wxBitmap> bitmaps_;

	// Scheduler for calling function on main thread
	std::shared_ptr<Scheduler> scheduler_;

};
#endif // SEARCH_UI_H