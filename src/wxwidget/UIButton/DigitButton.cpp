#include "wxwidget/UIButton/DigitButton.h"
#include "constants.h"
#include "neural/Data_Handling/DataPoint.h"
#include "neural/Data_Handling/DataSet.h"
#include "neural/Data_Handling/CanvasConverter.h"

/* 
public:
    static Canvas* canvasParent;
protected:
    int submitValue;
    void OnClick(wxCommandEvent& event);
*/
Canvas* DigitButton::canvasParent = nullptr;

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
    if (!DigitButton::canvasParent) {
        std::cout << "This panel is not linked to a canvas!" << std::endl;
        return;
    }
    // Deep copy to history
    
    // prevent submission of empty grid
    if (!DigitButton::canvasParent->is_empty()) {
        // Convert current grid into a DataPoint and append to dataset file
        if (DigitButton::canvasParent->get_grid()) {
            try {
                DataPoint* dp = CanvasConverter::GridToDataPoint(DigitButton::canvasParent->get_grid(), CONSTANTS_H::CANX, CONSTANTS_H::CANY, this->submitValue, CONSTANTS_H::NUMDIGITS, 28);
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
        
        // Remove submission on clear()
        DigitButton::canvasParent->ClearCanvas();
    } else {
        std::cout << "Nothing to submit!" << std::endl;
    }
}
