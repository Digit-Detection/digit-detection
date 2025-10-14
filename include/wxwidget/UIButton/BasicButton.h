#pragma once
#include <string>
#include <wx/wx.h>

class BasicButton : public wxButton {
public:
    BasicButton(wxWindow parent, std::string buttonLabel);
    ~BasicButton();
private:
    virtual void OnClick();
};