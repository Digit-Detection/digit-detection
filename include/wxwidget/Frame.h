#pragma once
#include <string>
#include "wxwidget/Leaderboard.h"
#include "neural/Data_Handling/NetworkData.h"
#include <functional>
#include <wx/wx.h>

// Create a window instance

class Frame : public wxFrame {
public:
    Frame(std::string windowName); 
    ~Frame();  
    void UpdateLeaderboard(double* grid);
private:
    NeuralNetwork* network;
    Leaderboard* leaderboard;
};