#include "wxwidget/developer/DrawingPanel.h"
#include <algorithm> // Clamp

DrawingPanel::DrawingPanel(wxFrame* parent, double* grid, int Y, int X, int scale) 
: wxPanel(parent, wxID_ANY, wxDefaultPosition, wxDefaultSize) {
    this->canvasY = Y;
    this->canvasX = X;
    this->grid = grid;
    this->scale = scale;
    
    SetBackgroundColour(*wxWHITE); // Add white background
    Bind(wxEVT_PAINT, &DrawingPanel::OnPaint, this);
}

DrawingPanel::~DrawingPanel() {}
    
void DrawingPanel::OnPaint(wxPaintEvent& evt) {
    wxPaintDC dc(this);

    // Paint white background first
    dc.SetBackground(*wxWHITE_BRUSH);
    dc.Clear();
    
    // Now draw the grid
    dc.SetPen(wxPen(*wxBLACK, 1));
    dc.SetBrush(wxBrush(wxColour(0, 0, 0)));
    
    int lastC = -1;
    wxColour lastColour;

    for (int y = 0; y < canvasY; ++y) {
        for (int x = 0; x < canvasX; ++x) {
            double value = grid[y * canvasX + x];
            int c = std::clamp(static_cast<int>(255 * (1.0 - value)), 0, 255);

            if (c != lastC) {
                lastColour.Set(c, c, c);
                dc.SetPen(lastColour);
                dc.SetBrush(lastColour);
                lastC = c;
            }

            dc.DrawRectangle(x * scale, y * scale, scale, scale);
        }
    }

}