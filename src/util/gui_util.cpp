#include "gui_util.h"

#include <boost/algorithm/string.hpp>

#include "../logger.h"

std::vector<int> gui_util::FindWordOccurences(wxStyledTextCtrl* ctrl, std::string word)
{
    std::vector<int> positions; // holds all the positions that sub occurs within str

    wxString wword(word);
    int pos = ctrl->FindText(0, ctrl->GetLastPosition(), wword);

    while (pos != -1)
    {
        positions.push_back(pos);
        pos = ctrl->FindText(pos + wword.size(), ctrl->GetLastPosition(), wword);
    }

    return positions;
}

void gui_util::HighlightWord(std::string word, wxStyledTextCtrl* ctrl, bool clear)
{

    if (word.size() < 3) {
        return;
    }

    // Select highlight color
    // simple hash: get first letter of word, convert to int and mod against the size of color vector
    // and use the resulting color from the vector.
    size_t idx = int(word[0]) % kHighlightColors.size();
    
    wxColour color;
    if (clear) {
        // clear the highlight
        color = blank;
    }
    else {
        color = kHighlightColors[idx];
    }

    SetColor(color, 8 + idx, ctrl); // set color for indicator for control view

    // Perform the search
    auto positions = FindWordOccurences(ctrl, word);

    // no matches, exit
    if (positions.empty()) {
        return;
    }

    for (auto& p : positions) {
        ctrl->IndicatorFillRange(p, word.size());
    }

    // selection
    auto caret = ctrl->GetSelectionNCaret(0);
    for (auto& p : positions) {
        if (p > caret) {
            auto anchor = p;
            caret = p + word.size();

            ctrl->SetAnchor(anchor);
            ctrl->SetSelectionNCaret(0, caret);

            ctrl->EnsureCaretVisible();
            return;
        }
    }

    // at this point, it means we reached the end. Scroll to beginning
    auto anchor = positions[0];
    ctrl->SetAnchor(anchor);
    ctrl->SetSelectionNCaret(0, anchor + word.size());
    ctrl->EnsureCaretVisible();
}

void gui_util::ScrollToFirstOccurence(std::vector<std::string> words, wxStyledTextCtrl* ctrl)
{
    //wxString wword(word);
    //int pos = ctrl->FindText(0, ctrl->GetLastPosition(), wword);

    ctrl->Refresh(true);

    auto text = ctrl->GetText();
    size_t min = 0;
    for (auto word : words) {
        wxString wword(word);
        int pos = ctrl->FindText(0, ctrl->GetLastPosition(), wword);
        if (pos != -1) {
            if (min < pos) {
                min = pos;
            }
        }
    }
    
    auto caret = ctrl->GetSelectionNCaret(0);
    auto anchor = min;
    caret = min + 10;
    ctrl->SetAnchor(anchor);
    ctrl->SetSelectionNCaret(0, caret);
    ctrl->EnsureCaretVisible();
}

void gui_util::SetColor(wxColour color, int indicator, wxStyledTextCtrl* ctrl)
{
    // indicator: can be between 8 and 32, sets color only for this indicator.

    // highlight a word
    ctrl->SetIndicatorCurrent(indicator);
    //ctrl->IndicatorClearRange(0, gui_text_view->GetLength());
    ctrl->IndicatorSetStyle(indicator, wxSTC_INDIC_STRAIGHTBOX);
    ctrl->IndicatorSetUnder(indicator, true);
    ctrl->IndicatorSetForeground(indicator, color);
    ctrl->IndicatorSetOutlineAlpha(indicator, 50);
    ctrl->IndicatorSetAlpha(indicator, 50);
}