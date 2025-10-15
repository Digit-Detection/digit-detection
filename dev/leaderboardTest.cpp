#include <wx/wx.h>
#include "constants.h"
#include <iostream>
#include <string>
#include <vector>
#include <algorithm>
#include <cmath>
#include <cstdlib> // For rand() and srand()
#include <ctime>   // For time()

double truncate_double(double value, int decimals) {
    double factor = std::pow(10.0, decimals);
    return std::trunc(value * factor) / factor;
}

class Leaderboard : public wxScrolledWindow {
    public:
        Leaderboard(wxWindow* parent, int* numValues, double* probability)
            : wxScrolledWindow(parent, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxVSCROLL)
        {
            // =========== Sort Leaderboard =============
            std::vector<std::pair<int, double>> payload(CONSTANTS_H::NUMDIGITS); // {<value, probability>}
            for (int i = 0; i < CONSTANTS_H::NUMDIGITS; i++) {
                payload[i] = {numValues[i], probability[i]};
            }
            sort(payload.begin(), payload.end(), [](const std::pair<int, double>& a, const std::pair<int, double>& b) {
                if (a.second == b.second) return a.first > b.first; // decreasing by value
                return a.second > b.second; // decreasing by probability
            });
            // ================ Scrollable window modification ================
            this->SetScrollRate(0, 10); // scrolling rate (vertical only)

            // ================ Insert Content ====================
            wxBoxSizer* sizer = new wxBoxSizer(wxVERTICAL);
            for (int i = 0; i < CONSTANTS_H::NUMDIGITS; i++) {
                wxStaticText* label = new wxStaticText(this, wxID_ANY, textPrettify(payload[i].first, payload[i].second));
                sizer->Add(label, 0, wxALL, 5);
            }

            SetSizer(sizer);
            FitInside();
            Layout();

        }
        ~Leaderboard() {}
    private:
        std::string textPrettify(int num, double prob) {
            return std::to_string(num) + " : %" + std::to_string(truncate_double(prob, 3) * 100);
        }
};

int* customInts(int size) {
    int* ret = new int[size];
    for (int i = 0; i < size; i++) {
        int rangedNumber = (rand() % 100); // 0 to 99
        ret[i] = rangedNumber; 
    }
    return ret;
}

double* customDouble(int size) {
    double* ret = new double[size];
    for (int i = 0; i < size; i++) {
        double rangedNumber = (double) (rand() % 100000) / (rand() % 1000) ; // 0 to 99
        ret[i] = rangedNumber; 
    }
    return ret;
}
class Frame : public wxFrame {
    public:
        // Create unresizeable frame
        Frame(std::string windowName) : wxFrame(nullptr, wxID_ANY, windowName, wxDefaultPosition, wxSize(CANX * CONSTANTS_H::SCALE, CANY * CONSTANTS_H::SCALE), wxDEFAULT_FRAME_STYLE & ~(wxRESIZE_BORDER | wxMAXIMIZE_BOX)) {
            this->custInt = customInts(CONSTANTS_H::NUMDIGITS);
            this->custDbl = customDouble(CONSTANTS_H::NUMDIGITS);
            myLeaderboard = new Leaderboard(this, custInt, custDbl);
            myLeaderboard->SetMinSize(wxSize(-1, 150));

            wxBoxSizer* sizer = new wxBoxSizer(wxVERTICAL);
            sizer->Add(myLeaderboard, 0, wxALIGN_CENTER);
            SetSizerAndFit(sizer);
        }

        ~Frame() {
            delete[] this->custInt;
            delete[] this->custDbl;
        } 
    private:
        int* custInt;
        double* custDbl;
        Leaderboard* myLeaderboard;
};

class MainApp : public wxApp {
    public:
        bool OnInit() override {
            srand(time(0)); // seed randomness
            this->myFrame = new Frame("Leaderboard Test");
            this->myFrame->Show();
            return true;
        }
        ~MainApp() {}
    private: 
        Frame* myFrame;
};

wxIMPLEMENT_APP(MainApp);