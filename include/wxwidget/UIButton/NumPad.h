#pragma once
#include "wxwidget/UIButton/DigitButton.h"

class NumPad : public wxPanel {
    public:
        NumPad(wxWindow* parent, double* grid);
        ~NumPad();
};