#include "hjFrame.h"
#include "hjApp.h"

wxDECLARE_APP(hjApp);

hjFrame::hjFrame(
	const wxString &title,
	const wxPoint &position,
	const wxSize &size
	) : wxFrame(NULL, wxID_ANY, title, position, size) {
	fileMenu = new wxMenu();
	fileMenu->Append(ID_File_New, "new");
	fileMenu->Append(ID_File_Open, "open...");
	fileMenu->Append(ID_File_Save, "save");
	fileMenu->Append(ID_File_SaveAs, "save as...");

	connodeMenu = new wxMenu();
	connodeMenu->Append(ID_Connode_Add, "add");

	varnodeMenu = new wxMenu();
	varnodeMenu->Append(ID_Varnode_AddIntnode, "add intnode");
	varnodeMenu->Append(ID_Varnode_AddStrnode, "add strnode");
	varnodeMenu->Append(ID_Varnode_AddBoolnode, "add boolnode");

	controlsMenu = new wxMenu();
	controlsMenu->Append(ID_Controls_ClearConsole, "clear console");

	aboutMenu = new wxMenu();
	aboutMenu->Append(ID_About, "m9('„D')");

	menubar = new wxMenuBar();
	menubar->Append(fileMenu, "&file");
	menubar->Append(connodeMenu, "&connodes");
	menubar->Append(varnodeMenu, "&varnodes");
	menubar->Append(controlsMenu, "con&trols");
	menubar->Append(aboutMenu, "&about");

	sizer = new wxBoxSizer(wxVERTICAL);

	console = new wxTextCtrl(this, -1, wxEmptyString, wxDefaultPosition, wxDefaultSize, wxTE_MULTILINE);
	console->SetCanFocus(false);

	drawpanel = new hjDrawPanel((wxFrame*)this);
	drawpanel->SetFocus();

	sizer->Add(drawpanel, 1, wxEXPAND);
	sizer->Add(console, 0.8, wxEXPAND);

	SetSizer(sizer);
	SetAutoLayout(true);

	SetMenuBar(menubar);

	Bind(wxEVT_MENU, &hjFrame::OnFileNew, this, ID_File_New);
	Bind(wxEVT_MENU, &hjFrame::OnFileOpen, this, ID_File_Open);
	Bind(wxEVT_MENU, &hjFrame::OnFileSave, this, ID_File_Save);
	Bind(wxEVT_MENU, &hjFrame::OnFileSaveAs, this, ID_File_SaveAs);
	Bind(wxEVT_MENU, &hjFrame::OnConnodeAdd, this, ID_Connode_Add);
	Bind(wxEVT_MENU, &hjFrame::OnVarnodeAddIntnode, this, ID_Varnode_AddIntnode);
	Bind(wxEVT_MENU, &hjFrame::OnVarnodeAddStrnode, this, ID_Varnode_AddStrnode);
	Bind(wxEVT_MENU, &hjFrame::OnVarnodeAddBoolnode, this, ID_Varnode_AddBoolnode);
	Bind(wxEVT_MENU, &hjFrame::OnControlsClearConsole, this, ID_Controls_ClearConsole);
	Bind(wxEVT_MENU, &hjFrame::OnAbout, this, ID_About);
}

void hjFrame::OnFileNew(wxCommandEvent&) {
	wxGetApp().startFromScratch();
}

void hjFrame::OnFileOpen(wxCommandEvent&) {
	wxString filepath = wxFileSelector(
		"('ƒÖ') open a .hjs file please",
		wxEmptyString,
		wxEmptyString,
		wxEmptyString,
		"hulljus savefiles (*.hjs)|*.hjs|or whatever format|*.*",
		wxFD_OPEN
		);
	if (!filepath.empty()) {
		wxGetApp().openFile(filepath.ToStdString());
	}
}

void hjFrame::OnFileSave(wxCommandEvent&) {
	if (wxGetApp().writepath.empty()) {
		wxString filepath = wxFileSelector(
			"('x') save to some .hjs file",
			wxEmptyString,
			wxEmptyString,
			wxEmptyString,
			"hulljus savefiles (*.hjs)|*.hjs",
			wxFD_SAVE
			);
		if (!filepath.empty()) {
			wxGetApp().saveFile(filepath.ToStdString());
		}
	}
	else {
		wxGetApp().saveFile();
	}
}

void hjFrame::OnFileSaveAs(wxCommandEvent&) {
	wxString filepath = wxFileSelector(
		"('x') save to some .hjs file",
		wxEmptyString,
		wxEmptyString,
		wxEmptyString,
		"hulljus savefiles (*.hjs)|*.hjs",
		wxFD_SAVE | wxFD_OVERWRITE_PROMPT
		);
	if (!filepath.empty()) {
		wxGetApp().saveFile(filepath.ToStdString());
	}
}

void hjFrame::OnConnodeAdd(wxCommandEvent&) {
	connode *newconnode = new connode(100 + rand() % 300, 100 + rand() % 200, "");
	wxGetApp().gennodes.emplace_back(newconnode);
}

void hjFrame::OnVarnodeAddIntnode(wxCommandEvent&) {
	intnode *newintnode = new intnode(100 + rand() % 300, 100 + rand() % 200, 0);
	wxGetApp().gennodes.emplace_back(newintnode);
}

void hjFrame::OnVarnodeAddStrnode(wxCommandEvent&) {
	strnode *newstrnode = new strnode(100 + rand() % 300, 100 + rand() % 200, "");
	wxGetApp().gennodes.emplace_back(newstrnode);
}

void hjFrame::OnVarnodeAddBoolnode(wxCommandEvent&) {
	boolnode *newboolnode = new boolnode(100 + rand() % 300, 100 + rand() % 200, true);
	wxGetApp().gennodes.emplace_back(newboolnode);
}

void hjFrame::OnControlsClearConsole(wxCommandEvent&) {
	wxGetApp().frame->console->Clear();
}

void hjFrame::OnAbout(wxCommandEvent&) {
	wxAboutBox(wxGetApp().aboutInfo, this);
}