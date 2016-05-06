#pragma once

#include "vardata.h"

class intdata : public vardata {
public:

	intdata();
	intdata(const int&);

	int value;

};
