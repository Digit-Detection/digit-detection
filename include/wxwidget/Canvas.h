#include <wx/wx.h>
#include <vector>
#include <wx/geometry.h>
#include "constants.h"
class Canvas : public wxPanel{
    public:
        // Parent window, 
        Canvas(wxPanel* parent);
        ~Canvas();

        // Bindable
        void ClearCanvas();

        // encapsulation
        double* get_grid();
        bool get_drawing_state();
    private:
        int lastX;
        int lastY; // Last mouse pos
        double* grid; // 2D array represented in 1D
        bool drawing; // Is holding left click
        
        bool MouseRange(const int& y, const int& x);
        void OnMouseDown(wxMouseEvent& evt);
        void OnMouseUp(wxMouseEvent& evt);
        void OnMouseMove(wxMouseEvent& evt);
        void OnPaint(wxPaintEvent& evt);      

        

};
