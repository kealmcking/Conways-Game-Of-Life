#include "MainWindow.h"
#include "DrawingPanel.h"
#include "Dialogue.h"
#include "play.xpm"
#include "pause.xpm"
#include "next.xpm"
#include "trash.xpm"
#include <wx/filedlg.h>


wxBEGIN_EVENT_TABLE(MainWindow, wxWindow)
	EVT_CLOSE(MainWindow::OnClose)
	EVT_SIZE(MainWindow::OnSizeChange)
	EVT_MENU(10000, MainWindow::PlayButtonPressed)
	EVT_MENU(10001, MainWindow::PauseButtonPressed)
	EVT_MENU(10002, MainWindow::NextButtonPressed)
	EVT_MENU(10003, MainWindow::TrashButtonPressed)
	EVT_TIMER(10004, MainWindow::Timer)
	EVT_MENU(10005, MainWindow::SettingsMenu)
	EVT_MENU(10006, MainWindow::ShowNeighborCount)
	EVT_MENU(10007, MainWindow::Randomize)
	EVT_MENU(10008, MainWindow::RandomizeWithSeed)
	EVT_MENU(10009, MainWindow::NewFile)
	EVT_MENU(10010, MainWindow::OpenFile)
	EVT_MENU(10011, MainWindow::SaveFile)
	EVT_MENU(10012, MainWindow::SaveAsFile)
	EVT_MENU(10013, MainWindow::Exit)
	EVT_MENU(10014, MainWindow::FiniteUniverse)
	EVT_MENU(10015, MainWindow::TorodialUniverse)
	EVT_MENU(10016, MainWindow::ImportFile)
	EVT_MENU(10017, MainWindow::DisplayGrid)
wxEND_EVENT_TABLE()

MainWindow::MainWindow(wxWindow* parent, wxWindowID id, const wxString& title, const wxPoint& pos, const wxSize& size)
	: wxFrame(parent, id, title, pos, size) {
	settings.SaveData();
	_timer = new wxTimer(this, 10004);
	GridInitialization();
	panel = new DrawingPanel(this, gameBoard, neighbors);
	panel->SetSettings(&settings);
	_status = CreateStatusBar();
	_toolBar = CreateToolBar();
	UpdateStatusBar("status");

	wxBitmap playIcon(play_xpm);
	wxBitmap pauseIcon(pause_xpm);
	wxBitmap nextIcon(next_xpm);
	wxBitmap trashIcon(trash_xpm);

	_toolBar->AddTool(10000, "Play", playIcon);
	_toolBar->AddTool(10001, "Pause", pauseIcon);
	_toolBar->AddTool(10002, "Next", nextIcon);
	_toolBar->AddTool(10003, "Trash", trashIcon);

	_sizer = new wxBoxSizer(wxVERTICAL);
	_sizer->Add(panel, 1, wxEXPAND | wxALL);
	this->SetSizer(_sizer);
	this->Layout();

	_toolBar->Realize();

	_menuBar = new wxMenuBar();
	SetMenuBar(_menuBar);

	_menu = new wxMenu();
	_fileMenu = new wxMenu();
	_viewMenu = new wxMenu();
	_optionsMenu = new wxMenu();

	wxMenuItem* _new = new wxMenuItem(_fileMenu, 10009, "New");
	wxMenuItem* _open = new wxMenuItem(_fileMenu, 10010, "Open");
	wxMenuItem* _save = new wxMenuItem(_fileMenu, 10011, "Save");
	wxMenuItem* _saveAs = new wxMenuItem(_fileMenu, 10012, "Save As");
	wxMenuItem* _exit = new wxMenuItem(_fileMenu, 10013, "Exit");
	wxMenuItem* _import = new wxMenuItem(_fileMenu, 10016, "Import");

	wxMenuItem* _displayGrid = new wxMenuItem(_viewMenu, 10017, "Display Grid", "", wxITEM_CHECK);

	_finite = new wxMenuItem(_viewMenu, 10014, "Finite Universe", "" , wxITEM_CHECK);
	_torodial = new wxMenuItem(_viewMenu, 10015, "Torodial Universe", "", wxITEM_CHECK);
	_finite->SetCheckable(true);
	_torodial->SetCheckable(true);

	wxMenuItem* _showNeighborCount = new wxMenuItem(_viewMenu, 10006, "Show Neighbor Count", "", wxITEM_CHECK);
	wxMenuItem* _randomize = new wxMenuItem(_optionsMenu, 10007, "Randomize");
	wxMenuItem* _randomWithSeed = new wxMenuItem(_optionsMenu, 10008, "Randomize with Seed!");
	_showNeighborCount->SetCheckable(true);

	_menu->Append(10005, "Board Settings");
	_viewMenu->Append(_showNeighborCount);
	_viewMenu->Append(_finite);
	_viewMenu->Append(_torodial);
	_viewMenu->Append(_displayGrid);
	_optionsMenu->Append(_randomize);
	_optionsMenu->Append(_randomWithSeed);
	_fileMenu->Append(_new);
	_fileMenu->Append(_open);
	_fileMenu->Append(_import);
	_fileMenu->Append(_save);
	_fileMenu->Append(_saveAs);
	_fileMenu->Append(_exit);
	_menuBar->Append(_fileMenu, "File");
	_menuBar->Append(_menu, "Settings");
	_menuBar->Append(_viewMenu, "View");
	_menuBar->Append(_optionsMenu, "Options");
}

