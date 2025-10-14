#include <wx/wx.h>
#include <vector>
#include <wx/geometry.h>
#include "constants.h"
class Canvas : public wxPanel{
    public:
        // Parent window, 
        Canvas(wxFrame* parent);
        ~Canvas();

    private:
        int lastX;
        int lastY; // Last mouse pos
        double grid[CONSTANTS_H::CANY][CONSTANTS_H::CANX]; // initialize Pixel size
        bool drawing; // Is holding left click
        
        bool MouseRange(const int& y, const int& x);
        void OnMouseDown(wxMouseEvent& evt);
        void OnMouseUp(wxMouseEvent& evt);
        void OnMouseMove(wxMouseEvent& evt);
        void OnPaint(wxPaintEvent& evt);      

};
