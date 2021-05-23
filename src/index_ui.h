#ifndef INDEX_UI_H
#define INDEX_UI_H

#include "ui/noname.h"

class FiltersUI : public FilterFrame
{
public:
	FiltersUI(wxWindow* parent);
	~FiltersUI() { }

private:
	void OnStart(wxCommandEvent& event);
	void PopErrorDialog(std::string);
};


#endif // INDEX_UI_H