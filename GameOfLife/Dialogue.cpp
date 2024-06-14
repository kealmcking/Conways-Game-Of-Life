#include "Dialogue.h"


wxBEGIN_EVENT_TABLE(Dialogue, wxDialog)
	EVT_BUTTON(wxID_OK, Dialogue::okButton)
	EVT_BUTTON(wxID_CANCEL, Dialogue::cancelButton)
wxEND_EVENT_TABLE()

Dialogue::Dialogue(wxWindow* parent)
	:wxDialog(parent, wxID_ANY, "Settings", wxPoint(100, 100), wxSize(200,200))
{
	settings = nullptr;
	mainBox = new wxBoxSizer(wxVERTICAL);
	SetSizer(mainBox);
	buttonSizer = CreateButtonSizer(wxOK | wxCANCEL);
	spinnerSizerGrid = new wxBoxSizer(wxHORIZONTAL);
	spinnerSizerInterval = new wxBoxSizer(wxHORIZONTAL);
	colorPickerSizerAlive = new wxBoxSizer(wxHORIZONTAL);
	colorPickerSizerDead = new wxBoxSizer(wxHORIZONTAL);

	_spinnerGrid = new wxSpinCtrl(this, wxID_ANY);
	_spinnerInterval = new wxSpinCtrl(this, wxID_ANY);
	_colorPickerAlive = new wxColourPickerCtrl(this, wxID_ANY);
	_colorPickerDead = new wxColourPickerCtrl(this, wxID_ANY);

	spinnerLabelGrid = new wxStaticText(this, wxID_ANY, "Grid Size");
	spinnerLabelInterval = new wxStaticText(this, wxID_ANY, "Interval");
	colorPickerLabelAlive = new wxStaticText(this, wxID_ANY, "Living Cell Color");
	colorPickerLabelDead = new wxStaticText(this, wxID_ANY, "Dead Cell Color");

	spinnerSizerGrid->Add(spinnerLabelGrid);
	spinnerSizerGrid->Add(_spinnerGrid);

	spinnerSizerInterval->Add(spinnerLabelInterval);
	spinnerSizerInterval->Add(_spinnerInterval);

	colorPickerSizerAlive->Add(colorPickerLabelAlive);
	colorPickerSizerAlive->Add(_colorPickerAlive);

	colorPickerSizerDead->Add(colorPickerLabelDead);
	colorPickerSizerDead->Add(_colorPickerDead);

	mainBox->Add(spinnerSizerGrid);
	mainBox->Add(spinnerSizerInterval);
	mainBox->Add(colorPickerSizerAlive);
	mainBox->Add(colorPickerSizerDead);

	mainBox->Add(buttonSizer);

}

Dialogue::~Dialogue() {

}

void Dialogue::okButton(wxCommandEvent& event) {
	settings->SetGridSize(_spinnerGrid->GetValue());
	settings->SetInterval(_spinnerInterval->GetValue());
	settings->SetColor(_colorPickerAlive->GetColour(), ColorType::Living);
	settings->SetColor(_colorPickerDead->GetColour(), ColorType::Dead);
	EndModal(wxID_OK);
}

void Dialogue::cancelButton(wxCommandEvent& event) {
	EndModal(wxID_CANCEL);
}

void Dialogue::SetSettings(GameSettings* _settings) {
	settings = _settings;
}