#pragma once

#include "wx/wx.h"
#include "MainWindow.h"

class App : public wxApp
{
private:

public:
	App();
	~App();
	virtual bool OnInit();

	MainWindow* mainWindow;
};

