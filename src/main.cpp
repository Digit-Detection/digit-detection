#include <iostream>
#include <string>
#include <wx/wx.h>
#include "wxwidget/Frame.h"
#include "neural/Training/NetworkTest.h"
#include "neural/Activation_Functions/FunctionsTest.h"
#include "constants.h"
class MainApp : public wxApp {
    public:
        // On App run
        bool OnInit() override;

};

// On App run (main function)
bool MainApp::OnInit() {
    // Variables
    const int canYSize = CONSTANTS_H::CANY * CONSTANTS_H::CANSCALE;
    const int canXSize = CONSTANTS_H::CANX * CONSTANTS_H::CANSCALE;

    // Run any neural stuff here
    std::cout << "====================== RUNNING! ======================" << std::endl;
    // Old Tests
    // NetworkTest network_test;
    // network_test.runTests();
    // FunctionsTest func_test;
    // func_test.runTests();

    NetworkData networkData = NetworkData();
    NetworkData network_data = NetworkData();
    // NeuralNetwork* network = network_data.LoadNetworkFromSaved();
    NetworkSettings* settings = new NetworkSettings();

    // ARCHITECTURE
    const int num_layers = 4;
    int* layer_sizes = new int[num_layers];
    layer_sizes[0] = 28 * 28;   // input dimension
    layer_sizes[1] = 64;   // hidden layers
    layer_sizes[2] = 16;
    layer_sizes[3] = 10;   // output layer (two classes)

    // Configure Settings
    settings->set_layer_sizes(layer_sizes);
    settings->set_num_layers(num_layers);
    NetworkTrainer network_trainer(settings); // Create trainer

    // Train Network
    std::cout << "Neural : Loading Training Data" << std::endl;
    network_trainer.LoadData();
    std::cout << "Neural : Training Network" << std::endl;
    network_trainer.StartTrainingSession(1); // Amount trained. Change this when testing program.

    delete[] layer_sizes;
    delete settings;

    // === Create window ===
    std::cout << "GUI : Loading Frame" << std::endl;
    Frame* mainFrame = new Frame("Digit Detector");
    std::cout << "GUI : Frame Loaded" << std::endl;

    // Finish Frame Window
    mainFrame->Center();
    mainFrame->Show();
    std::cout << "Frame : Revealed" << std::endl;

    std::cout << "====================== Project Loaded and Running ======================" << std::endl;
    return true;
}

wxIMPLEMENT_APP(MainApp);
