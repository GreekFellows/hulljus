#pragma once

#include <vector>
#include <set>
#include <map>
#include <fstream>
#include <sstream>

#include "wx/wx.h"
#include "wx/aboutdlg.h"

// for variable register
#include "boost/any.hpp"

#include "connode.h"
#include "vardata.h"
#include "intnode.h"
#include "strnode.h"
#include "boolnode.h"

#include "hjFrame.h"
#include "hjErrors.h"

#define hjLogNonStrConsole(a) wxGetApp().frame->console->AppendText(wxString(std::to_string(a)))
#define hjLogStrConsole(s) wxGetApp().frame->console->AppendText(wxString(s))
#define hjLogNonStrConsoleWithLine(a) wxGetApp().frame->console->AppendText(wxString(std::to_string(a)) + wxString("\n"))
#define hjLogStrConsoleWithLine(s) wxGetApp().frame->console->AppendText(wxString(s) + wxString("\n"))
#define hjClearConsole() wxGetApp().frame->console->Clear();

class hjApp : public wxApp {
public:

	virtual bool OnInit();
	void activateRenderLoop(bool);
	virtual void OnIdle(wxIdleEvent&);

	void startFromScratch();
	void openFile(const std::string&);
	void saveFile(const std::string& = "");

	// for modularization inside this class
	bool getVardataFromNode(gennode*, vardata*&);

	bool getValueFromNode(gennode*, int&);
	bool getValueFromNode(gennode*, std::string&);
	bool getValueFromNode(gennode*, bool&);

	bool setNodeValue(gennode*, const int&);
	bool setNodeValue(gennode*, const std::string&);
	bool setNodeValue(gennode*, const bool&);

	// actions of connodes
	void doPopmsg(gennode*);
	void doSum(gennode*);
	void doSubtract(gennode*);
	void doMultiply(gennode*);
	void doDivide(gennode*);
	void doEquality(gennode*);
	void doInequality(gennode*);
	void doGreaterThan(gennode*);
	void doLessThan(gennode*);
	void doGreaterThanOrEqualTo(gennode*);
	void doLessThanOrEqualTo(gennode*);
	void doNot(gennode*);
	void doAnd(gennode*);
	void doOr(gennode*);

	void doIf(gennode*);

	bool onRenderLoop;
	hjFrame *frame;

	wxSize *virtualSize;

	std::ifstream reader;
	std::ofstream writer;
	std::string writepath = "";

	std::vector<gennode*> gennodes;
	// std::vector<connode*> connodes;

	std::map<gennode*, vardata*> varRegister;
	void updateRegister(gennode*, vardata*);

	// for editing
	gennode *edited;

	// for dragging
	gennode *selection;
	wxPoint dragOffset;

	// for connections
	gennode *startConnect;

	std::vector<connode*> activatedNodes, newActivatedNodes;

	// styling
	double fromtoBandHeight = 5;
	double labelHorizontalMargin = 10;
	double labelVerticalMargin = 5;
	double hubDistance = 5;

	wxAboutDialogInfo aboutInfo;

};