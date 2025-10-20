#pragma once
#include <wx/wx.h>
#include <string>
#include <functional>
class FunctionButton : public wxButton {
public:
    FunctionButton(wxWindow* parent, const std::string& buttonLabel);
    virtual ~FunctionButton();

protected:
    std::function<void()> functionBehaviour;
    virtual void FunctionLink(std::function<void()> onClick);
    virtual void OnClick(wxCommandEvent& event);
    void BindEvents();
};
