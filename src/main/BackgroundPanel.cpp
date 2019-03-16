#pragma once
#include "BackgroundPanel.hpp"

BackgroundPanel::BackgroundPanel(wxFrame* parent, wxString file, wxBitmapType format)
	: wxPanel(parent)
{
	image.LoadFile(file, format);
	w = -1;
	h = -1;
}

void BackgroundPanel::paintEvent(wxPaintEvent & evt)
{
	wxPaintDC dc(this);
	render(dc);
}

void BackgroundPanel::paintNow()
{
	wxClientDC dc(this);
	render(dc);
}

void BackgroundPanel::render(wxDC&  dc)
{
	int neww, newh;
	dc.GetSize(&neww, &newh);

	if (neww != w || newh != h)
	{
		resized = wxBitmap(image.Scale(neww, newh, wxIMAGE_QUALITY_BICUBIC));
		w = neww;
		h = newh;
		dc.DrawBitmap(resized, 0, 0, false);
	}
	else {
		dc.DrawBitmap(resized, 0, 0, false);
	}
}

void BackgroundPanel::OnSize(wxSizeEvent& event) {
	Refresh();
	event.Skip();
}

BEGIN_EVENT_TABLE(BackgroundPanel, wxPanel)
EVT_PAINT(BackgroundPanel::paintEvent)
EVT_SIZE(BackgroundPanel::OnSize)
END_EVENT_TABLE()