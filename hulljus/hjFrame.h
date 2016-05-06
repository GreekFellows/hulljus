#pragma once

#include "wx/wx.h"
#include "hjDrawPanel.h"

class hjFrame : public wxFrame {
public:

	hjFrame(const wxString&, const wxPoint&, const wxSize&);

	wxTextCtrl *console;
	hjDrawPanel *drawpanel;

	wxMenuBar *menubar;
	wxMenu *fileMenu, *connodeMenu, *varnodeMenu, *controlsMenu, *aboutMenu;

	wxBoxSizer *sizer;

	void OnFileNew(wxCommandEvent&);
	void OnFileOpen(wxCommandEvent&);
	void OnFileSave(wxCommandEvent&);
	void OnFileSaveAs(wxCommandEvent&);
	void OnConnodeAdd(wxCommandEvent&);
	void OnVarnodeAddIntnode(wxCommandEvent&);
	void OnVarnodeAddStrnode(wxCommandEvent&);
	void OnVarnodeAddBoolnode(wxCommandEvent&);
	void OnControlsClearConsole(wxCommandEvent&);
	void OnAbout(wxCommandEvent&);

};

enum {
	ID_File_New = 1,
	ID_File_Open = 2,
	ID_File_Save = 3,
	ID_File_SaveAs = 4,

	ID_Connode_Add = 5,

	ID_Varnode_AddIntnode = 6,
	ID_Varnode_AddStrnode = 7,
	ID_Varnode_AddBoolnode = 8,

	ID_Controls_ClearConsole = 9,

	ID_About = 10
};