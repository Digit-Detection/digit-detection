#include <wx/wx.h>
#include "wxwidget/UIButton/BasicButton.h"
#include <iostream>
BasicButton::BasicButton(wxWindow* parent, const std::string& buttonLabel)
    : wxButton(parent, wxID_ANY, buttonLabel)
{
    BindEvents();
}

BasicButton::~BasicButton() {}

void BasicButton::BindEvents() {
    Bind(wxEVT_BUTTON, &BasicButton::OnClick, this);
}

void BasicButton::OnClick(wxCommandEvent& event) {
    // Default behavior (can be overridden)
    std::cout << "Basic Button Clicked" << std::endl;
}
