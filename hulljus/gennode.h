#pragma once

#include <string>
#include <vector>

#include "wx/wx.h"

// other than gennode or connode, they are all varnodes
// they are also used for vardatas to indicate the type of data they contain
enum nodetype {
	GENNODE = 0,
	CONNODE = 1,
	INTNODE = 2,
	STRNODE = 3,
	BOOLNODE = 4
};

// types of variables indicated by the nodetypes of varnodes
enum vartype {
	NUMBER = 0,
	STRING = 1,
	COMPOSITE = 2
};

vartype getVartypeFromTypeInfo(const type_info &ti);
vartype getVartypeFromNodetype(nodetype nt);

class gennode {
public:

	gennode(const double&, const double&, const std::string&);

	void addFromnode(gennode*);
	void addTonode(gennode*);
	void addInnode(gennode*);
	void addOutnode(gennode*);
	virtual wxRect calcRect();
	virtual wxRect calcMidRect();
	virtual wxRect calcFromBand();
	virtual wxRect calcToBand();

	double x, y;
	int zIndex;
	nodetype type;

	std::vector<gennode*> fromnodes, tonodes, innodes, outnodes;

	// when activated, this shall be set to 1! otherwise it will keep decreasing until 0.
	double isActivated = 0;

	bool connectionsAvailable = true;
	bool varnectionsAvailable = true;

	virtual std::string getLabel();
	virtual void setLabel(const std::string&);

protected:

	std::string label;

};