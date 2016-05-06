#include "strnode.h"
#include "hjApp.h"

wxDECLARE_APP(hjApp);

strnode::strnode(
	const double &cx,
	const double &cy,
	const std::string &clabel
	) :varnode(cx, cy, clabel) {
	type = nodetype::STRNODE;
}

wxRect strnode::calcRect() {
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

wxRect strnode::calcMidRect() {
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

// setLabel is empty because using it does not make any sense
void strnode::setLabel(const std::string &clabel) {

}

strdata* strnode::getVardata() {
	return &data;
}

void strnode::setVardata(const strdata &cdata) {
	data = cdata;
	label = data.value;
}

bool strnode::setFromStringIfValid(const std::string &cstring) {
	setVardata(strdata(cstring));
	return true;
}