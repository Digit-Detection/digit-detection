#include "wxwidget/Frame.h"
#include <wx/spinctrl.h> // Specifically defined for the spinCtrl obj
// wxFrame(parentWindow, windowID, windowName, de)
Frame::Frame(std::string windowName) : wxFrame(nullptr, wxID_ANY, windowName) {
    /*
    wxPoint(x, y) is a point on the object.
    wxSize(x, y) is the size of the object 
    if x||y == -1, it'll be the default

    */
    // wxPanel(parentWindow); {Kind of like a sub container}
    wxPanel* panel = new wxPanel(this);
    // wxButton(parentWindow, buttonID, textOnButton, position, size)
    wxButton* button = new wxButton(panel, wxID_ANY, "Submit", wxPoint(150, 150), wxSize(100, 35));

    // wxCheckBox(parentWindow, ID, Text Next to checkbox, position)
    wxCheckBox* checkBox = new wxCheckBox(panel, wxID_ANY, "check me", wxPoint(100, 100));

    // Static text
    wxStaticText* staticText = new wxStaticText(panel, wxID_ANY, "Hello World!!!!", wxPoint(520, 300));

    // Text Box
    wxTextCtrl* textCtrl = new wxTextCtrl(panel, wxID_ANY, "edit me!", wxPoint(350, 150), wxSize(200, -1));

    // Slider(parentwindow, objectID, initialValue, MinimumValue, MaximumValue, position, size)
    wxSlider* slider = new wxSlider(panel, wxID_ANY, 0, 0, 9, wxPoint(200, 250), wxSize(200, -1), wxSL_VALUE_LABEL);

    // wxGuage(parentWindow, objID, MaximumValue, Pos, Size) (progress bar) 
    wxGauge* gauge = new wxGauge(panel, wxID_ANY, 100, wxPoint(150, 500), wxSize(100, -1));
    gauge->SetValue(79);

    // wxChoices
    wxArrayString choices;
    choices.Add("1");
    choices.Add("2");
    choices.Add("3");

    // wxChoices(parentWindow, ID, pos, size, choicesArray OR wxString string arrays)
    wxChoice* choice = new wxChoice(panel, wxID_ANY, wxPoint(400, 500), wxSize(100, -1), choices); 
    // default selection item (by index)
    choice->Select(0);

    // Number-only input
    // wxSpinCtrl (parentWindow, ID, string inside text, pos, size, movement Option, minval, maxVal, Initial value)
    wxSpinCtrl* spinCtrl = new wxSpinCtrl(panel, wxID_ANY, "", wxPoint(450, 375), wxSize(-1, -1), wxSP_ARROW_KEYS, 1, 10, 1);
    
    // List Box
    wxListBox* listBox = new wxListBox(panel, wxID_ANY, wxPoint(550, 475), wxSize(-1, -1), choices);

    // Radio Box (..., size, choicesArray, numRows, Style)
    wxRadioBox* radioBox = new wxRadioBox(panel, wxID_ANY, "myRADIOBOX", wxPoint(650, 475), wxDefaultSize, choices, 2, wxRA_SPECIFY_ROWS);
    return;
}