#include "App.h"

wxIMPLEMENT_APP(App);

App::App() {

}

App::~App() {

}

bool App::OnInit() {

	mainWindow = new MainWindow(nullptr, wxID_ANY,"Game of Life", wxPoint(0,0), wxSize(980,980));
	mainWindow->settings.LoadData();
	mainWindow->Show();

	return true;
}