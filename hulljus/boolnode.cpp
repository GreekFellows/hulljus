#include "boolnode.h"
#include "hjApp.h"

wxDECLARE_APP(hjApp);

boolnode::boolnode(
	const double &cx,
	const double &cy,
	const bool &clabel
	) :varnode(cx, cy, std::to_string(clabel)) {
	type = nodetype::BOOLNODE;
}

wxRect boolnode::calcRect() {
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

wxRect boolnode::calcMidRect() {
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
void boolnode::setLabel(const std::string &clabel) {

}

booldata* boolnode::getVardata() {
	return &data;
}

void boolnode::setVardata(const booldata &cdata) {
	data = cdata;
	label = std::to_string(data.value);
}

bool boolnode::setFromStringIfValid(const std::string &cstring) {
	if (cstring == "true" || cstring == "t") {
		setVardata(booldata(true));
		return true;
	}
	else if (cstring == "false" || cstring == "f") {
		setVardata(booldata(false));
		return true;
	}
	else {
		int tmpValue;
		bool intConvertible = true;
		try {
			tmpValue = std::stoi(cstring);
		}
		catch (std::invalid_argument) {
			intConvertible = false;
		}
		catch (std::out_of_range) {
			intConvertible = false;
		}

		if (intConvertible) {
			setVardata(booldata(tmpValue));
			return true;
		}
	}

	return false;
}