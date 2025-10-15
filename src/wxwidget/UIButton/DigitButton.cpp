#include "wxwidget/UIButton/DigitButton.h"
#include "constants.h"
#include "neural/Data_Handling/DataPoint.h"
/* 
protected:
    int submitValue;
    double* grid;
    void OnClick(wxCommandEvent& event);
*/
double* DigitButton::grid = nullptr;

DigitButton::DigitButton(wxWindow* parent, std::string digit)
    : wxButton(parent, wxID_ANY, digit)
{
    SetFont(wxFont(14, wxFONTFAMILY_DEFAULT, wxFONTSTYLE_NORMAL, wxFONTWEIGHT_BOLD));
    SetBackgroundColour(wxColour(240, 240, 240));
    Bind(wxEVT_BUTTON, &DigitButton::OnClick, this);

    try {
        this->submitValue = std::stoi(digit);
    } catch (...) {
        this->submitValue = 0;
    }
}

DigitButton::~DigitButton() {}

void DigitButton::OnClick(wxCommandEvent& event) {
    std::cout << "Clicked and submitted " << this->submitValue << std::endl;
    // for (int i = 0; i < CONSTANTS_H::CANY; i++) {
    //     for (int j = 0; j < CONSTANTS_H::CANX; j++) {
    //         std::cout << grid[i * CONSTANTS_H::CANX + j] << " ";
    //     }
    //     std::cout << std::endl;
    // }
    
    // DataPoint(double* inputs, int inputs_length, int label, int num_labels);
    // DataPoint(grid, CONSTANTS_H::CANY * CONSTANTS_H::CANX, this->submitValue, 10);
}
