#pragma once
#include <string>
#include <wx/wx.h>

// Create a window instance

class Frame : public wxFrame {
public:
    Frame(std::string windowName);
private:
    // Event handlers
    void OnButtonClicked(wxCommandEvent& evt);
    // Event table
    wxDECLARE_EVENT_TABLE();
};