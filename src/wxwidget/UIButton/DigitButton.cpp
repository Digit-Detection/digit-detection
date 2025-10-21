#include "wxwidget/UIButton/DigitButton.h"
#include "constants.h"
#include "neural/Data_Handling/DataPoint.h"
#include "neural/Data_Handling/DataSet.h"
#include "neural/Data_Handling/CanvasConverter.h"
#include "neural/Data_Handling/Augmentations.h"

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
        // Convert current resampled grid into multiple augmented DataPoints and append to dataset file
        double* resampled = DigitButton::canvasParent->get_resampled_grid();
        if (resampled) {
            try {
                auto augmented = Augmentations::GenerateAugmentedDataPoints(resampled, CONSTANTS_H::DESTX, CONSTANTS_H::DESTY, this->submitValue, CONSTANTS_H::NUMDIGITS);
                int saved = 0;
                for (auto dp : augmented) {
                    DataSet::AppendDataPoint(dp, "data/user_drawings.bin");
                    ++saved;
                    delete dp;
                }
                std::cout << "Saved " << saved << " augmented drawings as label " << this->submitValue << " to data/user_drawings.bin" << std::endl;
            } catch (const std::exception& ex) {
                std::cerr << "Failed to save augmented drawings: " << ex.what() << std::endl;
            }
        } else {
            std::cerr << "No resampled grid available to save." << std::endl;
        }
        
        // Remove submission on clear()
        DigitButton::canvasParent->ClearCanvas();
    } else {
        std::cout << "Nothing to submit!" << std::endl;
    }
}
