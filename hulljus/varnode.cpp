#include "varnode.h"
#include "hjApp.h"

wxDECLARE_APP(hjApp);

varnode::varnode(
	const double &cx,
	const double &cy,
	const std::string &clabel
	) :gennode(cx, cy, clabel) {

}

wxRect varnode::calcRect() { return wxRect(); }
wxRect varnode::calcMidRect() { return wxRect(); }

// setLabel is empty because using it does not make any sense
void varnode::setLabel(const std::string &clabel) {

}

vardata* varnode::getVardata() {
	return NULL;
}

void varnode::setVardata(const vardata &cdata) {

}

bool varnode::setFromStringIfValid(const std::string &cstring) {
	return true;
}