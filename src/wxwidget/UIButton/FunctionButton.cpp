#include <wx/wx.h>
#include "wxwidget/UIButton/FunctionButton.h"
#include <iostream>

// std::function<void()> functionBehaviour;

FunctionButton::FunctionButton(wxWindow* parent, const std::string& buttonLabel)
    : wxButton(parent, wxID_ANY, buttonLabel)
{
    BindEvents();
}

FunctionButton::~FunctionButton() {}

void FunctionButton::FunctionLink(std::function<void()> functionToLink) {
    this->functionBehaviour = functionToLink;
}
void FunctionButton::BindEvents() {
    Bind(wxEVT_BUTTON, &FunctionButton::OnClick, this);
}

void FunctionButton::OnClick(wxCommandEvent& event) {
    // Default behavior (can be overridden)
    // std::cout << "Button Clicked!" << std::endl;
    if (this->functionBehaviour) this->functionBehaviour();
}

