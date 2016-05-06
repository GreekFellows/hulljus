#pragma once

#include <string>

#include "wx/wx.h"
#include "varnode.h"
#include "intdata.h"

class intnode : public varnode {
public:

	intnode(const double&, const double&, const int&);

	virtual wxRect calcRect();
	virtual wxRect calcMidRect();

	// setLabel has not any effect because it should not be used anyway
	virtual void setLabel(const std::string&);

	intdata* getVardata();
	void setVardata(const intdata&);

	bool setFromStringIfValid(const std::string&);

protected:

	intdata data;

};