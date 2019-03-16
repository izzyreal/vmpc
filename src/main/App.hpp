#pragma once

#include <wx/wx.h>

class BackgroundPanel;

class App : public wxApp
{

private:
	wxFrame *frame;
	BackgroundPanel* drawPane;

public:
	bool OnInit();

};

IMPLEMENT_APP(App)
