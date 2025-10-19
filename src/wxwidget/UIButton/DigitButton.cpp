#include "wxwidget/UIButton/DigitButton.h"
#include "constants.h"
#include "neural/Data_Handling/DataPoint.h"
#include "neural/Data_Handling/DataSet.h"
#include "neural/Data_Handling/CanvasConverter.h"
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
    // double* grid;
    // int submitValue;
    
    // for (int i = 0; i < CONSTANTS_H::CANY; i++) {
    //     for (int j = 0; j < CONSTANTS_H::CANX; j++) {
    //         std::cout << grid[i * CONSTANTS_H::CANX + j] << " ";
    //     }
    //     std::cout << std::endl;
    // }
    
    // Convert current grid into a DataPoint and append to dataset file
    if (DigitButton::grid != nullptr) {
        try {
            DataPoint* dp = CanvasConverter::GridToDataPoint(DigitButton::grid, CONSTANTS_H::CANX, CONSTANTS_H::CANY, this->submitValue, CONSTANTS_H::NUMDIGITS, 28);
            // Append to file
            DataSet::AppendDataPoint(dp, "user_drawings.bin");
            std::cout << "Saved drawing as label " << this->submitValue << " to user_drawings.bin" << std::endl;
            delete dp;
        } catch (const std::exception& ex) {
            std::cerr << "Failed to save drawing: " << ex.what() << std::endl;
        }
    } else {
        std::cerr << "No grid available to save." << std::endl;
    }
}
