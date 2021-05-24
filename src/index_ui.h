#ifndef INDEX_UI_H
#define INDEX_UI_H

#include "ui/noname.h"

#include <functional>

class FiltersUI : public FilterFrame
{
public:
	FiltersUI(wxWindow* parent);
	void AddCallbackOnCompleted(std::function<void()>);
	~FiltersUI() { }

private:
	void OnStart(wxCommandEvent& event);
	void PopErrorDialog(std::string);

	// Callback
	std::function<void()> callback_;
};


#endif // INDEX_UI_H