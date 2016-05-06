#include "gennode.h"

vartype getVartypeFromTypeInfo(const type_info &ti) {
	if (ti.name() == "int" || ti.name() == "bool") {
		return vartype::NUMBER;
	}
	else if (ti.name() == "std::string") {
		return vartype::STRING;
	}
}

vartype getVartypeFromNodetype(nodetype nt) {
	if (nt == INTNODE || nt == BOOLNODE) {
		return vartype::NUMBER;
	}
	else if (nt == STRNODE) {
		return vartype::STRING;
	}
}

gennode::gennode(
	const double &cx,
	const double &cy,
	const std::string &clabel
	) :x(cx), y(cy), label(clabel) {
	type = nodetype::GENNODE;
}

// use std::vector::emplace_back(Args&&... ) 
// http://stackoverflow.com/questions/4303513/push-back-vs-emplace-back

void gennode::addFromnode(gennode *fromnode) {
	fromnodes.emplace_back(fromnode);
}

void gennode::addTonode(gennode *tonode) {
	tonodes.emplace_back(tonode);
}

void gennode::addInnode(gennode *innode) {
	innodes.emplace_back(innode);
}

void gennode::addOutnode(gennode *outnode) {
	outnodes.emplace_back(outnode);
}

std::string gennode::getLabel() {
	return label;
}

void gennode::setLabel(const std::string &clabel) {
	label = clabel;
}

wxRect gennode::calcRect() { return wxRect(); }
wxRect gennode::calcMidRect() { return wxRect(); }
wxRect gennode::calcFromBand() { return wxRect(); }
wxRect gennode::calcToBand() { return wxRect(); }