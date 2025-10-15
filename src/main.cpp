#include <iostream>
#include <string>
#include <wx/wx.h>
#include "wxwidget/Frame.h"
#include "neural/Training/NetworkTest.h"
#include "constants.h"
class MainApp : public wxApp {
    public:
        // On App run
        bool OnInit() override;

};

// On App run (main function)
bool MainApp::OnInit() {
    // Variables
    const int canYSize = CONSTANTS_H::CANY * CONSTANTS_H::SCALE;
    const int canXSize = CONSTANTS_H::CANX * CONSTANTS_H::SCALE;

    // Run any neural stuff here
    std::cout << "RUNNING!" << std::endl;
    NetworkTest network_test;
    network_test.runTests();

    // === Create window ===
    Frame* mainFrame = new Frame("Digit Detector");
    // Window Size;
    mainFrame->SetClientSize(canXSize, canYSize);
    // Center the window
    mainFrame->Center();
    // Complet and Show the window
    mainFrame->Show();

    std::cout << "COMPLETE!" << std::endl;
    return true;
}

wxIMPLEMENT_APP(MainApp);
