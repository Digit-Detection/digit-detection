#include "wxwidget/Frame.h"
#include <wx/spinctrl.h> // Specifically defined for the spinCtrl obj
// wxFrame(parentWindow, windowID, windowName, de)

/*
Positive-only IDs
Can't be ID 0 or 1
Cannot be between 4999 to 5999
*/
enum IDs {
    SUBMIT_BUTTON_ID = 2
};

// Event table for {Class, Base Class}
wxBEGIN_EVENT_TABLE(Frame, wxFrame)
    EVT_BUTTON(SUBMIT_BUTTON_ID, Frame::OnButtonClicked)
wxEND_EVENT_TABLE()

Frame::Frame(std::string windowName) : wxFrame(nullptr, wxID_ANY, windowName) {
    /*
    wxPoint(x, y) is a point on the object.
    wxSize(x, y) is the size of the object 
    if x||y == -1, it'll be the default

    */
    // wxPanel(parentWindow); {Kind of like a sub container}
    wxPanel* panel = new wxPanel(this);
    // wxButton(parentWindow, buttonID, textOnButton, position, size)
    wxButton* button = new wxButton(panel, SUBMIT_BUTTON_ID, "Submit", wxPoint(150, 150), wxSize(100, 35));
    CreateStatusBar();
    return;
}

void Frame::OnButtonClicked(wxCommandEvent& evt) {
    wxLogStatus("Button Clicked!");
}