#pragma once

#include "vardata.h"

class booldata : public vardata {
public:

	booldata();
	booldata(const bool&);

	bool value;

};
