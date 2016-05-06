#pragma once

#include <string>

#include "wx/wx.h"
#include "gennode.h"
#include "vardata.h"

class varnode : public gennode {
public:

	varnode(const double&, const double&, const std::string&);

	virtual wxRect calcRect();
	virtual wxRect calcMidRect();

	// setLabel has not any effect because it should not be used anyway
	virtual void setLabel(const std::string&);

	virtual vardata* getVardata();
	virtual void setVardata(const vardata&);

	virtual bool setFromStringIfValid(const std::string&);

};