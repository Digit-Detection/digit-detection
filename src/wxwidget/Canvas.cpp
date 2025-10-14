#include "wxwidget/Canvas.h"

/*
private:
    int lastX;
    int lastY; // Last mouse pos
    double grid[CANY][CANX]; // initialize Pixel size
    bool drawing; // Is holding left click
*/

// Public functions
Canvas::Canvas(wxFrame* parent) : 
    wxPanel(parent, wxID_ANY, 
    wxDefaultPosition, 
    wxSize(CANX * SCALE, CANY * SCALE)) 
    {   
        // Initialize Variables
        this->lastX = -1;
        this->lastY = -1;
        this->grid[CANY][CANX] = {0}; // initialize empty state
        this->drawing = false; 

        // Default Canvas background color
        this->SetBackgroundColour(wxColour(wxColor(*wxWHITE)));

        // Bind events to corresponding function
        Bind(wxEVT_PAINT, &Canvas::OnPaint, this);
        Bind(wxEVT_LEFT_DOWN, &Canvas::OnMouseDown, this);
        Bind(wxEVT_LEFT_UP, &Canvas::OnMouseUp, this);
        Bind(wxEVT_MOTION, &Canvas::OnMouseMove, this);
        Bind(wxEVT_PAINT, &Canvas::OnPaint, this);
    }
Canvas::~Canvas() {}

// Private Functions
void Canvas::OnMouseDown(wxMouseEvent& evt) {
    drawing = true;
}
bool Canvas::MouseRange(const int& y, const int& x) {
    bool validY = (y >= 0) && (y < CANY);
    bool validX = (x >= 0) && (x < CANX);
    return validY && validX;
}

void Canvas::OnMouseUp(wxMouseEvent& evt) {
    drawing = false;
    lastX = -1;
    lastY = -1;
}

void Canvas::OnMouseMove(wxMouseEvent& evt) {
    if (drawing) { // Mouse held
        int x = evt.GetX() / SCALE;
        int y = evt.GetY() / SCALE;

        if (lastX != -1 && lastY != -1) {
            // Bresenham's line algorithm
            int dx = abs(x - lastX), dy = abs(y - lastY);
            int sx = (lastX < x) ? 1 : -1;
            int sy = (lastY < y) ? 1 : -1;
            int err = dx - dy;

            int cx = lastX, cy = lastY;
            while (true) {
                if (MouseRange(cy, cx)) {
                    grid[cy][cx] = 1;
                }

                if (cx == x && cy == y) break;

                int e2 = 2 * err;
                if (e2 > -dy) { err -= dy; cx += sx; }
                if (e2 < dx) { err += dx; cy += sy; }
            }

        } else {
            if (MouseRange(y, x)) {
                grid[y][x] = 1;
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

    for (int y = 0; y < CANY; ++y) {
        for (int x = 0; x < CANX; ++x) {
            if (grid[y][x]) {
                dc.DrawRectangle(x * SCALE, y * SCALE, SCALE, SCALE);
            }
        }
    }
}   