MainWindow::~MainWindow() {

}

void MainWindow::OnSizeChange(wxSizeEvent& event) {
	wxSize _size = event.GetSize();
	if (panel == nullptr) return;

	panel->SetSize(_size);
	event.Skip();

}

void MainWindow::GridInitialization() {
	int size = settings.gridSize;
	gameBoard.resize(size);

	for (int i = 0; i < gameBoard.size(); i++) {
		gameBoard[i].resize(size);
	}
}

int MainWindow::FindNeighborCount(int row, int column) {


	int neighborCount = 0;
	if (settings.isFinite) {
		for (int i = -1; i < 2; i++) {
			for (int j = -1; j < 2; j++) {
				if (row + i >= 0 && row + i < gameBoard.size() &&
					column + j >= 0 && column + j < gameBoard[row + i].size()) {
					if (!(i == 0 && j ==0) && gameBoard[row + i][column + j]) {
						neighborCount += 1;
					}
				}
			}
		}
	}
	//else {
	//	for (int i = -1; i < 2; i++) {
	//		for (int j = -1; j < 2; j++) {
	//			if (row + i >= -1 && row + i < gameBoard.size() &&
	//				column + j >= -1 && column + j < gameBoard[row + i].size()) {
	//				if (i == -1 && gameBoard[settings.gridSize - 1][j]) {
	//					neighborCount += 1;
	//				}
	//				if (j == -1 && gameBoard[i][settings.gridSize - 1]) {
	//					neighborCount += 1;
	//				}
	//				if (!(i == 0 && j == 0) && gameBoard[row + i][column + j]) {
	//					neighborCount += 1;
	//				}
	//			}
	//		}
	//	}
	//}

	return neighborCount;
}

void MainWindow::IncrementRound() {
	livingCells = 0;
	std::vector<std::vector<bool>> sandbox = gameBoard;

	sandbox.resize(gameBoard.size());
	neighbors.resize(gameBoard.size());

	for (int i = 0; i < gameBoard.size(); i++) {
		sandbox[i].resize(gameBoard.size());
		neighbors[i].resize(gameBoard.size());
	}

	for (int i = 0; i < sandbox.size(); i++) {
		for (int j = 0; j < sandbox.size(); j++) {
			int neighborCount = FindNeighborCount(i, j);
			if (sandbox[i][j]) {
				if (neighborCount < 2 || neighborCount > 3) {
					sandbox[i][j] = false;
					livingCells -= 1;
				}
				else {
					sandbox[i][j] = true;
					livingCells += 1;
				}
			}
			else {
				if (neighborCount == 3) {
					sandbox[i][j] = true;
					livingCells += 1;
				}
			}
			neighbors[i][j] = neighborCount;
		}
	}

	swap(sandbox, gameBoard);
	generation++;
	UpdateStatusBar("Generation: " + std::to_string(generation) + "  Living Cells: " + std::to_string(livingCells));
	panel->Refresh();
}

void MainWindow::ClearBoard() {
	for (int i = 0; i < gameBoard.size(); i++) {
		for (int j = 0; j < gameBoard.size(); j++) {
			gameBoard[i][j] = false;
		}
	}

	generation = 0;
	livingCells = 0;

	UpdateStatusBar("Generation: " + std::to_string(generation) + "  Living Cells: " + std::to_string(livingCells));

	panel->Refresh();
}

void MainWindow::RandomizeGrid(int _seed) {
	srand(_seed);

	for (int i = 0; i < gameBoard.size(); i++) {
		for (int j = 0; j < gameBoard.size(); j++) {
			if (rand() % 2 == 0) {
				gameBoard[i][j] = true;
			}
			else {
				gameBoard[i][j] = false;
			}
		}
	}
}

void MainWindow::UpdateStatusBar(std::string _statusText) {
	_status->SetStatusText(_statusText);

}

void MainWindow::PlayButtonPressed(wxCommandEvent& event) {
	_timer->Start(settings.interval);
	event.Skip();
}

void MainWindow::PauseButtonPressed(wxCommandEvent& event) {
	_timer->Stop();
	event.Skip();
}

void MainWindow::NextButtonPressed(wxCommandEvent& event) {
	IncrementRound();
	event.Skip();
}

void MainWindow::TrashButtonPressed(wxCommandEvent& event) {
	ClearBoard();
	event.Skip();
}

void MainWindow::Timer(wxTimerEvent& event) {
	IncrementRound();
	event.Skip();
}

void MainWindow::SettingsMenu(wxCommandEvent& event) {
	Dialogue* dialogueMenu = new Dialogue(this);
	dialogueMenu->SetSettings(&settings);

	if (dialogueMenu->ShowModal() == wxID_OK) {
		settings.SaveData();
		GridInitialization();
	}
	else if (dialogueMenu->ShowModal() == wxID_CANCEL) {
		settings.LoadData();
	}

	

	delete dialogueMenu;
	event.Skip();
}

