#include "GKnob.hpp" 

DEFINE_EVENT_TYPE(wxEVT_COMMAND_GKNOB)

IMPLEMENT_DYNAMIC_CLASS(GKnobEvent, wxCommandEvent)

GKnobEvent::GKnobEvent(wxEventType commandType, int id) : wxCommandEvent(commandType, id)
{
}

BEGIN_EVENT_TABLE(GKnob, wxPanel)
EVT_PAINT(GKnob::OnPaint)
EVT_ERASE_BACKGROUND(GKnob::OnEraseBackground)
EVT_MOUSE_EVENTS(GKnob::OnMouse)
EVT_KEY_DOWN(GKnob::OnKeyDown)
END_EVENT_TABLE()

GKnob::GKnob(wxWindow* parent, wxWindowID id, const wxPoint& pos, long style, const wxString& name, const wxString& knob)
{
	Init();
	(void)Create(parent, id, pos, style, name, knob);
}

void GKnob::Init()
{
	mouseOffsetY = 0;
	storedValue = 141;
	draggingKnob = false;
	wheelKnobMove = false;
}

bool GKnob::Create(wxWindow* parent, wxWindowID id, const wxPoint& pos, long style, const wxString& name, const wxString& knob)
{
	const wxBitmap &image = wxImage(knob, wxBITMAP_TYPE_PNG);
	knobs = image;

	knobWidth = 171;
	knobHeight = 171;
	knobPosX = 0;
	knobPosY = 0;

	if (!wxPanel::Create(parent, id, pos, wxSize(knobWidth, knobHeight), style, name))
		return false;

	return true;
}

float GKnob::NormalizeValue(float value) const
{
	if (value < minValue)
		return minValue;
	else if (value > maxValue)
		return maxValue;
	else
		return value;
}

float GKnob::GetValue() const
{
	return currentValue;
}

void GKnob::SetValue(float value)
{
	value = NormalizeValue(value);

	if (currentValue == value)
		return;

	currentValue = value;

	Refresh();
}

void GKnob::ResetStoredValue()
{
	storedValue = (212 - ((((currentValue * 100) / maxValue) * 141) / 100));
}

float GKnob::GetMin() const
{
	return minValue;
}

float GKnob::GetMax() const
{
	return maxValue;
}

void GKnob::SetRange(float minValue, float maxValue)
{
	if (minValue > maxValue)
	{
		float tmp = minValue;
		minValue = maxValue;
		maxValue = tmp;
	}

	if (minValue != minValue || maxValue != maxValue)
	{
		minValue = minValue;
		maxValue = maxValue;
		SetValue(currentValue);
		Refresh();
	}
}

bool GKnob::UseMouseCapture()
{
	if (GetWindowStyle() & FDR_NO_MOUSE_CAPTURE)
		return false;
	else {
		return true;
	}
}

void GKnob::OnEraseBackground(wxEraseEvent &event)
{
	// This is necessary for I forgot what
}

void GKnob::OnPaint(wxPaintEvent &WXUNUSED(event))
{
	wxBufferedPaintDC dc(this);
	DrawKnob(dc);
}

void GKnob::OnMouse(wxMouseEvent &event)
{
	wxClientDC dc(this);
	PrepareDC(dc);

	wxPoint pos = event.GetPosition();
	long x = dc.DeviceToLogicalX(pos.x);
	long y = dc.DeviceToLogicalY(pos.y);

	if (x >= knobPosX && x <= knobPosX + knobWidth
		&& y >= knobPosY && y <= knobPosY + knobHeight)
	{
		if (event.LeftDown()) {
			SetFocus();
			mouseOffsetY = y;
			draggingKnob = true;
			wheelKnobMove = false;
			if (UseMouseCapture()) CaptureMouse();
		}
	}

	if (event.GetWheelRotation() == 120) {
		SetValue(currentValue + 1);
		GKnobEvent event(wxEVT_COMMAND_GKNOB, GetId());
		event.SetEventObject(this);
		event.SetValue(currentValue);
		GetEventHandler()->ProcessEvent(event);
	}

	if (event.GetWheelRotation() == -120) {
		SetValue(currentValue - 1);
		GKnobEvent event(wxEVT_COMMAND_GKNOB, GetId());
		event.SetEventObject(this);
		event.SetValue(currentValue);
		GetEventHandler()->ProcessEvent(event);
	}

	if (event.LeftDClick()) {
		SetValue(maxValue / 2);
		GKnobEvent event(wxEVT_COMMAND_GKNOB, GetId());
		event.SetEventObject(this);
		event.SetValue(currentValue);
		GetEventHandler()->ProcessEvent(event);
	}

	if (event.LeftUp() || (event.Leaving() && !UseMouseCapture()))
	{
		draggingKnob = false;
		wheelKnobMove = false;
		mouseOffsetY = 0;
		storedValue = (212 - ((((currentValue * 100) / maxValue) * 141) / 100));

		if (HasCapture()) {
			ReleaseMouse();
		}
		return;
	}

	if (draggingKnob) {
		if (event.Dragging()) {
			SetValue(ValueFromVertPosition((storedValue - (mouseOffsetY - y))));
		}
		else {
			return;
		}
	}

	if (draggingKnob) {
		GKnobEvent event(wxEVT_COMMAND_GKNOB, GetId());
		event.SetEventObject(this);
		event.SetValue(currentValue);
		GetEventHandler()->ProcessEvent(event);
	}

	if (event.RightUp()) {
		//Alter this to give the object it's own right click properties
	}
}

void GKnob::OnKeyDown(wxKeyEvent &event)
{
	if (event.GetKeyCode() == WXK_UP || event.GetKeyCode() == WXK_NUMPAD_UP) {
		SetValue(currentValue + 1);

		GKnobEvent event2(wxEVT_COMMAND_GKNOB, GetId());
		event2.SetEventObject(this);
		event2.SetValue(currentValue);
		GetEventHandler()->ProcessEvent(event2);
	}

	if (event.GetKeyCode() == WXK_DOWN || event.GetKeyCode() == WXK_NUMPAD_DOWN) {
		SetValue(currentValue - 1);
		GKnobEvent event2(wxEVT_COMMAND_GKNOB, GetId());
		event2.SetEventObject(this);
		event2.SetValue(currentValue);
		GetEventHandler()->ProcessEvent(event2);
	}
}

float GKnob::ValueFromVertPosition(wxCoord position)
{
	if (position < 71) { position = 71; }
	if (position > 212) { position = 212; }
	position = 212 - position;
	position = (position * 100) / 141;
	position = (position * maxValue) / 100;

	return position;
}


void GKnob::CalcGeometry()
{
	int totalWidth = 0,
		totalHeight = 0;

	GetClientSize(&totalWidth, &totalHeight);

}

wxString GKnob::FormatValue(float value) const
{
	return wxString::Format(wxT("%d"), (int)value);
}

void GKnob::DrawKnob(wxDC &dc)
{
	wxPoint offset;
	wxImage temp = knobs.ConvertToImage();

	auto index = ((maxValue - minValue) / 100.f) * currentValue;
	auto cropped = temp.GetSubImage(wxRect(wxPoint(0, 0), wxPoint(171, 171)));
	dc.DrawBitmap(cropped, offset.x, offset.y, false);
}
