#ifndef INSPECTOR_H
#define INSPECTOR_H

#include "ui/noname.h"

class Inspector : public InspectorPanel {
public:
	Inspector(wxWindow* window);
	~Inspector();
};
#endif // INSPECTOR_H