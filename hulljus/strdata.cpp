#include "strdata.h"

strdata::strdata() :vardata(vartype::STRING, nodetype::STRNODE) {

}

strdata::strdata(const std::string &cvalue) : vardata(vartype::STRING, nodetype::STRNODE) {
	value = cvalue;
}