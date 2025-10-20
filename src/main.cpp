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
    // NetworkTest network_test;
    // network_test.runTests();
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
    std::cout << "CP1" << std::endl;
    network_trainer.LoadData();
    std::cout << "CP2" << std::endl;
    network_trainer.StartTrainingSession(0); // AMOUNT TRAINED

    delete[] layer_sizes;
    delete settings;

    // === Create window ===
    std::cout << "completed 0" << std::endl;

    Frame* mainFrame = new Frame("Digit Detector");
    // Window Size;
    std::cout << "completed 1" << std::endl;
    // Center the window
    mainFrame->Center();
    std::cout << "completed 2" << std::endl;

    // Complet and Show the window
    mainFrame->Show();
    std::cout << "completed 3" << std::endl;

    std::cout << "COMPLETE!" << std::endl;
    return true;
}

wxIMPLEMENT_APP(MainApp);
