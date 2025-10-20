#pragma once
#include "wxwidget/UIButton/FunctionButton.h"
#include "wxwidget/Canvas.h"
#include <string>

class DigitButton : public wxButton {
public:
    DigitButton(wxWindow* parent, std::string title);
    ~DigitButton();
    static Canvas* canvasParent;

private:
    int submitValue;
    void OnClick(wxCommandEvent& event);
};
