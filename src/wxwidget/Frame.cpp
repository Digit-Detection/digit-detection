#include "wxwidget/Frame.h"
#include "wxwidget/Canvas.h"
#include "wxwidget/UIButton/ClearButton.h"
#include "wxwidget/UIButton/UndoButton.h"
#include "wxwidget/UIButton/NumPad.h"
#include "constants.h"
#include <wx/scrolwin.h>
#include <wx/sizer.h>
#include <iostream>

const int canvasHeight = CONSTANTS_H::CANY * CONSTANTS_H::SCALE;
const int canvasWidth = CONSTANTS_H::CANX * CONSTANTS_H::SCALE;
Frame::Frame(std::string windowName) : wxFrame(nullptr, wxID_ANY, windowName, 
        wxPoint(-1, -1), // Window Position (Default)
        wxSize(-1, -1), // Window Size (Default)
        wxDEFAULT_FRAME_STYLE & ~(wxRESIZE_BORDER | wxMAXIMIZE_BOX)) // Unresizeable
    {  
        // ============ Link Neural ================
        NetworkData neural;
        this->network = neural.LoadNetworkFromSaved();  
        std::cout << "Network Initialized" << std::endl; 

        // =========== Develop Containers ===========
        // Left Panel (Canvas)
        wxPanel* canvasPanel = new wxPanel(this, wxID_ANY);
        canvasPanel->SetBackgroundColour(wxColor(100, 100, 200));

        // Right Panel (Tools)
        wxPanel* toolPanel = new wxPanel(this, wxID_ANY);
        toolPanel->SetBackgroundColour(wxColor(100, 200, 100));
        
        // =========== Develop Canvas ===========
        Canvas* drawCanvas = new Canvas(canvasPanel, [this](double* grid) {
            this->UpdateLeaderboard(grid);
        });

        // =========== Develop UI Buttons ===========
        wxPanel* buttonPanel = new wxPanel(toolPanel, wxID_ANY);
        buttonPanel->SetBackgroundColour(wxColor(200, 200, 50));
        
        // -- Clear Button/Undo --
        wxPanel* clearBPanel = new wxPanel(buttonPanel, wxID_ANY);
        clearBPanel->SetBackgroundColour(wxColor(200, 50, 50));

        ClearButton* clearButton = new ClearButton(clearBPanel, [drawCanvas]() {
            drawCanvas->ClearCanvas(); // Pass Function pointer
        });

        UndoButton* undoButton = new UndoButton(clearBPanel, [drawCanvas]() {
            drawCanvas->RollBack();
        });
        
        wxBoxSizer* centerSizer = new wxBoxSizer(wxHORIZONTAL);
        centerSizer->Add(clearButton, 0, wxALIGN_CENTER | wxALL, 2);
        centerSizer->Add(undoButton, 0, wxALIGN_CENTER | wxALL, 2);
        clearBPanel->SetSizerAndFit(centerSizer);
        
        // -- Submission Buttons -- 
        wxPanel* numPanel = new wxPanel(buttonPanel, wxID_ANY);
        numPanel->SetBackgroundColour(wxColor(50, 50, 200));

        NumPad* submissionPad = new NumPad(numPanel, drawCanvas->get_grid());
        
        // =========== Develop Leaderboard ===========
        this->leaderboard = new Leaderboard(toolPanel);

        // =========== Apply Changes ===============
        // Horizontal (Main) Sizer
        wxBoxSizer* mainSizer = new wxBoxSizer(wxHORIZONTAL);
        mainSizer->Add(canvasPanel, 7, wxEXPAND | wxALL, 2); // 70%
        mainSizer->Add(toolPanel, 3, wxEXPAND | wxALL, 2); // 30%
        this->SetSizerAndFit(mainSizer);

        // Vertical (Tool) Sizer
        wxBoxSizer* toolSizer = new wxBoxSizer(wxVERTICAL);
        toolSizer->Add(buttonPanel, 4, wxEXPAND); // 40%
        toolSizer->Add(leaderboard, 6, wxEXPAND); // 60%
        toolPanel->SetSizerAndFit(toolSizer);

        // Vertical (Button) Sizer
        wxBoxSizer* buttonSizer = new wxBoxSizer(wxVERTICAL);
        buttonSizer->Add(clearBPanel, 3, wxEXPAND); // 30%
        buttonSizer->Add(numPanel, 7, wxEXPAND); // 70%
        buttonPanel->SetSizerAndFit(buttonSizer);

        // Update leaderboard
        this->UpdateLeaderboard(drawCanvas->get_grid());
    }

Frame::~Frame() {
    delete network;
}

void Frame::UpdateLeaderboard(double* grid) {
    std::pair<int, double*> res = network->Run(grid);
    this->leaderboard->UpdateLeaderboard(res.second);
    delete[] res.second;
}