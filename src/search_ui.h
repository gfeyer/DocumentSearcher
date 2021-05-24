#ifndef SEARCH_UI_H
#define SEARCH_UI_H

#include "ui/noname.h"

#include <unordered_map>
#include <memory>
#include <string>

#include <wx/checkbox.h>

#include "lucene_api/api.h"

class SearchUI : public SearchPanel {
public:
	SearchUI(wxWindow* window);
	~SearchUI();

private:

	// Events
	void OnDoubleClick(wxDataViewEvent& event);
	void OnSelectResult(wxDataViewEvent& event);
	void OnSelectIndex(wxCommandEvent& event);

	void OnSearch(wxCommandEvent& event);
	void OnKeyUpFilter(wxKeyEvent& event);

	// Utility Functions
	void LoadIndexes();
	void LoadResources();
	wxBitmap GetBitmapForExtension(std::string ext);
	void OnCheck(wxCommandEvent& event);
	void OnCheck(wxCheckBox* checkbox);

	void NewSearch(std::string, std::string);
	void PopErrorDialog(std::string);

	// Session data
	std::shared_ptr<lucene_api::IResults> results_;

	// Preloaded bitmaps storage
	std::unordered_map<std::string, wxBitmap> bitmaps_;

};
#endif // SEARCH_UI_H