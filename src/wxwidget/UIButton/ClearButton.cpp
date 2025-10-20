#include "wxwidget/UIButton/ClearButton.h"
#include <wx/colour.h>
#include <iostream>

ClearButton::ClearButton(wxWindow* parent, std::function<void()> onClick)
    : FunctionButton(parent, "CLEAR")
{
    this->FunctionLink(onClick);
    ApplyStyle();
}

ClearButton::~ClearButton() {}

void ClearButton::OnClick(wxCommandEvent& event) {
    FunctionButton::OnClick(event); // Call base behavior
}

void ClearButton::ApplyStyle() {
    SetBackgroundColour(wxColour(255, 200, 200)); // Light red
    SetForegroundColour(*wxBLACK);
}
