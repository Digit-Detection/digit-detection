#include "wxwidget/developer/ShowDrawing.h"

ShowDrawing::ShowDrawing(double* grid, int Y, int X, int submitValue, int scale)
: wxFrame(nullptr, wxID_ANY, "Submission " + std::to_string(submitValue)) {
    this->gridCopy = new double[Y * X];
    for (int i = 0; i < Y * X; i++) {
        this->gridCopy[i] = grid[i];
    }
    DrawingPanel* panel = new DrawingPanel(this, gridCopy, Y, X, scale);
    
    panel->SetMinSize(wxSize(X * scale, Y * scale));
    
    // Create a sizer to manage layout
    wxBoxSizer* sizer = new wxBoxSizer(wxVERTICAL);
    sizer->Add(panel, 1, wxEXPAND);
    
    this->SetSizer(sizer);
    this->SetClientSize(X * scale, Y * scale);
    this->Layout(); // Force layout update
    this->Center();
    this->Show();
}
ShowDrawing::~ShowDrawing() {
    delete[] this->gridCopy;
}
