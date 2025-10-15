#include "wxwidget/Canvas.h"
#include "constants.h"
/*
private:
    int lastX;
    int lastY; // Last mouse pos
    double* grid;
    bool drawing; // Is holding left click
*/

// Public functions
Canvas::Canvas(wxPanel* parent) : 
    wxPanel(parent, wxID_ANY, 
    wxDefaultPosition, 
    wxSize(CONSTANTS_H::CANX * CONSTANTS_H::SCALE, CONSTANTS_H::CANY * CONSTANTS_H::SCALE)) 
    {   
        // Initialize Variables
        this->lastX = -1;
        this->lastY = -1;
        this->drawing = false; 
        this->grid = new double[CONSTANTS_H::CANY * CONSTANTS_H::CANX];
        for (int i = 0; i < CONSTANTS_H::CANY * CONSTANTS_H::CANX; i++) {
            grid[i] = 0.0;
        }

        // Default Canvas background color
        this->SetBackgroundColour(wxColour(wxColor(*wxWHITE)));

        // Bind events to corresponding function
        Bind(wxEVT_PAINT, &Canvas::OnPaint, this);
        Bind(wxEVT_LEFT_DOWN, &Canvas::OnMouseDown, this);
        Bind(wxEVT_LEFT_UP, &Canvas::OnMouseUp, this);
        Bind(wxEVT_MOTION, &Canvas::OnMouseMove, this);
        Bind(wxEVT_PAINT, &Canvas::OnPaint, this);
    }
Canvas::~Canvas() {
    delete[] grid;
}

// Private Functions
void Canvas::OnMouseDown(wxMouseEvent& evt) {
    drawing = true;
    int x = evt.GetX() / CONSTANTS_H::SCALE;
    int y = evt.GetY() / CONSTANTS_H::SCALE;
    grid[y * CONSTANTS_H::CANX + x] = 1;
    Refresh();
}
bool Canvas::MouseRange(const int& y, const int& x) {
    bool validY = (y >= 0) && (y < CONSTANTS_H::CANY);
    bool validX = (x >= 0) && (x < CONSTANTS_H::CANX);
    return validY && validX;
}

void Canvas::OnMouseUp(wxMouseEvent& evt) {
    drawing = false;
    lastX = -1;
    lastY = -1;
}

void Canvas::OnMouseMove(wxMouseEvent& evt) {
    if (drawing) { // Mouse held
        int x = evt.GetX() / CONSTANTS_H::SCALE;
        int y = evt.GetY() / CONSTANTS_H::SCALE;

        if (lastX != -1 && lastY != -1) {
            // Bresenham's line algorithm
            int dx = abs(x - lastX), dy = abs(y - lastY);
            int sx = (lastX < x) ? 1 : -1;
            int sy = (lastY < y) ? 1 : -1;
            int err = dx - dy;

            int cx = lastX, cy = lastY;
            while (true) {
                if (MouseRange(cy, cx)) {
                    grid[cy * CONSTANTS_H::CANX + cx] = 1;
                }

                if (cx == x && cy == y) break;

                int e2 = 2 * err;
                if (e2 > -dy) { err -= dy; cx += sx; }
                if (e2 < dx) { err += dx; cy += sy; }
            }

        } else {
            if (MouseRange(y, x)) {
                grid[y * CONSTANTS_H::CANX + x] = 1;
            }

        }

        lastX = x;
        lastY = y;
        // Update Frame
        Refresh();
    }
}

// Frame update
void Canvas::OnPaint(wxPaintEvent& evt) {
    wxPaintDC dc(this);
    dc.Clear();
    dc.SetPen(wxPen(*wxBLACK, 1)); // Border
    dc.SetBrush(wxBrush(wxColour(0, 0, 0))); // Fill

    for (int y = 0; y < CONSTANTS_H::CANY; ++y) {
        for (int x = 0; x < CONSTANTS_H::CANX; ++x) {
            if (grid[y * CONSTANTS_H::CANX + x]) {
                dc.DrawRectangle(x * CONSTANTS_H::SCALE, y * CONSTANTS_H::SCALE, CONSTANTS_H::SCALE, CONSTANTS_H::SCALE);
            }
        }
    }

}   


// Bindable
void Canvas::ClearCanvas() {
    for (int i = 0; i < CONSTANTS_H::CANX * CONSTANTS_H::CANY; i++) {
        grid[i] = 0;
    }
    Refresh();
}

// encapsulation
double* Canvas::get_grid() {
    return this->grid;
}
bool Canvas::get_drawing_state() {
    return this->drawing;
}