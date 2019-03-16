#pragma once

#include <wx/wx.h>

class BackgroundPanel : public wxPanel
{
	wxImage image;
	wxBitmap resized;
	int w, h;

public:
	BackgroundPanel(wxFrame* parent, wxString file, wxBitmapType format);

	void paintEvent(wxPaintEvent & evt);
	void paintNow();
	void OnSize(wxSizeEvent& event);
	void render(wxDC& dc);
	DECLARE_EVENT_TABLE()
};
