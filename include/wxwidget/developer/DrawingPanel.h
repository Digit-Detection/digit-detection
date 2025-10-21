#pragma once
#include <wx/wx.h>
#include <string>
class DrawingPanel : public wxPanel {
private:
    int canvasY;
    int canvasX;
    double* grid;
    int scale;

    void OnPaint(wxPaintEvent& evt);
public:
    DrawingPanel(wxFrame* parent, double* grid, int Y, int X, int scale);
    ~DrawingPanel();
};