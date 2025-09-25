#include "DigitRowPanel.h"

DigitRowPanel::DigitRowPanel(wxWindow& parent) : wxPanel(parent) {
    wxBoxSizer* boxSizer = wxBoxSizer(wxHORIZONTAL);

}

DigitRowPanel::~DigitRowPanel() {
    for (int i = 0; i < 3; i++)
}