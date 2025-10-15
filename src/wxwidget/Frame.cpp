#include "wxwidget/Frame.h"
#include "wxwidget/Canvas.h"
#include "wxwidget/UIButton/ClearButton.h"
#include "wxwidget/UIButton/NumPad.h"
#include "constants.h"

const int canvasHeight = CONSTANTS_H::CANY * CONSTANTS_H::SCALE;
const int canvasWidth = CONSTANTS_H::CANX * CONSTANTS_H::SCALE;
Frame::Frame(std::string windowName) : wxFrame(nullptr, wxID_ANY, windowName, 
        wxPoint(-1, -1), // Window Position (Default)
        wxSize(-1, -1), // Window Size (Default)
        wxDEFAULT_FRAME_STYLE & ~(wxRESIZE_BORDER | wxMAXIMIZE_BOX)) // Unresizeable
    {  
        // =========== Develop Containers ===========
        // Left Panel (Canvas)
        wxPanel* canvasPanel = new wxPanel(this, wxID_ANY);
        canvasPanel->SetBackgroundColour(wxColor(100, 100, 200));

        // Right Panel (Tools)
        wxPanel* toolPanel = new wxPanel(this, wxID_ANY);
        toolPanel->SetBackgroundColour(wxColor(100, 200, 100));
        
        // =========== Develop Canvas ===========
        Canvas* drawCanvas = new Canvas(canvasPanel);
        
        // =========== Develop UI Buttons ===========
        wxPanel* buttonPanel = new wxPanel(toolPanel, wxID_ANY);
        buttonPanel->SetBackgroundColour(wxColor(200, 200, 50));
        
        // -- Clear Button --
        wxPanel* clearBPanel = new wxPanel(buttonPanel, wxID_ANY);
        clearBPanel->SetBackgroundColour(wxColor(200, 50, 50));

        ClearButton* clearButton = new ClearButton(clearBPanel, [drawCanvas]() {
            drawCanvas->ClearCanvas(); // Pass Function pointer
        });
        wxBoxSizer* centerSizer = new wxBoxSizer(wxVERTICAL);
        centerSizer->Add(clearButton, 0, wxALIGN_CENTER | wxALL, 1);
        clearBPanel->SetSizerAndFit(centerSizer);
        
        // -- Submission Buttons -- 
        wxPanel* numPanel = new wxPanel(buttonPanel, wxID_ANY);
        numPanel->SetBackgroundColour(wxColor(50, 50, 200));

        NumPad* submissionPad = new NumPad(numPanel, drawCanvas->get_grid());
        
        // =========== Develop Leaderboard ===========
        wxPanel* leaderPanel = new wxPanel(toolPanel, wxID_ANY);
        leaderPanel->SetBackgroundColour(wxColor(0, 150, 0));
        
        // =========== Apply Changes ===============
        // Horizontal (Main) Sizer
        wxBoxSizer* mainSizer = new wxBoxSizer(wxHORIZONTAL);
        mainSizer->Add(canvasPanel, 7, wxEXPAND | wxALL, 2); // 70%
        mainSizer->Add(toolPanel, 3, wxEXPAND | wxALL, 2); // 30%
        this->SetSizerAndFit(mainSizer);

        // Vertical (Tool) Sizer
        wxBoxSizer* toolSizer = new wxBoxSizer(wxVERTICAL);
        toolSizer->Add(buttonPanel, 4, wxEXPAND); // 40%
        toolSizer->Add(leaderPanel, 6, wxEXPAND); // 60%
        toolPanel->SetSizerAndFit(toolSizer);

        // Vertical (Button) Sizer
        wxBoxSizer* buttonSizer = new wxBoxSizer(wxVERTICAL);
        buttonSizer->Add(clearBPanel, 3, wxEXPAND); // 30%
        buttonSizer->Add(numPanel, 7, wxEXPAND); // 70%
        buttonPanel->SetSizerAndFit(buttonSizer);
    }

Frame::~Frame() {}
    