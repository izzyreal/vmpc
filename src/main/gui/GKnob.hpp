#ifndef GKNOB_H
#define GKNOB_H

#include <wx/window.h>
#include <wx/panel.h>
#include <wx/dcbuffer.h> 
#include <wx/msgdlg.h>
#include <wx/image.h>

#define FDR_NO_MOUSE_CAPTURE 0x0001

class GKnobEvent: public wxCommandEvent 
{ 
	public: 
		GKnobEvent(wxEventType commandType = wxEVT_NULL, int id = 0 ); 

		float GetValue() const { return faderValue; } 
		void SetValue(float value) { faderValue = value; } 

		virtual wxEvent *Clone() const { return new GKnobEvent(*this); } 

	
	private: 
		float faderValue; 

	DECLARE_DYNAMIC_CLASS(GKnobEvent) 
}; 

BEGIN_DECLARE_EVENT_TYPES() 
	DECLARE_EVENT_TYPE(wxEVT_COMMAND_GKNOB, -1) 
END_DECLARE_EVENT_TYPES() 

typedef void (wxEvtHandler::*wxGKnobEventFunction)(GKnobEvent&); 

#define EVT_GKNOB(id, fn) \
	DECLARE_EVENT_TABLE_ENTRY(wxEVT_COMMAND_GKNOB, id, -1,  \
	(wxObjectEventFunction) (wxEventFunction) (wxCommandEventFunction)  \
	wxStaticCastEvent(wxGKnobEventFunction, & fn), (wxObject *) NULL ),


class GKnob: public wxPanel
{ 
	public: 
		GKnob(wxWindow* parent, wxWindowID id, const wxPoint& pos, long style, const wxString& name, const wxString& knob);

		bool Create(wxWindow* parent, wxWindowID id, const wxPoint& pos, long style, const wxString& name, const wxString& knob);

		virtual float GetValue() const; 
		virtual void SetValue(float value); 
		virtual void ResetStoredValue();

		virtual void SetRange(float minValue, float maxValue); 
		virtual float GetMin() const; 
		virtual float GetMax() const; 

		bool focused;

	protected: 
		void Init(); 
		float NormalizeValue(float value) const; 
		bool UseMouseCapture(); 
		float ValueFromVertPosition(wxCoord position); 
		void CalcGeometry(); 

		void DrawKnob(wxDC &dc); 

		wxString FormatValue(float value) const; 

		void OnSize(wxSizeEvent &event); 
		void OnEraseBackground(wxEraseEvent &event);
		void OnPaint(wxPaintEvent &event); 
		void OnMouse(wxMouseEvent &event); 
		void OnKeyDown(wxKeyEvent &event);

	private:
		int knobPosX, 
			knobPosY, 
			knobWidth, 
			knobHeight; 

		int tickPosY, 
			tickPosX, 
			tickWidth, 
			tickHeight; 

		// Mouse logic 
		int mouseOffsetY, 
			wheelRotation, 
			wheelStep; 
		bool draggingKnob, 
			wheelKnobMove; 

		float minValue, 
			maxValue, 
			currentValue, 
		    storedValue,
		    gainValue;

		wxBitmap knobs;
		
		DECLARE_EVENT_TABLE() 
};

#endif //GKNOB_H