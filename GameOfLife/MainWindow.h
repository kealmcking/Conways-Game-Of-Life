#pragma once
#include <wx/wx.h>
#include <wx/frame.h>
#include <wx/numdlg.h>

#include "GameSettings.h"

class DrawingPanel;

class MainWindow : public wxFrame
{
public:
	MainWindow(wxWindow* parent, wxWindowID id, const wxString& title, const wxPoint& pos = wxDefaultPosition, const wxSize& size = wxDefaultSize);
	~MainWindow();
	void OnClose(wxCloseEvent& event);

	void GridInitialization();
	void OnSizeChange(wxSizeEvent& event);
	void UpdateStatusBar(std::string _statusText);
	void PlayButtonPressed(wxCommandEvent& event);
	void PauseButtonPressed(wxCommandEvent& event);
	void NextButtonPressed(wxCommandEvent& event);
	void TrashButtonPressed(wxCommandEvent& event);
	void SettingsMenu(wxCommandEvent& event);
	void NewFile(wxCommandEvent& event);
	void OpenFile(wxCommandEvent& event);
	void SaveFile(wxCommandEvent& event);
	void SaveAsFile(wxCommandEvent& event);
	void Exit(wxCommandEvent& event);
	void ImportFile(wxCommandEvent& event);
	void ShowNeighborCount(wxCommandEvent& event);
	void Randomize(wxCommandEvent& event);
	void RandomizeWithSeed(wxCommandEvent& event);
	void FiniteUniverse(wxCommandEvent& event);
	void TorodialUniverse(wxCommandEvent& event);
	void RefreshMenuItems();

	void DisplayGrid(wxCommandEvent& event);

	void RandomizeGrid(int _seed);

	void Timer(wxTimerEvent& timer);

	int FindNeighborCount(int row, int column);

	void IncrementRound();
	void ClearBoard();

	GameSettings settings;
	DrawingPanel* panel = nullptr;
	wxMenuBar* _menuBar = nullptr;
	wxMenu* _menu = nullptr;
	wxMenu* _fileMenu = nullptr;
	wxMenu* _viewMenu = nullptr;
	wxMenu* _optionsMenu = nullptr;
	wxBoxSizer* _sizer = nullptr;
	wxStatusBar* _status = nullptr;
	wxToolBar* _toolBar = nullptr;
	wxTimer* _timer = nullptr;

	wxMenuItem* _finite;
	wxMenuItem* _torodial;


	std::vector<std::vector<bool>> gameBoard;
	std::vector<std::vector<int>> neighbors;

	std::string fileName;


	wxDECLARE_EVENT_TABLE();

	

private:

	int generation;
	int livingCells;

};

