#pragma once

#include "wx/wx.h"
#include "wx/dcbuffer.h"

class hjDrawPanel : public wxScrolled<wxWindow> {
public:

	hjDrawPanel(wxWindow*);

	void paintEvent(wxPaintEvent&);
	void paintNow();
	void OnLeftDown(wxMouseEvent&);
	void OnLeftUp(wxMouseEvent&);
	void OnMotion(wxMouseEvent&);
	void OnLeftDoubleClick(wxMouseEvent&);
	void OnRightDown(wxMouseEvent&);
	void OnRightUp(wxMouseEvent&);
	void OnRightDoubleClick(wxMouseEvent&);
	void OnEraseBackground(wxEraseEvent&);

	void render(wxDC&);

};