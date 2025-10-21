#include "wxwidget/Leaderboard.h"
#include "constants.h"
#include <algorithm>

Leaderboard::Leaderboard(wxWindow* parent)
    : wxScrolledWindow(parent, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxVSCROLL)
    {   
        this->SetMinSize(wxSize(CONSTANTS_H::TOOLSIZE, CONSTANTS_H::CANY * CONSTANTS_H::CANSCALE));    
        // ================ Scrollable window modification ================
        this->SetScrollRate(10, 10);
        this->sizer = new wxBoxSizer(wxVERTICAL);
        this->SetSizer(sizer);

    }
Leaderboard::~Leaderboard() {}

double Leaderboard::truncate_double(double value, int decimals) {
    double factor = std::pow(10.0, decimals);
    return std::trunc(value * factor) / factor;
}
std::string Leaderboard::textPrettify(int num, double prob) {
    return std::to_string(num) + " : %" + std::to_string(truncate_double(prob, 3) * 100);
}

void Leaderboard::UpdateLeaderboard(double* probability) {
    // std::cout << "Updating Leaderboard" << std::endl;
    // Clear previous content
    this->sizer->Clear(true); // true = delete windows
    
    // =========== Sort Leaderboard =============
    std::vector<std::pair<int, double>> payload(CONSTANTS_H::NUMDIGITS); // {<value, probability>}
    for (int i = 0; i < CONSTANTS_H::NUMDIGITS; i++) {
        payload[i] = {i, probability[i]};
    }
    sort(payload.begin(), payload.end(), [&](const std::pair<int, double>& a, const std::pair<int, double>& b) {
        if (this->truncate_double(a.second, 6) == this->truncate_double(b.second, 6)) return a.first < b.first; // decreasing by value
        return a.second > b.second; // decreasing by probability
    });
    
    // ================ Insert Content ====================
    for (int i = 0; i < CONSTANTS_H::NUMDIGITS; i++) {
        wxStaticText* label = new wxStaticText(this, wxID_ANY, textPrettify(payload[i].first, payload[i].second));
        this->sizer->Add(label, 0, wxALL, 5);
    }
    
    // Refresh layout
    FitInside();
    Layout();
}