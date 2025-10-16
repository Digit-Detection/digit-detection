#pragma once
#include <wx/wx.h>
#include <wx/scrolwin.h>
#include <wx/sizer.h>

class Leaderboard : public wxScrolledWindow {
public:
    Leaderboard(wxWindow* parent);
    ~Leaderboard();

    void UpdateLeaderboard(double* probability);

private:
    wxBoxSizer* sizer;
    double truncate_double(double value, int decimals);
    std::string textPrettify(int num, double prob);
};
