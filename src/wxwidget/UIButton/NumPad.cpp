#include "wxwidget/UIButton/NumPad.h"
#include "wxwidget/UIButton/DigitButton.h"

NumPad::NumPad(wxWindow* parent, Canvas* canvasParent) : wxPanel(parent, wxID_ANY) {
    DigitButton* butt1 = new DigitButton(this, "1");
    DigitButton* butt2 = new DigitButton(this, "2");
    DigitButton* butt3 = new DigitButton(this, "3");
    DigitButton* butt4 = new DigitButton(this, "4");
    DigitButton* butt5 = new DigitButton(this, "5");
    DigitButton* butt6 = new DigitButton(this, "6");
    DigitButton* butt7 = new DigitButton(this, "7");
    DigitButton* butt8 = new DigitButton(this, "8");
    DigitButton* butt9 = new DigitButton(this, "9");
    DigitButton* butt0 = new DigitButton(this, "0");
    DigitButton::canvasParent = canvasParent;
    
    // First Row
    wxBoxSizer* firstSizer = new wxBoxSizer(wxHORIZONTAL);
    firstSizer->Add(butt1, 0, wxALIGN_CENTER, 2);
    firstSizer->Add(butt2, 0, wxALIGN_CENTER, 2);
    firstSizer->Add(butt3, 0, wxALIGN_CENTER, 2);
    
    // Second Row
    wxBoxSizer* secondSizer = new wxBoxSizer(wxHORIZONTAL);
    secondSizer->Add(butt4, 0, wxALIGN_CENTER, 2);
    secondSizer->Add(butt5, 0, wxALIGN_CENTER, 2);
    secondSizer->Add(butt6, 0, wxALIGN_CENTER, 2);
    
    // Third Row
    wxBoxSizer* thirdSizer = new wxBoxSizer(wxHORIZONTAL);
    thirdSizer->Add(butt7, 0, wxALIGN_CENTER, 2);
    thirdSizer->Add(butt8, 0, wxALIGN_CENTER, 2);
    thirdSizer->Add(butt9, 0, wxALIGN_CENTER, 2);
    
    // Fourth Row
    wxBoxSizer* fourthSizer = new wxBoxSizer(wxHORIZONTAL);
    fourthSizer->Add(butt0, 0, wxALIGN_CENTER, 2);
    
    wxBoxSizer* mainSizer = new wxBoxSizer(wxVERTICAL);
    mainSizer->Add(firstSizer, 0, wxALIGN_CENTER | wxALL, 5);
    mainSizer->Add(secondSizer, 0, wxALIGN_CENTER | wxALL, 5);
    mainSizer->Add(thirdSizer, 0, wxALIGN_CENTER | wxALL, 5);
    mainSizer->Add(fourthSizer, 0, wxALIGN_CENTER | wxALL, 5);
    
    this->SetSizer(mainSizer);
}

NumPad::~NumPad() {}