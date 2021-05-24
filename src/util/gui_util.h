#ifndef GUI_UTIL_H_
#define GUI_UTIL_H_

#include <string>
#include <wx/stc/stc.h>

namespace gui_util {
	void HighlightWord(std::string, wxStyledTextCtrl*, bool clear = false);
	void SetColor(wxColour, int, wxStyledTextCtrl*);

    const std::vector<wxColour> kHighlightColors = {
        wxColour(255,111,140),
        wxColour(223,29,108),
        wxColour(175,0,0),
        wxColour(238,102,10),
        wxColour(255,163,21),
        wxColour(0,15,179),
        wxColour(243,11,54),
        wxColour(138,43,226),
        wxColour(218,250,40),
        wxColour(15,241,206),
    };

    const wxColour blank(255,255,255);
}


#endif // GUI_UTIL_H_