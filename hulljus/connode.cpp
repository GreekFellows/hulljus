#include "connode.h"
#include "hjApp.h"

wxDECLARE_APP(hjApp);

connode::connode(
	const double &cx,
	const double &cy,
	const std::string &clabel
	) :gennode(cx, cy, clabel) {
	type = nodetype::CONNODE;
	varnectionsAvailable = false;
}

wxRect connode::calcRect() {
	wxClientDC dc(wxGetApp().frame->drawpanel);
	wxCoord width, height;
	dc.GetTextExtent(label, &width, &height);
	return wxRect(
		x,
		y,
		width + 2 * wxGetApp().labelHorizontalMargin,
		height + 2 * (wxGetApp().labelVerticalMargin + wxGetApp().fromtoBandHeight)
		);
}

wxRect connode::calcMidRect() {
	wxClientDC dc(wxGetApp().frame->drawpanel);
	wxCoord width, height;
	dc.GetTextExtent(label, &width, &height);
	return wxRect(
		x,
		y + wxGetApp().fromtoBandHeight,
		width + 2 * wxGetApp().labelHorizontalMargin,
		height + 2 * wxGetApp().labelVerticalMargin
		);
}