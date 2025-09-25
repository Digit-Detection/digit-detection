#pragma once
#include <string>
#include <wx/wx.h>

class DigitButton : public wxButton {
private:
    int digitValue;
public:
    DigitButton(wxPanel& parent, int digitValue);
    ~DigitButton();
};
