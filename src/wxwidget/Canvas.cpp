#include "wxwidget/Canvas.h"
#include "neural/Network/NeuralNetwork.h"
#include "neural/Data_Handling/CanvasConverter.h"
#include "constants.h"
#include <iostream>
/*
private:
    int lastX;
    int lastY; // Last mouse pos
    double* grid;
    bool drawing; // Is holding left click
    deque<double*> drawingHistory;
*/

// Public functions
Canvas::Canvas(wxPanel* parent, std::function<void(double*)> updateLeaderboardFunc) : 
    wxPanel(parent, wxID_ANY, 
    wxDefaultPosition, 
    wxDefaultSize) 
    {   
        // Set minimum size instead
        this->SetMinSize(wxSize(CONSTANTS_H::CANX * CONSTANTS_H::CANSCALE, CONSTANTS_H::CANY * CONSTANTS_H::CANSCALE));
        
        // Initialize Variables
        this->brushSize = 1;
        this->lastX = -1;
        this->lastY = -1;
        this->drawing = false; 
        this->grid = new double[CONSTANTS_H::CANY * CONSTANTS_H::CANX];
        for (int i = 0; i < CONSTANTS_H::CANY * CONSTANTS_H::CANX; i++) {
            grid[i] = 0.0;
        }
        // Allocate resampled buffer used for continuous updates (DESTX x DESTY)
        this->resampledGrid = new double[CONSTANTS_H::DESTX * CONSTANTS_H::DESTY];
        
        // initialize resampled buffer
        for (int i = 0; i < CONSTANTS_H::DESTX * CONSTANTS_H::DESTY; ++i) this->resampledGrid[i] = 0.0;
        this->lastDrew = false;
        // Create first history instance
        double* copy = new double[CONSTANTS_H::CANY * CONSTANTS_H::CANX];
        for (int i = 0; i < CONSTANTS_H::CANY * CONSTANTS_H::CANX; i++) {
            copy[i] = grid[i];
        }
        this->drawingHistory.push_front(copy);
        // link updateleaderboard function 
        this->updateLeaderboard = updateLeaderboardFunc;
        // Default Canvas background color
        this->SetBackgroundColour(wxColour(wxColor(*wxWHITE)));
        // Bind events to corresponding function
        Bind(wxEVT_PAINT, &Canvas::OnPaint, this);
        Bind(wxEVT_LEFT_DOWN, &Canvas::OnMouseDown, this);
        Bind(wxEVT_LEFT_UP, &Canvas::OnMouseUp, this);
        Bind(wxEVT_MOTION, &Canvas::OnMouseMove, this);
    }
Canvas::~Canvas() {
    delete[] grid;
    delete[] resampledGrid;
    while (!this->drawingHistory.empty()) {
        double* item = this->drawingHistory.back();
        this->drawingHistory.pop_back();
        delete[] item;
    }
}

// Private Functions
void Canvas::OnMouseDown(wxMouseEvent& evt) {
    //std::cout << "Mouse Down" << std::endl;
    drawing = true;
    int x = evt.GetX() / CONSTANTS_H::CANSCALE;
    int y = evt.GetY() / CONSTANTS_H::CANSCALE;
    if (grid[y * CONSTANTS_H::CANX + x] == 0) {
        this->lastDrew = true;
    }
    this->DrawGrid(y, x);
    Refresh();
}

bool Canvas::MouseRange(const int& y, const int& x) {
    bool validY = (y >= 0) && (y < CONSTANTS_H::CANY);
    bool validX = (x >= 0) && (x < CONSTANTS_H::CANX);
    return validY && validX;
}

void Canvas::OnMouseUp(wxMouseEvent& evt) {
    //std::cout << "Mouse Up" << std::endl;
    drawing = false;
    lastX = -1;
    lastY = -1;
    StoreState();
    // Resample the high-resolution canvas into the ML input resolution and
    // notify the leaderboard/predictor with the latest resample.
    UpdateResampled();
    this->updateLeaderboard(this->resampledGrid);
}

