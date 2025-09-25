#include "gui/DigitButton.h"

DigitButton::DigitButton(wxPanel& parent, int digitValue) : 
    wxButton(parent, wxID_ANY, to_string(digitValue)) 
    {
    this->digitValue = digitValue;
    
}

DigitButton::~DigitButton() {

}