#include <wx/wx.h>
#include <vector>
#include <wx/geometry.h>

class Canvas : public wxPanel{
    public:
        // Parent window, 
        Canvas(wxFrame* parent) : wxPanel(parent, wxID_ANY, wxDefaultPosition, wxSize(CANX * SCALE, CANY * SCALE)) {
            this->SetBackgroundColour(wxColour(wxColor(*wxWHITE))); // Set Canvas Color

            // Bind events
            Bind(wxEVT_PAINT, &Canvas::OnPaint, this);
                Bind(wxEVT_LEFT_DOWN, &Canvas::OnMouseDown, this);
                Bind(wxEVT_LEFT_UP, &Canvas::OnMouseUp, this);
                Bind(wxEVT_MOTION, &Canvas::OnMouseMove, this);
                Bind(wxEVT_PAINT, &Canvas::OnPaint, this);

        }
        ~Canvas() {}

    private:
        int lastX = -1, lastY = -1; // Last mouse pos
        double grid[CANY][CANX] = {0}; // initialize Pixel size
        bool drawing = false; // Is holding left click

        void OnMouseDown(wxMouseEvent& evt) {
            drawing = true;
        }
        void OnMouseUp(wxMouseEvent& evt) {
            drawing = false;
            lastX = -1;
            lastY = -1;
        }

        void OnMouseMove(wxMouseEvent& evt) {
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
                        grid[cy][cx] = 1;
                        if (cx == x && cy == y) break;
                        int e2 = 2 * err;
                        if (e2 > -dy) { err -= dy; cx += sx; }
                        if (e2 < dx) { err += dx; cy += sy; }
                    }
                } else {
                    grid[y][x] = 1;
                }

                lastX = x;
                lastY = y;
                // Update Frame
                Refresh();
            }
        }
        
        // Frame update
        void OnPaint(wxPaintEvent& evt) {
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
        

};
