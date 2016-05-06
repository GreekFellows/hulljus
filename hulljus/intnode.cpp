#include "intnode.h"
#include "hjApp.h"

wxDECLARE_APP(hjApp);

intnode::intnode(
	const double &cx,
	const double &cy,
	const int &clabel
	) :varnode(cx, cy, std::to_string(clabel)) {
	type = nodetype::INTNODE;
}

wxRect intnode::calcRect() {
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

wxRect intnode::calcMidRect() {
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
void intnode::setLabel(const std::string &clabel) {

}

intdata* intnode::getVardata() {
	return &data;
}

void intnode::setVardata(const intdata &cdata) {
	data = cdata;
	label = std::to_string(data.value);
}

bool intnode::setFromStringIfValid(const std::string &cstring) {
	int newValue;

	try {
		newValue = std::stoi(cstring);
	}
	catch (std::invalid_argument) {
		return false;
	}
	catch (std::out_of_range) {
		return false;
	}
	
	setVardata(intdata(newValue));
	return true;
}