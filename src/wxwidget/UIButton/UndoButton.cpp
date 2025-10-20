#include "wxwidget/UIButton/UndoButton.h"
#include <wx/colour.h>
#include <iostream>

UndoButton::UndoButton(wxWindow* parent, std::function<void()> onClick)
    : FunctionButton(parent, "UNDO")
{
    this->FunctionLink(onClick);
    ApplyStyle();
}

UndoButton::~UndoButton() {}

void UndoButton::OnClick(wxCommandEvent& event) {
    FunctionButton::OnClick(event);
}

void UndoButton::ApplyStyle() {
    SetBackgroundColour(wxColour(200, 200, 255)); // Light blue
    SetForegroundColour(*wxBLACK);
}
