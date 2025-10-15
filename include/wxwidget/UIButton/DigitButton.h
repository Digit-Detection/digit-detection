#pragma once
#include "BasicButton.h"
#include <string>

class DigitButton : public wxButton {
public:
    DigitButton(wxWindow* parent, std::string title);
    ~DigitButton();
    static double* grid;

private:
    int submitValue;
    void OnClick(wxCommandEvent& event);
};
