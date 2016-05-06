#include "hjDrawPanel.h"
#include "hjApp.h"

wxDECLARE_APP(hjApp);

hjDrawPanel::hjDrawPanel(wxWindow *parent) : wxScrolled<wxWindow>(parent, wxID_ANY) {
	SetScrollRate(10, 10);
	SetVirtualSize(*wxGetApp().virtualSize);

	Bind(wxEVT_PAINT, &hjDrawPanel::paintEvent, this, wxID_ANY);
	Bind(wxEVT_LEFT_DOWN, &hjDrawPanel::OnLeftDown, this, wxID_ANY);
	Bind(wxEVT_LEFT_UP, &hjDrawPanel::OnLeftUp, this, wxID_ANY);
	Bind(wxEVT_MOTION, &hjDrawPanel::OnMotion, this, wxID_ANY);
	Bind(wxEVT_LEFT_DCLICK, &hjDrawPanel::OnLeftDoubleClick, this, wxID_ANY);
	Bind(wxEVT_RIGHT_DOWN, &hjDrawPanel::OnRightDown, this, wxID_ANY);
	Bind(wxEVT_RIGHT_UP, &hjDrawPanel::OnRightUp, this, wxID_ANY);
	Bind(wxEVT_ERASE_BACKGROUND, &hjDrawPanel::OnEraseBackground, this, wxID_ANY);
	Bind(wxEVT_RIGHT_DCLICK, &hjDrawPanel::OnRightDoubleClick, this, wxID_ANY);
}

void hjDrawPanel::OnEraseBackground(wxEraseEvent&) {

}

void hjDrawPanel::paintEvent(wxPaintEvent&) {
	// wxPaintDC dc(this);
	wxBufferedPaintDC dc(this);
	PrepareDC(dc);
	render(dc);
}

void hjDrawPanel::paintNow() {
	Refresh();
}

