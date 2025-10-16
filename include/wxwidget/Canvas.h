#include <wx/wx.h>
#include <vector>
#include <wx/geometry.h>
#include "constants.h"
#include <deque>
#include <functional>
class Canvas : public wxPanel{
    public:
        // Parent window, 
        Canvas(wxPanel* parent, std::function<void(double*)> updateLeaderboardFunc);
        ~Canvas();

        // Bindable
        void ClearCanvas();
        void RollBack();
        // encapsulation
        double* get_grid();
        bool get_drawing_state();
        
    private:
        int lastX;
        int lastY; // Last mouse pos
        double* grid; // 2D array represented in 1D
        bool drawing; // Is holding left click
        bool lastDrew; // Check if any changes to canvas has been made on last drew
        std::deque<double*> drawingHistory; // Store copies
        std::function<void(double*)> updateLeaderboard;

        bool MouseRange(const int& y, const int& x);
        void OnMouseDown(wxMouseEvent& evt);
        void OnMouseUp(wxMouseEvent& evt);
        void OnMouseMove(wxMouseEvent& evt);
        void OnPaint(wxPaintEvent& evt);      
        void StoreState(); // Store canvas state for rollback
        

};
