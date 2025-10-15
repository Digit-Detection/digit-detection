#include <wx/wx.h>
#include <vector>
#include "constants.h"

// Compile with respect to include folder
// g++ myDrawTest.cpp -I../include `wx-config --cxxflags --libs`

class Canvas : public wxPanel{
    public:
        // Parent window, 
        Canvas(wxFrame* parent) : wxPanel(parent, wxID_ANY, wxDefaultPosition, wxSize(CANX * CONSTANTS_H::SCALE, CANY * CONSTANTS_H::SCALE)) {
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
        double grid[CONSTANTS_H::CANY][CONSTANTS_H::CANX] = {0}; // initialize Pixel size
        bool drawing = false; // Is holding left click

        void OnMouseDown(wxMouseEvent& evt) {
            drawing = true;
        }
        bool MouseRange(const int& y, const int& x) {
            bool validY = (y >= 0) && (y < CONSTANTS_H::CANY);
            bool validX = (x >= 0) && (x < CONSTANTS_H::CANX);
            return validY && validX;
        }

        void OnMouseUp(wxMouseEvent& evt) {
            drawing = false;
            lastX = -1;
            lastY = -1;
        }

        void OnMouseMove(wxMouseEvent& evt) {
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
        void OnPaint(wxPaintEvent& evt) {
            wxPaintDC dc(this);
            dc.Clear();
            dc.SetPen(wxPen(*wxBLACK, 1)); // Border
            dc.SetBrush(wxBrush(wxColour(0, 0, 0))); // Fill

            for (int y = 0; y < CONSTANTS_H::CANY; ++y) {
                for (int x = 0; x < CONSTANTS_H::CANX; ++x) {
                    if (grid[y][x]) {
                        dc.DrawRectangle(x * CONSTANTS_H::SCALE, y * CONSTANTS_H::SCALE, CONSTANTS_H::SCALE, CONSTANTS_H::SCALE);
                    }
                }
            }
        }
        

};

class Frame : public wxFrame {
    public:
        // Create unresizeable frame
        Frame(std::string windowName) : wxFrame(nullptr, wxID_ANY, windowName, wxDefaultPosition, wxSize(CANX * CONSTANTS_H::SCALE, CANY * CONSTANTS_H::SCALE), wxDEFAULT_FRAME_STYLE & ~(wxRESIZE_BORDER | wxMAXIMIZE_BOX)) {
            myCanvas = new Canvas(this);
            myCanvas->SetMinSize(wxSize(CONSTANTS_H::CANX * CONSTANTS_H::SCALE, CONSTANTS_H::CANY * CONSTANTS_H::SCALE));

            wxBoxSizer* sizer = new wxBoxSizer(wxVERTICAL);
            sizer->Add(myCanvas, 0, wxALIGN_CENTER);
            SetSizerAndFit(sizer);
        }

        ~Frame() {}
    private:
        Canvas* myCanvas;
};

class MainApp : public wxApp {
    public:
        bool OnInit() override {
            this->myFrame = new Frame("Drawing Software");
            this->myFrame->Show();
            return true;
        }
        ~MainApp() {}
    private: 
        Frame* myFrame;
};

wxIMPLEMENT_APP(MainApp);