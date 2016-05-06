#include "vardata.h"

vardata::vardata() {

}

vardata::vardata(vartype vt, nodetype dt) {
	type = vt;
	datatype = dt;
}

void vardata::forPolymorphism() {}