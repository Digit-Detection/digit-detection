#pragma once
#include <wx/wx.h>
#include <string>
#include "wxwidget/developer/DrawingPanel.h"
class ShowDrawing : public wxFrame {
private:
    double* gridCopy;
public:    
    ShowDrawing(double* grid, int Y, int X, int submitValue, int scale);
    ~ShowDrawing();
};