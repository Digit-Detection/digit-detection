#pragma once
#include <wx/wx.h>
#include <wx/spinctrl.h>
#include <functional>
class BrushPicker : public wxSpinCtrl {
public:
    BrushPicker(wxWindow* parent, int maxSize, std::function<void(int)> canvasFunction);
    ~BrushPicker();
    void LinkCanvas(std::function<void(int)> canvasFunction);
    void OnChange(wxCommandEvent& event);
private:
    std::function<void(int)> canvasLink;
};