void MainWindow::ShowNeighborCount(wxCommandEvent& event) {
	settings.showNeighborCount = event.IsChecked();
	event.Skip();
}

void MainWindow::Randomize(wxCommandEvent& event) {
	RandomizeGrid(time(NULL));
	event.Skip();
}

void MainWindow::RandomizeWithSeed(wxCommandEvent& event) {
	int seed = wxGetNumberFromUser("Enter Seed", "Seed: ", "Random with Seed");
	RandomizeGrid(seed);

	event.Skip();
}

void MainWindow::NewFile(wxCommandEvent& event) {
	ClearBoard();
	event.Skip();
}

void MainWindow::OpenFile(wxCommandEvent& event) {
	wxFileDialog onOpenDialog(this, _("Open Universe File"), "", "", "Cells files (*.cells) | .cells", wxFD_OPEN | wxFD_FILE_MUST_EXIST);

	if (onOpenDialog.ShowModal() == wxID_CANCEL)
		return;

	for (int i = 0; i < gameBoard.size(); i++) {
		gameBoard[i].clear();
		gameBoard.resize(0);
	}

	gameBoard.clear();
	gameBoard.resize(0);

	std::string buffer;
	std::ifstream fileStream;
	int index = 0;

	fileName = (std::string)onOpenDialog.GetPath();
	fileStream.open((fileName));

	if (fileStream.is_open()) {
		while (!fileStream.eof()) {
			std::getline(fileStream, buffer);
			if (buffer.size() == 0) { break; }
			if (gameBoard.size() == 0) {
				gameBoard.resize(buffer.size());
			}
			gameBoard[index].resize(buffer.size());
			for (int i = 0; i < buffer.size(); i++) {
				if (buffer[i] == '*') {
					gameBoard[index][i] = true;
				}
				else {
					gameBoard[index][i] = false;
				}
			}
			index++;
		}

		fileStream.close();
	}

	event.Skip();

}

void MainWindow::SaveFile(wxCommandEvent& event) {
	std::ofstream fileStream;
	fileStream.open(fileName);

	if (fileStream.is_open()) {
		for (int i = 0; i < gameBoard.size(); i++) {
			for (int j = 0; j < gameBoard[i].size(); j++) {
				if (gameBoard[i][j]) {
					fileStream << "*";
				}
				else {
					fileStream << ".";
				}

			}

			fileStream << "\n";
		}
		fileStream.close();
	}
	event.Skip();
}

void MainWindow::SaveAsFile(wxCommandEvent& event) {
	wxFileDialog onSaveDialog(this, "Save Game of Life File", wxEmptyString, wxEmptyString, "Game of Life Files (*.cells) | .cells", wxFD_SAVE | wxFD_OVERWRITE_PROMPT);

	if (onSaveDialog.ShowModal() == wxID_CANCEL)
		return;

	std::ofstream fileStream;
	fileName = (std::string)onSaveDialog.GetPath();
	fileStream.open(fileName);

	if (fileStream.is_open()) {
		for (int i = 0; i < gameBoard.size(); i++) {
			for (int j = 0; j < gameBoard[i].size(); j++) {
				if (gameBoard[i][j]) {
					fileStream << "*";
				}
				else {
					fileStream << ".";
				}
			
			}

			fileStream << "\n";
		}
		fileStream.close();
	}
	event.Skip();
}

void MainWindow::Exit(wxCommandEvent& event) {
	Close();
	event.Skip();
}

void MainWindow::ImportFile(wxCommandEvent& event) {
	wxFileDialog onOpenDialog(this, _("Import Universe File"), "", "", "Cells files (*.cells) | .cells", wxFD_OPEN | wxFD_FILE_MUST_EXIST);

	if (onOpenDialog.ShowModal() == wxID_CANCEL)
		return;

	std::string buffer;
	std::ifstream fileStream;
	int index = 0;

	fileName = (std::string)onOpenDialog.GetPath();
	fileStream.open((fileName));

	if (fileStream.is_open()) {
		while (!fileStream.eof()) {
			std::getline(fileStream, buffer);
			if (buffer.size() == 0) { break; }
			for (int i = 0; i < buffer.size(); i++) {
				if (buffer[i] == '*') {
					gameBoard[index][i] = true;
				}
				else {
					gameBoard[index][i] = false;
				}
			}
			index++;
		}

		fileStream.close();
	}

	event.Skip();
}

void MainWindow::FiniteUniverse(wxCommandEvent& event) {
	settings.isFinite = true;
	RefreshMenuItems();
	event.Skip();
}

void MainWindow::TorodialUniverse(wxCommandEvent& event) {
	settings.isFinite = false;
	RefreshMenuItems();
	event.Skip();
}

void MainWindow::RefreshMenuItems() {
	_finite->Check(settings.isFinite);
	_torodial->Check(!settings.isFinite);
}

void MainWindow::DisplayGrid(wxCommandEvent& event) {
	settings.displayGrid = event.IsChecked();
	event.Skip();
}

void MainWindow::OnClose(wxCloseEvent& event) {
	Destroy();
}