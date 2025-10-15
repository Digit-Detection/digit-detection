#pragma once
#include <wx/wx.h>
#include <string>

class BasicButton : public wxButton {
public:
    BasicButton(wxWindow* parent, const std::string& buttonLabel);
    virtual ~BasicButton();

protected:
    virtual void OnClick(wxCommandEvent& event);

private:
    void BindEvents();
};
