#pragma once
#include "BasicButton.h"
#include <functional>
class ClearButton : public wxButton {
public:
    ClearButton(wxWindow* parent, std::function<void()> onClick);
    ~ClearButton();

private:
    std::function<void()> clearFunction;
    void OnClick(wxCommandEvent& event);


};