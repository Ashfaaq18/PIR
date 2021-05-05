#ifndef FRAME_H
#define FRAME_H

#include "ImagePanel.h"
#include <wx/aui/aui.h>

class MyFrame : public wxFrame
{
	public:
		MyFrame();
	private:
		void OnHello(wxCommandEvent& event);
		void OnExit(wxCommandEvent& event);
		void OnAbout(wxCommandEvent& event);
		void text(wxCommandEvent& event);
		
		wxMenu *menuFile;
		wxMenu *menuHelp;
		wxMenuBar *menuBar;
		
		wxImageScrollWindow *drawPane;

		wxBoxSizer* imgPanelSizer;
		wxBoxSizer* imgSwitchButtonSizer;
		
		wxButton *PrevBtn;
		wxButton *NextBtn;
		
		wxTextCtrl* m_textCtrl3;
		wxTextCtrl* m_textCtrl4;
		
		wxToolBar* toolBar1;
		wxBitmapButton* bpButton1;
		//wxBitmap* templateSelect;
		
		wxAuiManager m_mgr;
};

enum
{
    ID_Hello = 1
};

#endif