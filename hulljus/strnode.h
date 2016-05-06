#pragma once

#include <string>

#include "wx/wx.h"
#include "varnode.h"
#include "strdata.h"

class strnode : public varnode {
public:

	strnode(const double&, const double&, const std::string&);

	virtual wxRect calcRect();
	virtual wxRect calcMidRect();

	// setLabel has not any effect because it should not be used anyway
	virtual void setLabel(const std::string&);

	strdata* getVardata();
	void setVardata(const strdata&);

	bool setFromStringIfValid(const std::string&);

protected:

	strdata data;

};