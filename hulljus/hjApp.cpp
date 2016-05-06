#include "hjApp.h"

wxIMPLEMENT_APP(hjApp);

void hjApp::startFromScratch() {
	// this is called when File->New is pressed

	// reset everything
	edited = NULL;
	selection = NULL;
	startConnect = NULL;
	activatedNodes.clear();
	newActivatedNodes.clear();

	// information in varRegister and gennodes are dynamically allocated, hence must be deleted
	for (auto iter = varRegister.begin(); iter != varRegister.end(); ++iter) {
		delete (*iter).second;
	}
	varRegister.clear();

	for (auto iter = gennodes.begin(); iter != gennodes.end(); ++iter) {
		delete *iter;
	}
	gennodes.clear();

	// reset file streams
	writer.close();
}

void hjApp::openFile(const std::string &filepath) {
	reader.open(filepath);

	if (reader.is_open()) {
		startFromScratch();

		std::string data;
		reader >> data;

		auto tildeFound = data.find('~');
		std::stringstream part1ss(data.substr(0, tildeFound));
		std::stringstream part2ss(data.substr(tildeFound + 1));

		// fancy for usage as in http://en.cppreference.com/w/cpp/io/basic_istream/getline
		for (std::string nodeinfo; std::getline(part1ss, nodeinfo, ';');) {
			std::stringstream nodeinfoss(nodeinfo);
			std::string nodetypes, labels, xpositions, ypositions;
			std::getline(nodeinfoss, nodetypes, ',');
			std::getline(nodeinfoss, labels, ',');
			std::getline(nodeinfoss, xpositions, ',');
			std::getline(nodeinfoss, ypositions, ',');

			int nodetypei = std::stoi(nodetypes);
			int xpositioni = std::stoi(xpositions);
			int ypositioni = std::stoi(ypositions);

			// http://stackoverflow.com/questions/20177495/error-c2360-initialization-of-hdc-is-skipped-by-case-label
			switch (nodetypei) {
			case nodetype::CONNODE: {
				connode *newconnode = new connode(xpositioni, ypositioni, labels);
				gennodes.push_back(newconnode);
				break;
			}

			case nodetype::INTNODE: {
				intnode *newintnode = new intnode(xpositioni, ypositioni, 0);
				newintnode->setFromStringIfValid(labels);
				gennodes.push_back(newintnode);
				break;
			}

			case nodetype::STRNODE: {
				strnode *newstrnode = new strnode(xpositioni, ypositioni, "");
				newstrnode->setFromStringIfValid(labels);
				gennodes.push_back(newstrnode);
				break;
			}

			case nodetype::BOOLNODE: {
				boolnode *newboolnode = new boolnode(xpositioni, ypositioni, true);
				newboolnode->setFromStringIfValid(labels);
				gennodes.push_back(newboolnode);
				break;
			}
			}
		}

		auto node = gennodes.begin();
		for (std::string coninfo; std::getline(part2ss, coninfo, ';');) {
			if (!coninfo.empty()) {
				std::stringstream coninfoss(coninfo);

				std::string connections, varnections;
				std::getline(coninfoss, connections, ',');
				std::getline(coninfoss, varnections, ',');

				if (!connections.empty()) {
					std::stringstream connectionss(connections);
					for (std::string con; std::getline(connectionss, con, ' ');) {
						if (!con.empty()) {
							(*node)->addTonode(gennodes[std::stoi(con)]);
							gennodes[std::stoi(con)]->addFromnode(*node);
						}
					}
				}

				if (!varnections.empty()) {
					std::stringstream varnectionss(varnections);
					for (std::string var; std::getline(varnectionss, var, ' ');) {
						if (!var.empty()) {
							(*node)->addOutnode(gennodes[std::stoi(var)]);
							gennodes[std::stoi(var)]->addInnode(*node);
						}
					}
				}
			}

			++node;
		}
	}

	reader.close();
}

