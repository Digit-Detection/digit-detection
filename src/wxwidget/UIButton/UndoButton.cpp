#include "wxwidget/UIButton/UndoButton.h"
#include <iostream>
UndoButton::UndoButton(wxWindow* parent, std::function<void()> onClick)
    : wxButton(parent, wxID_ANY, "UNDO"), undoFunction(onClick)
{
    Bind(wxEVT_BUTTON, &UndoButton::OnClick, this);
}

UndoButton::~UndoButton() {}

void UndoButton::OnClick(wxCommandEvent& event) {
    std::cout << "Undone" << std::endl;
    if (undoFunction) undoFunction();
}
