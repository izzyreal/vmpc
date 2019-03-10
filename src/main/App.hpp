#pragma once
//#include <wx/wxprec.h>

#include <wx/wx.h>

class App : public wxApp
{
public:
	App();
	~App();

public:
	bool OnInit() override;
};

wxIMPLEMENT_APP(App);