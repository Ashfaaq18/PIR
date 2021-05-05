#include "Frame.h"
#include "Pdf2img.h"
#include <stdlib.h>

MyFrame::MyFrame()
    : wxFrame(NULL, wxID_ANY, "Hello World")
{
	SetClientSize(800,600);
    
	menuFile = new wxMenu;
    menuFile->Append(ID_Hello, "&Hello...\tCtrl-H",
                     "Help string shown in status bar for this menu item");
    menuFile->AppendSeparator();
    menuFile->Append(wxID_EXIT);
    
	menuHelp = new wxMenu;
    menuHelp->Append(wxID_ABOUT);
    
	menuBar = new wxMenuBar;
    menuBar->Append(menuFile, "&File");
    menuBar->Append(menuHelp, "&Help");
	SetMenuBar( menuBar );
    
	CreateStatusBar();
    SetStatusText("Welcome to wxWidgets!");
    Bind(wxEVT_MENU, &MyFrame::OnHello, this, ID_Hello);
    Bind(wxEVT_MENU, &MyFrame::OnAbout, this, wxID_ABOUT);
    Bind(wxEVT_MENU, &MyFrame::OnExit, this, wxID_EXIT);

	imgPanelSizer = new wxBoxSizer( wxVERTICAL );
	
	//init the pdf drawing pane 
	drawPane = new wxImageScrollWindow(this); 
	
	drawPane->SetScrollRate( 10, 5 );
	
	imgPanelSizer->Add( drawPane, 1, wxEXPAND | wxALL, 5 );
	
	
	imgSwitchButtonSizer = new wxBoxSizer( wxHORIZONTAL );
	imgSwitchButtonSizer->Add( 0, 0, 1, 0, 5 );

	PrevBtn = new wxButton( this, wxID_ANY, wxT("<"), wxDefaultPosition, wxDefaultSize, 0 );
	imgSwitchButtonSizer->Add( PrevBtn, 0, wxALIGN_CENTER|wxALL, 5 );

	NextBtn = new wxButton( this, wxID_ANY, wxT(">"), wxDefaultPosition, wxDefaultSize, 0 );
	imgSwitchButtonSizer->Add( NextBtn, 0, wxALL, 5 );

	imgSwitchButtonSizer->Add( 0, 0, 1, wxEXPAND, 5 );
	imgPanelSizer->Add( imgSwitchButtonSizer, 0, wxEXPAND, 5 );

	
	this->SetSizer( imgPanelSizer );
	this->Layout();
	
	toolBar1 = this->CreateToolBar( wxTB_HORIZONTAL, wxID_ANY );
	wxBitmap templateSelect(wxT("files/Cursor-Select-icon.png"), wxBITMAP_TYPE_PNG);
	toolBar1->AddTool(wxID_ANY, wxT("Templ"), templateSelect);
	toolBar1->Realize();
	
	this->Centre( wxBOTH );
	std::cout<<drawPane->GetSize().x<<" | "<<drawPane->GetSize().y<<std::endl;
	
	/*m_mgr.SetManagedWindow(this);
	wxTextCtrl* text1 = new wxTextCtrl(this, -1);
	m_mgr.AddPane(text1, wxLEFT, wxT("Pane Number One"));
	m_mgr.Update();*/
	
}

void MyFrame::text(wxCommandEvent& event)
{
	/*double x = atof((m_textCtrl3->GetValue()).c_str());
	double y = atof((m_textCtrl4->GetValue()).c_str());
	drawPane->setPos( x, y);*/
}

void MyFrame::OnExit(wxCommandEvent& event)
{
	pdfium_close();
    Close(true);
}

void MyFrame::OnAbout(wxCommandEvent& event)
{
    wxMessageBox("This is a wxWidgets Hello World example",
                 "About Hello World", wxOK | wxICON_INFORMATION);
}

void MyFrame::OnHello(wxCommandEvent& event)
{
    wxLogMessage("Hello world from wxWidgets!");
}