void hjApp::saveFile(const std::string &filepath) {
	if (!filepath.empty()) {
		writepath = filepath;
	}

	if (!writepath.empty()) {
		writer.open(writepath);

		std::string data;
		std::vector<std::string> part2s;

		for (auto iter = gennodes.begin(); iter != gennodes.end(); ++iter) {
			writer << (*iter)->type << "," << (*iter)->getLabel() << "," << (*iter)->x << "," << (*iter)->y;
			if (iter + 1 != gennodes.end()) {
				writer << ";";
			}

			std::string coninfo;
			for (auto titer = (*iter)->tonodes.begin(); titer != (*iter)->tonodes.end(); ++titer) {
				coninfo += std::to_string(std::distance(gennodes.begin(), titer));
				if (titer + 1 != (*iter)->tonodes.end()) {
					coninfo += " ";
				}
			}

			std::string varinfo;
			for (auto oiter = (*iter)->outnodes.begin(); oiter != (*iter)->outnodes.end(); ++oiter) {
				varinfo += std::to_string(std::distance(gennodes.begin(), oiter));
				if (oiter + 1 != (*iter)->outnodes.end()) {
					varinfo += " ";
				}
			}

			part2s.push_back(coninfo + "," + varinfo);
		}

		writer << "~";

		for (auto part2str = part2s.begin(); part2str != part2s.end(); ++part2str) {
			writer << *part2str;
			if (part2str + 1 != part2s.end()) {
				writer << ";";
			}
		}

		writer.close();
	}
}

bool hjApp::OnInit() {
	onRenderLoop = false;
	virtualSize = new wxSize(800, 450);

	edited = NULL;
	selection = NULL;

	activateRenderLoop(true);

	frame = new hjFrame("hulljus v0", wxPoint(50, 50), wxSize(800, 450));
	frame->Show(true);

	aboutInfo.SetName("hulljus");
	aboutInfo.SetVersion("v0");
	aboutInfo.SetDescription(_("a graphical programming interface that flickers like mad.\nwelcome, beta testers! exploit this software! :)"));
	aboutInfo.SetCopyright("Copyright 2016");
	aboutInfo.SetWebSite("https://twitter.com/GreekFellows");
	aboutInfo.AddDeveloper("greekfellows");

	return true;
}

void hjApp::activateRenderLoop(bool activate) {
	if (activate && !onRenderLoop) {
		Bind(wxEVT_IDLE, &hjApp::OnIdle, this, wxID_ANY);
		onRenderLoop = true;
	}
	else if (!activate && onRenderLoop) {
		Unbind(wxEVT_IDLE, &hjApp::OnIdle, this, wxID_ANY);
		onRenderLoop = false;
	}
}

void hjApp::OnIdle(wxIdleEvent &event) {
	if (onRenderLoop) {
		frame->drawpanel->paintNow();
		event.RequestMore();
	}

	newActivatedNodes.clear();

	// here we process activated connodes
	for (auto iter = activatedNodes.begin(); iter != activatedNodes.end(); ++iter) {
		bool activateAll = true;

		if ((*iter)->getLabel() == "") {
			
		}
		else if ((*iter)->getLabel() == "popmsg") {
			doPopmsg(*iter);
		}
		else if ((*iter)->getLabel() == "+") {
			doSum(*iter);
		}
		else if ((*iter)->getLabel() == "-") {
			doSubtract(*iter);
		}
		else if ((*iter)->getLabel() == "*") {
			doMultiply(*iter);
		}
		else if ((*iter)->getLabel() == "/") {
			doDivide(*iter);
		}
		else if ((*iter)->getLabel() == "==") {
			doEquality(*iter);
		}
		else if ((*iter)->getLabel() == "!=") {
			doInequality(*iter);
		}
		else if ((*iter)->getLabel() == ">") {
			doGreaterThan(*iter);
		}
		else if ((*iter)->getLabel() == "<") {
			doLessThan(*iter);
		}
		else if ((*iter)->getLabel() == ">=") {
			doGreaterThanOrEqualTo(*iter);
		}
		else if ((*iter)->getLabel() == "<=") {
			doLessThanOrEqualTo(*iter);
		}
		else if ((*iter)->getLabel() == "if") {
			doIf(*iter);
			activateAll = false;
		}

		if (activateAll) {
			for (auto titer = (*iter)->tonodes.begin(); titer != (*iter)->tonodes.end(); ++titer) {
				(*titer)->isActivated = 1;
				newActivatedNodes.push_back((connode*)(*titer));
			}
		}
	}

	activatedNodes = newActivatedNodes;
}

void hjApp::doPopmsg(gennode *node) {
	if (node->innodes.size() == 0) {
		wxMessageBox("yolo!", "popmsg");
	}
	else {
		std::string message;
		getValueFromNode(node->innodes[0], message);
		wxMessageBox(message, "popmsg");
	}
}

