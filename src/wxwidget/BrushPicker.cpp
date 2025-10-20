#include <wxwidget/BrushPicker.h>
#include <iostream>
/*
private:
    std::function<void()> canvasLink;
*/
BrushPicker::BrushPicker(wxWindow* parent, int maxSize, std::function<void(int)> canvasFunction) : wxSpinCtrl(parent, wxID_ANY, "") {
    this->SetRange(1, maxSize);
    this->SetValue(1); // initial brush size
    this->LinkCanvas(canvasFunction);
    Bind(wxEVT_SPINCTRL, &BrushPicker::OnChange, this);
}
BrushPicker::~BrushPicker() {}

void BrushPicker::LinkCanvas(std::function<void(int)> canvasFunction) {
    this->canvasLink = canvasFunction;
}

void BrushPicker::OnChange(wxCommandEvent& event) {
    this->canvasLink(event.GetInt());
    // std::cout << "Brush Size changed to " << event.GetInt() << std::endl;
}



