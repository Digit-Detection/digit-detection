#pragma once
#include "BasicButton.h"

class ClearButton : public BasicButton {
public:
    ClearButton(wxWindow parent, std::string buttonLabel);
    ~ClearButton();
private:
    void OnClick() override;
};