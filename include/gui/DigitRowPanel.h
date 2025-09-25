#pragma once
#include <wx/wx.h>

class DigitRowPanel : public wxPanel {
private:
    wxButton** buttonList;
public:
    DigitRowPanel(wxWindow& parent);
    ~DigitRowPanel();
};
