#include "wxwidget/UIButton/ClearButton.h"

ClearButton::ClearButton(wxWindow* parent, std::function<void()> onClick)
    : wxButton(parent, wxID_ANY, "CLEAR"), clearFunction(onClick)
{
    Bind(wxEVT_BUTTON, &ClearButton::OnClick, this);
}

ClearButton::~ClearButton() {}

void ClearButton::OnClick(wxCommandEvent& event) {
    // std::cout << "Cleared" << std::endl;
    if (clearFunction) clearFunction();
}
