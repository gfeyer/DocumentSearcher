#ifndef UI_H_
#define UI_H_

#include "ui/noname.h"

#include <memory>

#include "search_ui.h"

const std::string VERSION = "DesktopSearcher 1.0";
const std::string BUILD_TIME = __TIME__;
const std::string BUILD_DATE = __DATE__;

// Main UI class
class App : public UIFrame
{
public:
    App(wxWindow* parent);
    ~App() {}

private:
    SearchUI* search_panel_;
    // Event handlers

    // Closing & Cleanup
    void OnClose();


};


#endif  // UI_H_