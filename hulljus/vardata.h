#pragma once

#include "gennode.h"

class vardata {
public:

	vardata();
	vardata(vartype, nodetype);

	virtual void forPolymorphism();
	
	vartype type;
	nodetype datatype;

};