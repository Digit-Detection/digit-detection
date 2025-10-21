#include <wx/wx.h>
#include "constants.h"
#include <iostream>
#include <string>
class DigitButton : public wxButton {
    public:
        DigitButton(wxWindow* parent, std::string digit) 
        : wxButton(parent, wxID_ANY, digit)
            {
                SetFont(wxFont(14, wxFONTFAMILY_DEFAULT, wxFONTSTYLE_NORMAL, wxFONTWEIGHT_BOLD));
                SetBackgroundColour(wxColour(240, 240, 240));
                Bind(wxEVT_BUTTON, &DigitButton::OnClick, this);
                try {
                    digitValue = std::stoi(digit);
                } catch (...) {
                    digitValue = 0;
                }
                
            }
        ~DigitButton() {}
        static int digitValue;
    protected:
        

        void OnClick(wxCommandEvent& event) {
            std::cout << "Clicked " << digitValue << std::endl;
        }
};
int DigitButton::digitValue = 0;

class NumPad : public wxPanel {
    public:
        NumPad(wxWindow* parent) : wxPanel(parent, wxID_ANY) {
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
            
            SetSizerAndFit(mainSizer);

        }
        ~NumPad() {}
};

class Frame : public wxFrame {
    public:
        // Create unresizeable frame
        Frame(std::string windowName) : wxFrame(nullptr, wxID_ANY, windowName, wxDefaultPosition, wxSize(CANX * CONSTANTS_H::CANSCALE, CANY * CONSTANTS_H::CANSCALE), wxDEFAULT_FRAME_STYLE & ~(wxRESIZE_BORDER | wxMAXIMIZE_BOX)) {
            myNumPad = new NumPad(this);
            myNumPad->SetMinSize(wxSize(CONSTANTS_H::CANX * CONSTANTS_H::CANSCALE, CONSTANTS_H::CANY * CONSTANTS_H::CANSCALE));

            wxBoxSizer* sizer = new wxBoxSizer(wxVERTICAL);
            sizer->Add(myNumPad, 0, wxALIGN_CENTER);
            SetSizerAndFit(sizer);
        }

        ~Frame() {}
    private:
        NumPad* myNumPad;
};

class MainApp : public wxApp {
    public:
        bool OnInit() override {
            this->myFrame = new Frame("Button Test");
            this->myFrame->Show();
            return true;
        }
        ~MainApp() {}
    private: 
        Frame* myFrame;
};

wxIMPLEMENT_APP(MainApp);