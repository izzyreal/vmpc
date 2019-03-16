#include "App.hpp"
#include "BackgroundPanel.hpp"

#include "gui/GKnob.hpp"

#include "StartUp.hpp"

#include "file/FileUtil.hpp"

using namespace moduru;
using namespace std;

bool App::OnInit() {
	wxInitAllImageHandlers();

	const int screenWidth = wxSystemSettings::GetMetric(wxSYS_SCREEN_X);
	const int screenHeight = wxSystemSettings::GetMetric(wxSYS_SCREEN_Y);

	const float aspectRatio = 1298.f / 994.f;

	const int initialFrameHeight = static_cast<int>(1.f * screenHeight);
	const int initialFrameWidth = static_cast<int>(initialFrameHeight * aspectRatio);

	const int initialX = static_cast<int>((screenWidth - initialFrameWidth) / 2.f);
	const int style = wxMINIMIZE_BOX | wxSYSTEM_MENU | wxCAPTION | wxCLOSE_BOX | wxCLIP_CHILDREN;
	frame = new wxFrame(NULL, wxID_ANY, wxT("vMPC2000XL"), wxPoint(initialX, 20), wxSize(initialFrameWidth, initialFrameHeight), style);
	
	const auto sep = file::FileUtil::getSeparator();
	const auto resPath = mpc::StartUp::resPath;
	const auto imgPath = resPath + sep + "img" + sep;
	const auto dataWheelPath = imgPath + "datawheels.png";
	const auto bgPath = imgPath + "bg.png";

	drawPane = new BackgroundPanel(frame, bgPath.c_str(), wxBITMAP_TYPE_PNG);

	auto knob = new GKnob(drawPane, wxID_ANY, wxPoint(20, 20), 0, wxString("knob"), dataWheelPath.c_str());
	knob->SetRange(0, 1000);
	knob->SetValue(500);

	knob->Show();
	frame->Show();

	return true;
}
