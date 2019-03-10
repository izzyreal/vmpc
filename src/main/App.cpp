#include "App.hpp"

#include "MainFrame.hpp"

App::App()
{
}


App::~App()
{
}

bool App::OnInit() {
	auto mf = new MainFrame();
	mf->Show(true);
	return true;
}
