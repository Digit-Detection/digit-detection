#include "wxwidget/Frame.h"
#include "constants.h"

Frame::Frame(std::string windowName) : wxFrame(nullptr, wxID_ANY, windowName, 
        wxDefaultPosition, // Window Position
        wxSize(1000 * CONSTANTS_H::SCALE, 1000 * CONSTANTS_H::SCALE), // Window Size
        wxDEFAULT_FRAME_STYLE & ~(wxRESIZE_BORDER | wxMAXIMIZE_BOX)) // Unresizeable
    {  
        // Develop Canvas

        // Develop UI Buttons

        // Develop Leaderboard
    }

Frame::~Frame() {}
    