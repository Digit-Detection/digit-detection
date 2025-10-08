#include <wx/wx.h>
#include <vector>
// g++ myDrawTest.cpp `wx-config --cxxflags --libs`

const int CANY = 50; // Canvas Y size
const int CANX = 50; // Canvas X size;
const int SCALE = 20; // Each cell is 20x20 pixels

class Canvas : public wxPanel{
    public:
        // Parent window, 
        Canvas(wxFrame* parent) : wxPanel(parent, wxID_ANY, wxDefaultPosition, wxSize(CANX * SCALE, CANY * SCALE)) {
            this->SetBackgroundColour(wxColour(wxColor(*wxGREEN))); // Set default color
            Bind(wxEVT_PAINT, &Canvas::OnPaint, this);
                Bind(wxEVT_LEFT_DOWN, &Canvas::OnMouseDown, this);
                Bind(wxEVT_LEFT_UP, &Canvas::OnMouseUp, this);
                Bind(wxEVT_MOTION, &Canvas::OnMouseMove, this);
                Bind(wxEVT_PAINT, &Canvas::OnPaint, this);

        }
        ~Canvas() {}

    private:
        void OnMouseMove(wxMouseEvent& evt) {
            if (drawing) {
                int x = evt.GetX() / SCALE;
                int y = evt.GetY() / SCALE;
                grid[y][x] = 1;
                Refresh(); // triggers OnPaint
            }
        }

        double grid[50][50] = {0}; // initialize drawing pixel size to 50
        void OnPaint(wxPaintEvent& evt) {
            wxPaintDC dc(this);
            dc.Clear();
            dc.SetPen(wxPen(*wxBLACK, 1));

            for (int y = 0; y < CANY; ++y) {
                for (int x = 0; x < CANX; ++x) {
                    if (grid[y][x]) {
                        dc.DrawRectangle(x * SCALE, y * SCALE, SCALE, SCALE);
                    }
                }
            }
        }
        void OnMouseDown(wxMouseEvent& evt) {
            drawing = true;
        }
        void OnMouseUp(wxMouseEvent& evt) {
            drawing = false;
        }

        bool drawing = false; // initally false
};

class Frame : public wxFrame {
    public:
        Frame(std::string windowName) : wxFrame(nullptr, wxID_ANY, windowName, wxDefaultPosition, wxSize(CANX * SCALE, CANY * SCALE), wxDEFAULT_FRAME_STYLE & ~(wxRESIZE_BORDER | wxMAXIMIZE_BOX)) {
            myCanvas = new Canvas(this);
            myCanvas->SetMinSize(wxSize(CANX * SCALE, CANY * SCALE));

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