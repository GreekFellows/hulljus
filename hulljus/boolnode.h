#pragma once

#include <string>

#include "wx/wx.h"
#include "varnode.h"
#include "booldata.h"

class boolnode : public varnode {
public:

	boolnode(const double&, const double&, const bool&);

	virtual wxRect calcRect();
	virtual wxRect calcMidRect();

	// setLabel has not any effect because it should not be used anyway
	virtual void setLabel(const std::string&);

	booldata* getVardata();
	void setVardata(const booldata&);

	bool setFromStringIfValid(const std::string&);

protected:

	booldata data;

};