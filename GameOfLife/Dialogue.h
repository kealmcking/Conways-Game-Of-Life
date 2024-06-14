#pragma once
#include "wx/wx.h"
#include "wx/spinctrl.h"
#include "wx/clrpicker.h"
#include "GameSettings.h"

class Dialogue :
    public wxDialog
{
public:
    Dialogue(wxWindow* parent);
    ~Dialogue();

    wxStaticText* spinnerLabelGrid;
    wxStaticText* spinnerLabelInterval;
    wxStaticText* colorPickerLabelAlive;
    wxStaticText* colorPickerLabelDead;
    wxSpinCtrl* _spinnerGrid;
    wxSpinCtrl* _spinnerInterval;
    wxColourPickerCtrl* _colorPickerAlive;
    wxColourPickerCtrl* _colorPickerDead;

    wxBoxSizer* mainBox;
    
    wxBoxSizer* spinnerSizerGrid;
    wxBoxSizer* spinnerSizerInterval;
    wxBoxSizer* colorPickerSizerAlive;
    wxBoxSizer* colorPickerSizerDead;
    wxSizer* buttonSizer;

    GameSettings* settings;

    void okButton(wxCommandEvent& event);
    void SetSettings(GameSettings* settings);
    void cancelButton(wxCommandEvent& event);


    wxDECLARE_EVENT_TABLE();
};