void hjApp::doSum(gennode *node) {
	if (node->innodes.size() < 1) {
		hjErrors::reportError(node, hjErrors::NotEnoughParameters);
	}

	// https://en.wikipedia.org/wiki/Addition
	int sum;
	getValueFromNode(node->innodes[0], sum);
	for (auto iiter = node->innodes.begin() + 1; iiter != node->innodes.end(); ++iiter) {
		int summand;
		if (getValueFromNode(*iiter, summand)) {
			sum += summand;
		}
	}

	updateRegister(node, new intdata(sum)); // in the register, we will map this pointer (NOT the outnode!) to the value
	for (auto oiter = node->outnodes.begin(); oiter != node->outnodes.end(); ++oiter) {
		setNodeValue(*oiter, sum);
	}
}

void hjApp::doSubtract(gennode *node) {
	if (node->innodes.size() < 1) {
		hjErrors::reportError(node, hjErrors::NotEnoughParameters);
	}

	int minuend;
	getValueFromNode(node->innodes[0], minuend);
	for (auto iiter = node->innodes.begin() + 1; iiter != node->innodes.end(); ++iiter) {
		int subtrahend;
		if (getValueFromNode(*iiter, subtrahend)) {
			minuend -= subtrahend;
		}
	}

	updateRegister(node, new intdata(minuend));
	for (auto oiter = node->outnodes.begin(); oiter != node->outnodes.end(); ++oiter) {
		setNodeValue(*oiter, minuend);
	}
}

void hjApp::doMultiply(gennode *node) {
	if (node->innodes.size() < 1) {
		hjErrors::reportError(node, hjErrors::NotEnoughParameters);
	}

	int product;
	getValueFromNode(node->innodes[0], product);
	for (auto iiter = node->innodes.begin() + 1; iiter != node->innodes.end(); ++iiter) {
		int factor;
		if (getValueFromNode(*iiter, factor)) {
			product *= factor;
		}
	}

	updateRegister(node, new intdata(product));
	for (auto oiter = node->outnodes.begin(); oiter != node->outnodes.end(); ++oiter) {
		setNodeValue(*oiter, product);
	}
}

void hjApp::doDivide(gennode *node) {
	if (node->innodes.size() < 1) {
		hjErrors::reportError(node, hjErrors::NotEnoughParameters);
	}

	int dividend;
	getValueFromNode(node->innodes[0], dividend);
	for (auto iiter = node->innodes.begin() + 1; iiter != node->innodes.end(); ++iiter) {
		int divisor;
		if (getValueFromNode(*iiter, divisor)) {
			dividend /= divisor;
		}
	}

	updateRegister(node, new intdata(dividend));
	for (auto oiter = node->outnodes.begin(); oiter != node->outnodes.end(); ++oiter) {
		setNodeValue(*oiter, dividend);
	}
}

void hjApp::doEquality(gennode *node) {
	if (node->innodes.size() < 2) {
		hjErrors::reportError(node, hjErrors::NotEnoughParameters);
	}

	vardata *operand0, *operand1;
	getVardataFromNode(node->innodes[0], operand0);
	getVardataFromNode(node->innodes[1], operand1);

	if (operand0->type == operand1->type) {
		bool result;

		if (operand0->datatype == nodetype::INTNODE) {
			int lhs = static_cast<intdata*>(operand0)->value;
			switch (operand1->datatype) {
			case nodetype::INTNODE:
				result = (lhs == static_cast<intdata*>(operand1)->value);
				break;

			case nodetype::BOOLNODE:
				result = (!!lhs && static_cast<booldata*>(operand1)->value);
				break;

			default:
				hjErrors::reportError(node, hjErrors::CannotConvertConnodeToType);
				break;
			}
		}
		else if (operand0->datatype == nodetype::BOOLNODE) {
			bool lhs = static_cast<booldata*>(operand0)->value;
			switch (operand1->datatype) {
			case nodetype::INTNODE:
				// http://stackoverflow.com/questions/1310344/why-use-when-converting-int-to-bool
				result = (lhs && !!static_cast<intdata*>(operand1)->value);
				break;

			case nodetype::BOOLNODE:
				result = (lhs && static_cast<booldata*>(operand1)->value);
				break;

			default:
				hjErrors::reportError(node, hjErrors::CannotConvertConnodeToType);
				break;
			}
		}
		else if (operand0->datatype == nodetype::STRNODE) {
			std::string lhs = static_cast<strdata*>(operand0)->value;
			switch (operand1->datatype) {
			case nodetype::STRNODE:
				result = (lhs == static_cast<strdata*>(operand1)->value);
				break;

			default:
				hjErrors::reportError(node, hjErrors::CannotConvertConnodeToType);
				break;
			}
		}

		updateRegister(node, new booldata(result));
		for (auto oiter = node->outnodes.begin(); oiter != node->outnodes.end(); ++oiter) {
			setNodeValue(*oiter, result);
		}
	}
	else {
		hjErrors::reportError(node, hjErrors::CannotConvertConnodeToType);
	}
}

