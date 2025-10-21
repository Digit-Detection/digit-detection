#include <wxwidget/BrushPicker.h>
#include <iostream>
#include "constants.h"
/*
private:
    std::function<void()> canvasLink;
*/
BrushPicker::BrushPicker(wxWindow* parent, std::function<void(int)> canvasFunction) : wxSpinCtrl(parent, wxID_ANY, "") {
    int min_size = CONSTANTS_H::CANX / 20;
    this->SetRange(min_size, CONSTANTS_H::CANX / 10);
    this->SetValue(min_size); // initial brush size
    this->LinkCanvas(canvasFunction);
    Bind(wxEVT_SPINCTRL, &BrushPicker::OnChange, this);
    canvasFunction(min_size);
}
BrushPicker::~BrushPicker() {}

void BrushPicker::LinkCanvas(std::function<void(int)> canvasFunction) {
    this->canvasLink = canvasFunction;
}

void BrushPicker::OnChange(wxCommandEvent& event) {
    this->canvasLink(event.GetInt());
    // std::cout << "Brush Size changed to " << event.GetInt() << std::endl;
}