void hjDrawPanel::render(wxDC &dc) {
	dc.Clear();

	auto &gennodes = wxGetApp().gennodes;
	auto &selection = wxGetApp().selection;
	auto &edited = wxGetApp().edited;
	auto &activatedNodes = wxGetApp().activatedNodes;

	// draw nodes
	for (auto iter = gennodes.begin(); iter != gennodes.end();  ++iter) {
		// calculate rectangles beforehand for later convenience
		wxRect rect = (*iter)->calcRect();
		wxRect midRect = (*iter)->calcMidRect();

		// if the node is dragged, draw a white rectangle underneath
		if (selection == *iter) {
			dc.SetPen(*wxBLACK_DASHED_PEN);
			dc.SetBrush(*wxWHITE_BRUSH);
			dc.DrawRectangle(rect.x - 5, rect.y - 5, rect.width + 10, rect.height + 10);
		}

		// nodetype-specific styles
		if ((*iter)->type == nodetype::CONNODE) { // black -> blue
			dc.SetPen(wxPen(wxColor(0, 0, (*iter)->isActivated * 255)));
			dc.SetBrush(wxBrush(wxColor(0, 0, (*iter)->isActivated * 255)));
		}
		else if ((*iter)->type == nodetype::INTNODE) { // grey -> yellow
			dc.SetPen(wxPen(wxColor(128 + (*iter)->isActivated * 127, 128 + (*iter)->isActivated * 127, 128)));
			dc.SetBrush(wxBrush(wxColor(128 + (*iter)->isActivated * 127, 128 + (*iter)->isActivated * 127, 128)));
		}
		else if ((*iter)->type == nodetype::STRNODE) { // dark green -> yellow
			dc.SetPen(wxPen(wxColor((*iter)->isActivated * 255, 128 + (*iter)->isActivated * 127, 0)));
			dc.SetBrush(wxBrush(wxColor((*iter)->isActivated * 255, 128 + (*iter)->isActivated * 127, 0)));
		}
		else if ((*iter)->type == nodetype::BOOLNODE) { // dark red --> yellow
			dc.SetPen(wxPen(wxColor(128 + (*iter)->isActivated * 127, (*iter)->isActivated * 255, 0)));
			dc.SetBrush(wxBrush(wxColor(128 + (*iter)->isActivated * 127, (*iter)->isActivated * 255, 0)));
		}

		// decrement the activation tick
		if ((*iter)->isActivated > 0) {
			(*iter)->isActivated -= 0.01;
		}
		else {
			(*iter)->isActivated = 0;
		}

		dc.DrawRectangle(rect);

		// white brush for mid rect
		dc.SetBrush(*wxWHITE_BRUSH);
		dc.DrawRectangle(midRect);

		// black text foreground color for label
		dc.SetTextForeground(*wxBLACK);
		dc.SetTextBackground(wxNullColour);
		dc.DrawText(wxString((*iter)->getLabel()), midRect.x + wxGetApp().labelHorizontalMargin, midRect.y + wxGetApp().labelVerticalMargin);

		// black pen to from-to connect connodes, grey brush for from-to connection slots
		for (auto titer = (*iter)->tonodes.begin(); titer != (*iter)->tonodes.end(); ++titer) {
			wxRect trect = (*titer)->calcRect();

			wxPoint fromHub = wxPoint(rect.x, rect.y + rect.height - 1);
			wxPoint toHub = wxPoint(trect.x, trect.y);
			int squaredDistance = pow(fromHub.x - toHub.x, 2) + pow(fromHub.y - toHub.y, 2);

			// if the distance is too little, hub distance is ignored
			if (squaredDistance > pow(wxGetApp().hubDistance * 2, 2)) {
				fromHub.y += wxGetApp().hubDistance;
				toHub.y -= wxGetApp().hubDistance;
			}

			dc.SetPen(*wxBLACK_PEN);
			dc.DrawLine(fromHub, toHub);

			dc.SetBrush(*wxGREY_BRUSH);
			dc.DrawCircle(fromHub, 2);
			dc.DrawCircle(toHub, 2);
		}

		// black pen to in-out connect connodes, yellow brush for in-out connection slots
		int numOutnodes = (*iter)->outnodes.size() - 1;
		if (numOutnodes == 0) numOutnodes = 1;

		int currentOutnodeIndex = 0;

		for (auto oiter = (*iter)->outnodes.begin(); oiter != (*iter)->outnodes.end(); ++oiter) {
			wxRect omidRect = (*oiter)->calcMidRect();

			int numInnodes = (*oiter)->innodes.size() - 1;
			if (numInnodes == 0) numInnodes = 1;

			int currentInnodeIndex = std::distance((*oiter)->innodes.begin(), std::find((*oiter)->innodes.begin(), (*oiter)->innodes.end(), *iter));

			wxPoint inHub = wxPoint(midRect.x + midRect.width, midRect.y + midRect.height * currentOutnodeIndex / numOutnodes);
			wxPoint outHub = wxPoint(omidRect.x, omidRect.y + omidRect.height * currentInnodeIndex / numInnodes);
			int squaredDistance = pow(inHub.x - outHub.x, 2) + pow(inHub.y - outHub.y, 2);

			if (squaredDistance > pow(wxGetApp().hubDistance * 2, 2)) {
				inHub.x += wxGetApp().hubDistance;
				outHub.x -= wxGetApp().hubDistance;
			}

			dc.SetPen(*wxBLACK_PEN);
			dc.DrawLine(inHub, outHub);

			dc.SetBrush(*wxYELLOW_BRUSH);
			dc.DrawCircle(inHub, 2);
			dc.DrawCircle(outHub, 2);

			++currentOutnodeIndex;
		}
	}
}

// a press on the LMB initiates a drag
void hjDrawPanel::OnLeftDown(wxMouseEvent &event) {
	// define a few references to wxGetApp() members, for convenience
	auto &gennodes = wxGetApp().gennodes;
	auto &selection = wxGetApp().selection;
	auto &dragOffset = wxGetApp().dragOffset;

	dragOffset = wxPoint(0, 0);

	selection = NULL;
	
	// was any node clicked?
	for (auto iter = gennodes.begin(); iter != gennodes.end(); ++iter) {
		if ((*iter)->calcRect().Contains(event.GetX(), event.GetY())) {
			selection = *iter;
			dragOffset = wxPoint((*iter)->x - event.GetX(), (*iter)->y - event.GetY());
		}
	}

	// if some node was clicked, then we will bring her forwardmost
	if (selection) {
		gennodes.erase(std::find(gennodes.begin(), gennodes.end(), selection));
		gennodes.emplace_back(selection);
	}
}

void hjDrawPanel::OnLeftUp(wxMouseEvent &event) {
	auto &selection = wxGetApp().selection;

	selection = NULL;
}

// a drag will drag the nodes if they are in drag
void hjDrawPanel::OnMotion(wxMouseEvent &event) {
	auto &selection = wxGetApp().selection;
	auto &dragOffset = wxGetApp().dragOffset;

	if (wxGetMouseState().LeftIsDown()) {
		if (selection) {
			selection->x = dragOffset.x + event.GetX();
			selection->y = dragOffset.y + event.GetY();
		}
	}
}