void hjApp::doInequality(gennode *node) {
	if (node->innodes.size() < 2) {
		hjErrors::reportError(node, hjErrors::NotEnoughParameters);
	}

	vardata *operand0, *operand1;
	getVardataFromNode(node->innodes[0], operand0);
	getVardataFromNode(node->innodes[1], operand1);

	if (operand0->type == operand1->type) {
		bool result;

		if (operand0->datatype == nodetype::INTNODE) {
			int lhs = static_cast<intdata*>(operand0)->value;
			switch (operand1->datatype) {
			case nodetype::INTNODE:
				result = (lhs != static_cast<intdata*>(operand1)->value);
				break;

			case nodetype::BOOLNODE:
				result = (lhs != (int)(static_cast<booldata*>(operand1)->value));
				break;

			default:
				hjErrors::reportError(node, hjErrors::CannotConvertConnodeToType);
				break;
			}
		}
		else if (operand0->datatype == nodetype::BOOLNODE) {
			bool lhs = static_cast<booldata*>(operand0)->value;
			switch (operand1->datatype) {
			case nodetype::INTNODE:
				// http://stackoverflow.com/questions/1310344/why-use-when-converting-int-to-bool
				result = ((int)lhs != static_cast<intdata*>(operand1)->value);
				break;

			case nodetype::BOOLNODE:
				result = (lhs != static_cast<booldata*>(operand1)->value);
				break;

			default:
				hjErrors::reportError(node, hjErrors::CannotConvertConnodeToType);
				break;
			}
		}
		else if (operand0->datatype == nodetype::STRNODE) {
			std::string lhs = static_cast<strdata*>(operand0)->value;
			switch (operand1->datatype) {
			case nodetype::STRNODE:
				result = (lhs != static_cast<strdata*>(operand1)->value);
				break;

			default:
				hjErrors::reportError(node, hjErrors::CannotConvertConnodeToType);
				break;
			}
		}

		updateRegister(node, new booldata(result));
		for (auto oiter = node->outnodes.begin(); oiter != node->outnodes.end(); ++oiter) {
			setNodeValue(*oiter, result);
		}
	}
	else {
		hjErrors::reportError(node, hjErrors::CannotConvertConnodeToType);
	}
}

void hjApp::doGreaterThan(gennode *node) {}
void hjApp::doLessThan(gennode *node) {}
void hjApp::doGreaterThanOrEqualTo(gennode *node) {}
void hjApp::doLessThanOrEqualTo(gennode *node) {}
void hjApp::doNot(gennode *node) {}
void hjApp::doAnd(gennode *node) {}
void hjApp::doOr(gennode *node) {}

void hjApp::doIf(gennode *node) {
	if (node->innodes.size() < 1) { // for condition
		hjErrors::reportError(node, hjErrors::NotEnoughParameters);
	}

	if (node->tonodes.size() < 2) { // for if-true statements and end-if CONTROL END; ELSE IF is optional
		hjErrors::reportError(node, hjErrors::NotEnoughParameters);
	}

	bool condition;
	if (getValueFromNode(node->innodes[0], condition)) {
		if (condition) {
			for (auto titer = node->tonodes.begin(); titer != node->tonodes.end(); ++titer) {
				// then
				if ((*titer)->getLabel() == "then") {
					(*titer)->isActivated = 1;
					newActivatedNodes.push_back((connode*)(*titer));
				}
			}
		}
		else {
			for (auto titer = node->tonodes.begin(); titer != node->tonodes.end(); ++titer) {
				// ELSE
				if ((*titer)->getLabel() == "else" ) {
					(*titer)->isActivated = 1;
					newActivatedNodes.push_back((connode*)(*titer));
					break;
				}
			}
		}

		// call FI
		gennode *endifnode = (*std::find_if(node->tonodes.begin(), node->tonodes.end(), [](gennode* node) {
			return (node->getLabel() == "fi");
		}));

		endifnode->isActivated = 1;
		newActivatedNodes.push_back((connode*)(endifnode));
	}
}

