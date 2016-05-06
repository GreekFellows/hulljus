#include "intdata.h"

intdata::intdata() :vardata(vartype::NUMBER, nodetype::INTNODE) {

}

intdata::intdata(const int &cvalue) : vardata(vartype::NUMBER, nodetype::INTNODE) {
	value = cvalue;
}