#ifndef INSPECTOR_H
#define INSPECTOR_H

#include "ui/noname.h"

#include <memory>
#include <string>
#include "lucene_api/api.h"

class SearchUI : public SearchPanel {
public:
	SearchUI(wxWindow* window);
	~SearchUI();

private:

	// Events
	void OnSelect(wxDataViewEvent& event);
	void OnSearch(wxCommandEvent& event);
	void OnKeyUpFilter(wxKeyEvent& event);

	// Utility Functions
	void UpdateResults();

	// Session data
	std::shared_ptr<lucene_api::IResults> results_;
};
#endif // INSPECTOR_H