void hjApp::updateRegister(gennode *node, vardata *data) {
	delete varRegister[node];
	varRegister[node] = data;
}

bool hjApp::getVardataFromNode(gennode *node, vardata *&data) {
	if (node->type == nodetype::CONNODE) {
		data = varRegister[node];
	}
	else {
		data = dynamic_cast<varnode*>(node)->getVardata();
		return data;
	}

	return true;
}

bool hjApp::getValueFromNode(gennode *node, int &value) {
	if (node->type == nodetype::CONNODE) { // get the value from the register
		intdata *tmpVardata = dynamic_cast<intdata*>(varRegister[node]);

		if (tmpVardata) {
			value = tmpVardata->value;
		} else {
			hjErrors::reportError(node, hjErrors::CannotConvertConnodeToType);
			return false;
		}
	}
	else { // some varnode, just get the value
		if (node->type == nodetype::INTNODE) {
			value = ((intnode*)node)->getVardata()->value;
			node->isActivated = 1;
		}
		else if (node->type == nodetype::BOOLNODE) {
			value = ((boolnode*)node)->getVardata()->value ? 1 : 0;
			node->isActivated = 1;
		}
		else {
			hjErrors::reportError(node, hjErrors::CannotConvertVarnodeToType);
			return false;
		}
	}

	return true;
}

bool hjApp::getValueFromNode(gennode *node, std::string &value) {
	if (node->type == nodetype::CONNODE) { // get the value from the register
		strdata *tmpVardata = dynamic_cast<strdata*>(varRegister[node]);

		if (tmpVardata) {
			value = tmpVardata->value;
		}
		else {
			hjErrors::reportError(node, hjErrors::CannotConvertConnodeToType);
			return false;
		}
	}
	else { // some varnode, just get the value
		if (node->type == nodetype::STRNODE) {
			value = ((strnode*)node)->getVardata()->value;
			node->isActivated = 1;
		}
		else {
			hjErrors::reportError(node, hjErrors::CannotConvertVarnodeToType);
			return false;
		}
	}

	return true;
}

bool hjApp::getValueFromNode(gennode *node, bool &value) {
	if (node->type == nodetype::CONNODE) { // get the value from the register
		booldata *tmpVardata = dynamic_cast<booldata*>(varRegister[node]);

		if (tmpVardata) {
			value = tmpVardata->value;
		}
		else {
			hjErrors::reportError(node, hjErrors::CannotConvertConnodeToType);
			return false;
		}
	}
	else { // some varnode, just get the value
		if (node->type == nodetype::BOOLNODE) {
			value = ((boolnode*)node)->getVardata()->value;
			node->isActivated = 1;
		}
		else if (node->type == nodetype::INTNODE) {
			value = ((intnode*)node)->getVardata()->value;
			node->isActivated = 1;
		}
		else {
			hjErrors::reportError(node, hjErrors::CannotConvertVarnodeToType);
			return false;
		}
	}

	return true;
}

bool hjApp::setNodeValue(gennode *node, const int &value) {
	if (node->type == nodetype::INTNODE) {
		((intnode*)node)->setVardata(intdata(value));
	}
	else if (node->type == nodetype::BOOLNODE) {
		((boolnode*)node)->setVardata(booldata(value));
	}
	else {
		return false;
	}

	node->isActivated = 1;
	return true;
}

bool hjApp::setNodeValue(gennode *node, const std::string &value) {
	if (node->type == nodetype::STRNODE) {
		((strnode*)node)->setVardata(strdata(value));
	}
	else {
		return false;
	}

	node->isActivated = 1;
	return true;
}

bool hjApp::setNodeValue(gennode *node, const bool &value) {
	if (node->type == nodetype::BOOLNODE) {
		((boolnode*)node)->setVardata(booldata(value));
	}
	else {
		return false;
	}

	node->isActivated = 1;
	return true;
}