void Canvas::OnMouseMove(wxMouseEvent& evt) {
    if (drawing) { // Mouse held
        int x = evt.GetX() / CONSTANTS_H::CANSCALE;
        int y = evt.GetY() / CONSTANTS_H::CANSCALE;

        if (lastX != -1 && lastY != -1) {
            // Bresenham's line algorithm
            int dx = abs(x - lastX), dy = abs(y - lastY);
            int sx = (lastX < x) ? 1 : -1;
            int sy = (lastY < y) ? 1 : -1;
            int err = dx - dy;

            int cx = lastX, cy = lastY;
            while (true) {
                if (MouseRange(cy, cx)) {
                    if (grid[cy * CONSTANTS_H::CANX + cx] == 0) {
                        this->lastDrew = true;
                    }
                    this->DrawGrid(cy, cx);
                }

                if (cx == x && cy == y) {
                    break;
                }

                int e2 = 2 * err;
                if (e2 > -dy) {
                    err -= dy; cx += sx;
                }
                if (e2 < dx) {
                    err += dx; cy += sy;
                }
            }

        } else {
            if (MouseRange(y, x)) {
                if (grid[y * CONSTANTS_H::CANX + x] == 0) {
                    this->lastDrew = true;
                }
                this->DrawGrid(y, x);
            }
        }

        lastX = x;
        lastY = y;
        // Update Frame
        Refresh();
    // For real-time feedback, continuously resample the canvas to the
    // ML input resolution and send the preview to the leaderboard/predictor.
    UpdateResampled();
    this->updateLeaderboard(this->resampledGrid);
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
                dc.DrawRectangle(x * CONSTANTS_H::CANSCALE, y * CONSTANTS_H::CANSCALE, CONSTANTS_H::CANSCALE, CONSTANTS_H::CANSCALE);
            }
        }
    }
}   

void Canvas::StoreState() {
        if (this->lastDrew) {
        // If the user modified the canvas since the last stored state,
        // push a copy into the undo history. Keep the history bounded to
        // the most recent 15 states to limit memory use.
        this->lastDrew = false;
        if (drawingHistory.size() >= 15) {
            drawingHistory.pop_back();
        }
        double* copy = new double[CONSTANTS_H::CANY * CONSTANTS_H::CANX];
        for (int i = 0; i < CONSTANTS_H::CANY * CONSTANTS_H::CANX; i++) {
            copy[i] = grid[i];
        }
        this->drawingHistory.push_front(copy);
    }   
}

// Bindable
void Canvas::ClearCanvas() {
    for (int i = 0; i < CONSTANTS_H::CANX * CONSTANTS_H::CANY; i++) {
        grid[i] = 0;
    }
    this->lastDrew = true;
    Refresh();
    StoreState();
    UpdateResampled();
    this->updateLeaderboard(this->resampledGrid);
}

void Canvas::RollBack() {
    // std::cout << "Rolled Back with history size " << this->drawingHistory.size() << std::endl;
    if (this->drawingHistory.size() > 1) {
        // Remove the current state from history
        double* currentState = this->drawingHistory.front();
        this->drawingHistory.pop_front();
        delete[] currentState;
        
        // Copy data from previous state
        double* previousState = this->drawingHistory.front();
        for (int i = 0; i < CONSTANTS_H::CANY * CONSTANTS_H::CANX; i++) {
            grid[i] = previousState[i];
        }
        
        Refresh();
        UpdateResampled();
        this->updateLeaderboard(this->resampledGrid);
    }
    
}

// Draw with brush sie
void Canvas::DrawGrid(const int& y, const int& x) {
    // Draw on position, and draw on points around the position
    for (int dy = -brushSize / 2; dy <= brushSize / 2; ++dy) {
        for (int dx = -brushSize / 2; dx <= brushSize / 2; ++dx) {
            int px = x + dx;
            int py = y + dy;
            if (MouseRange(py, px)) {
                if (grid[py * CONSTANTS_H::CANX + px] == 0) {
                    this->lastDrew = true;
                }
                grid[py * CONSTANTS_H::CANX + px] = 1;
            }
        }
    }
}

void Canvas::UpdateResampled() {
    // call CanvasConverter to resample current grid into resampledGrid
    if (this->resampledGrid) {
        // Direct call to perform resampling
        CanvasConverter::ResampleGrid(this->grid, CONSTANTS_H::CANX, CONSTANTS_H::CANY, this->resampledGrid, CONSTANTS_H::DESTX, CONSTANTS_H::DESTY);
    }
}

bool Canvas::is_empty() {
    for (int i = 0; i < CONSTANTS_H::CANY * CONSTANTS_H::CANX; i++) {
        if (grid[i]) {
            return false;
        }
    }
    return true;
}
// encapsulation
double* Canvas::get_grid() {
    return this->grid;
}

double* Canvas::get_resampled_grid() {
    return this->resampledGrid;
}
bool Canvas::get_drawing_state() {
    return this->drawing;
}

int Canvas::get_brushSize() {
    return this->brushSize;
}

void Canvas::set_brushSize(int newSize) {
    if (newSize <= 0) {
        return;
    }
    this->brushSize = newSize;
}

bool Canvas::get_lastDrew() {
    return this->lastDrew;
}