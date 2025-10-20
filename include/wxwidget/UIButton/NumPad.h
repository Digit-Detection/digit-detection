#pragma once
#include "wxwidget/UIButton/DigitButton.h"
#include "wxwidget/Canvas.h"
class NumPad : public wxPanel {
    public:
        NumPad(wxWindow* parent, Canvas* canvasParent);
        ~NumPad();
};