// wxWidgets "Hello World" Program
// For compilers that support precompilation, includes "wx/wx.h".

#include "Frame.h"

class MyApp : public wxApp
{
public:
    virtual bool OnInit();
};

bool MyApp::OnInit()
{
	wxInitAllImageHandlers();
    MyFrame *frame = new MyFrame();
	SetTopWindow(frame);
    frame->Show(true);
    return true;
}

wxIMPLEMENT_APP(MyApp);