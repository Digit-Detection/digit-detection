#pragma once
#include "wxwidget/UIButton/FunctionButton.h"

class ClearButton : public FunctionButton {
public:
    ClearButton(wxWindow* parent, std::function<void()> onClick = nullptr);
    ~ClearButton();

protected:
    void OnClick(wxCommandEvent& event) override;
    void ApplyStyle();
};
