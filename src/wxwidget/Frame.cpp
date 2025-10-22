#include "wxwidget/Frame.h"
#include "wxwidget/Canvas.h"
#include "wxwidget/UIButton/ClearButton.h"
#include "wxwidget/UIButton/UndoButton.h"
#include "wxwidget/UIButton/NumPad.h"
#include "wxwidget/BrushPicker.h"
#include "constants.h"
#include <wx/scrolwin.h>
#include <wx/sizer.h>
#include <iostream>

Frame::Frame(std::string windowName) : wxFrame(nullptr, wxID_ANY, windowName, 
        wxDefaultPosition,
        wxDefaultSize,
        wxDEFAULT_FRAME_STYLE & ~(wxRESIZE_BORDER | wxMAXIMIZE_BOX))
{  
    NetworkData neural;
    this->network = neural.LoadNetworkFromSaved();  
    std::cout << "Frame : Network Initialized" << std::endl; 
    
    // =========== Develop Containers ===========
    wxPanel* canvasPanel = new wxPanel(this, wxID_ANY);
    canvasPanel->SetBackgroundColour(wxColor(100, 100, 200));
    canvasPanel->SetMinSize(wxSize(CONSTANTS_H::CANX * CONSTANTS_H::CANSCALE, CONSTANTS_H::CANY * CONSTANTS_H::CANSCALE));
    
    wxPanel* toolPanel = new wxPanel(this, wxID_ANY);
    toolPanel->SetBackgroundColour(wxColor(100, 200, 100));
    toolPanel->SetMinSize(wxSize(CONSTANTS_H::TOOLSIZE, CONSTANTS_H::CANY * CONSTANTS_H::CANSCALE));
    std::cout << "Frame : Sizers Developed" << std::endl;
    // =========== Develop Canvas ===========
    Canvas* drawCanvas = new Canvas(canvasPanel, [this](double* grid) {
        this->UpdateLeaderboard(grid);
    });
    std::cout << "Frame : Canvas Developed" << std::endl;
    
    wxBoxSizer* canvasSizer = new wxBoxSizer(wxVERTICAL);
    canvasSizer->Add(drawCanvas, 0, wxEXPAND | wxALL, 0);
    canvasPanel->SetSizer(canvasSizer);
    
    // =========== Develop UI Buttons ===========
    wxPanel* buttonPanel = new wxPanel(toolPanel, wxID_ANY);
    buttonPanel->SetBackgroundColour(wxColor(200, 200, 50));
    
    // -- Clear Button/Undo --
    wxPanel* clearBPanel = new wxPanel(buttonPanel, wxID_ANY);
    clearBPanel->SetBackgroundColour(wxColor(200, 50, 50));
    
    ClearButton* clearButton = new ClearButton(clearBPanel, [drawCanvas]() {
        drawCanvas->ClearCanvas();
    });
    UndoButton* undoButton = new UndoButton(clearBPanel, [drawCanvas]() {
        drawCanvas->RollBack();
    });
    BrushPicker* brushPicker = new BrushPicker(clearBPanel, [drawCanvas](int newSize) {
        drawCanvas->set_brushSize(newSize);
    });
    std::cout << "Frame : Utility Panel Developed" << std::endl;
    
    wxBoxSizer* centerSizer = new wxBoxSizer(wxHORIZONTAL);
    centerSizer->Add(clearButton, 0, wxALIGN_CENTER | wxALL, 2);
    centerSizer->Add(undoButton, 0, wxALIGN_CENTER | wxALL, 2);
    centerSizer->Add(brushPicker, 0, wxALIGN_CENTER | wxALL, 2);
    clearBPanel->SetSizer(centerSizer);
    
    // -- Submission Buttons -- 
    wxPanel* numPanel = new wxPanel(buttonPanel, wxID_ANY);
    numPanel->SetBackgroundColour(wxColor(50, 50, 200));
    NumPad* submissionPad = new NumPad(numPanel, drawCanvas);
    
    wxBoxSizer* numSizer = new wxBoxSizer(wxVERTICAL);
    numSizer->Add(submissionPad, 1, wxEXPAND | wxALL, 0);
    numPanel->SetSizer(numSizer);
    
    // =========== Develop Leaderboard ===========
    this->leaderboard = new Leaderboard(toolPanel);
    this->UpdateLeaderboard(drawCanvas->get_grid());
    std::cout << "Frame : Leaderboard panel Developed" << std::endl;

    // =========== Apply Sizers ===========
    
    // Button Panel: Clear/Undo/Brush (20%) + NumPad (80%)
    wxBoxSizer* buttonSizer = new wxBoxSizer(wxVERTICAL);
    buttonSizer->Add(clearBPanel, 2, wxEXPAND | wxALL, 0);
    buttonSizer->Add(numPanel, 8, wxEXPAND | wxALL, 0);
    buttonPanel->SetSizer(buttonSizer);
    
    // Tool Panel: Buttons (40%) + Leaderboard (60%)
    wxBoxSizer* toolSizer = new wxBoxSizer(wxVERTICAL);
    toolSizer->Add(buttonPanel, 4, wxEXPAND | wxALL, 0);
    toolSizer->Add(leaderboard, 6, wxEXPAND | wxALL, 0);
    toolPanel->SetSizer(toolSizer);
    std::cout << "Frame : features linked to Sizers" << std::endl;

    // Main Sizer: Canvas + Tools
    wxBoxSizer* mainSizer = new wxBoxSizer(wxHORIZONTAL);
    mainSizer->Add(canvasPanel, 1, wxEXPAND | wxALL, 0);
    mainSizer->Add(toolPanel, 0, wxEXPAND | wxALL, 0);
    this->SetSizer(mainSizer);

    std::cout << "Frame : Sizers linked to frame" << std::endl;

    // Set explicit client size to prevent compression
    int totalWidth = CONSTANTS_H::CANX * CONSTANTS_H::CANSCALE + CONSTANTS_H::TOOLSIZE;
    int totalHeight = CONSTANTS_H::CANY * CONSTANTS_H::CANSCALE;
    this->SetClientSize(totalWidth, totalHeight);
    
    this->Layout();
    this->Refresh();
    std::cout << "Frame : Updated and Ready" << std::endl;

    // DEBUGGING CODE
    // std::cout << "Tool Panel Children: " << toolPanel->GetChildren().GetCount() << std::endl;
    // std::cout << "Button Panel Children: " << buttonPanel->GetChildren().GetCount() << std::endl;
    // std::cout << "Leaderboard Children: " << leaderboard->GetChildren().GetCount() << std::endl;
    // wxSize toolSize = toolPanel->GetSize();
    // wxSize canvasSize = canvasPanel->GetSize();
    // std::cout << "Tool Panel Size: " << toolSize.x << " x " << toolSize.y << std::endl;
    // std::cout << "Canvas Panel Size: " << canvasSize.x << " x " << canvasSize.y << std::endl;
}

Frame::~Frame() {
    delete network;
}

void Frame::UpdateLeaderboard(double* grid) {
    std::pair<int, double*> res = network->Run(grid);
    this->leaderboard->UpdateLeaderboard(res.second);
    delete[] res.second;
}