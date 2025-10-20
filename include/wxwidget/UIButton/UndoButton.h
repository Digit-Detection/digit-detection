#pragma once
#include "wxwidget/UIButton/FunctionButton.h"

class UndoButton : public FunctionButton {
public:
    UndoButton(wxWindow* parent, std::function<void()> onClick = nullptr);
    ~UndoButton();

protected:
    void OnClick(wxCommandEvent& event) override;
    void ApplyStyle();
};
