#include "booldata.h"

booldata::booldata() :vardata(vartype::NUMBER, nodetype::BOOLNODE) {

}

booldata::booldata(const bool &cvalue) : vardata(vartype::NUMBER, nodetype::BOOLNODE) {
	value = cvalue;
}