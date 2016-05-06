#pragma once

#include <string>

#include "wx/wx.h"
#include "gennode.h"

class connode : public gennode {
public:

	connode(const double&, const double&, const std::string&);

	virtual wxRect calcRect();
	virtual wxRect calcMidRect();

};