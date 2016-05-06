#pragma once

#include <string>

#include "vardata.h"

class strdata : public vardata {
public:

	strdata();
	strdata(const std::string&);

	std::string value;

};