// double clicking a node activates it
void hjDrawPanel::OnLeftDoubleClick(wxMouseEvent &event) {
	auto &gennodes = wxGetApp().gennodes;
	auto &activatedNodes = wxGetApp().activatedNodes;
	connode *clickedNode = NULL;

	// find the forwardmost node clicked
	for (auto iter = gennodes.begin(); iter != gennodes.end(); ++iter) {
		// we are only interested in activating connodes
		if ((*iter)->type != nodetype::CONNODE) {
			continue;
		}

		if ((*iter)->calcRect().Contains(event.GetX(), event.GetY())) {
			clickedNode = (connode*)(*iter);
		}
	}

	// if some node was double clicked
	if (clickedNode) {
		clickedNode->isActivated = 1;
		activatedNodes.push_back(clickedNode);
	}
}

// a press of the RMD starts a connection / varnection
void hjDrawPanel::OnRightDown(wxMouseEvent &event) {
	auto &gennodes = wxGetApp().gennodes;
	auto &startConnect = wxGetApp().startConnect;

	startConnect = NULL;

	for (auto iter = gennodes.begin(); iter != gennodes.end(); ++iter) {
		if ((*iter)->calcRect().Contains(event.GetX(), event.GetY())) {
			startConnect = *iter;
		}
	}
}

// a release of the RMD ends a connection / varnection
void hjDrawPanel::OnRightUp(wxMouseEvent &event) {
	auto &gennodes = wxGetApp().gennodes;
	auto &startConnect = wxGetApp().startConnect;

	gennode* endConnect = NULL;

	for (auto iter = gennodes.begin(); iter != gennodes.end(); ++iter) {
		if ((*iter)->calcRect().Contains(event.GetX(), event.GetY())) {
			endConnect = *iter;
		}
	}

	// let's connect
	if (startConnect && endConnect && (startConnect != endConnect)) {
		if (startConnect->type == nodetype::CONNODE && endConnect->type == nodetype::CONNODE) { // if both are connodes
			if (wxGetKeyState(wxKeyCode::WXK_CONTROL)) { // if ctrl is pressed, make this a varnection
				startConnect->addOutnode(endConnect);
				endConnect->addInnode(startConnect);
			}
			else { // otherwise this is a connection
				startConnect->addTonode(endConnect);
				endConnect->addFromnode(startConnect);
			}
		}
		else if (startConnect->type == nodetype::CONNODE || endConnect->type == nodetype::CONNODE) { // connode-varnode
			startConnect->addOutnode(endConnect);
			endConnect->addInnode(startConnect);
		}
	}

	startConnect = NULL;
}

// double clicking a node with the RMB enables edit
void hjDrawPanel::OnRightDoubleClick(wxMouseEvent &event) {
	auto &gennodes = wxGetApp().gennodes;
	auto &edited = wxGetApp().edited;

	edited = NULL;

	for (auto iter = gennodes.begin(); iter != gennodes.end(); ++iter) {
		if ((*iter)->calcRect().Contains(event.GetX(), event.GetY())) {
			edited = *iter;
		}
	}

	// if some node was right double-clicked
	if (edited) {
		wxString newLabel = wxGetTextFromUser("change the label (value) to:", "~( '-' )~", edited->getLabel());
		if (edited->type == nodetype::CONNODE) { // a connode can have a blank label
			edited->setLabel(newLabel.ToStdString());
		}
		else if (edited->type == nodetype::STRNODE) { // types that allow a blank label
			((strnode*)edited)->setFromStringIfValid(newLabel.ToStdString());
		}
		else { // types that do not allow a blank label
			if (newLabel.IsEmpty()) { // the user probably cancelled, or set a blank new label, which we don't allow
				edited = NULL;
				return;
			}

			if (edited->type == nodetype::INTNODE) {
				while (!((intnode*)edited)->setFromStringIfValid(newLabel.ToStdString())) {
					newLabel = wxGetTextFromUser("to (something fancier please): ", "change the label", edited->getLabel());
				}
			}
			else if (edited->type == nodetype::BOOLNODE) {
				while (!((boolnode*)edited)->setFromStringIfValid(newLabel.ToStdString())) {
					newLabel = wxGetTextFromUser("to (something fancier please): ", "change the label", edited->getLabel());
				}
			}
		}
	}

	edited = NULL;
}