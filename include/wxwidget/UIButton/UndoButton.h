#pragma once
#include "BasicButton.h"

class UndoButton : public wxButton {
public:
    UndoButton(wxWindow* parent, std::function<void()> onClick);
    ~UndoButton();

private:
    std::function<void()> undoFunction;
    void OnClick(wxCommandEvent& event);


};