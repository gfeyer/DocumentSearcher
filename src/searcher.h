#ifndef INSPECTOR_H
#define INSPECTOR_H

#include "ui/noname.h"

#include <memory>
#include <string>
#include "lucene_api/api.h"

class Searcher : public SearchPanel {
public:
	Searcher(wxWindow* window);
	~Searcher();

private:

	void InsertResult(std::vector<std::string>);

	// Session data
	std::shared_ptr<lucene_api::IResults> results_;
};
#endif // INSPECTOR_H