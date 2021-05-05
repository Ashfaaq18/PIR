#include "ImagePanel.h"
#include <wx/dcbuffer.h>

wxBEGIN_EVENT_TABLE(wxImageScrollWindow, wxScrolledWindow)
   EVT_SCROLLWIN_THUMBTRACK(wxImageScrollWindow::scrollActions)
wxEND_EVENT_TABLE()



wxImageScrollWindow::wxImageScrollWindow(wxWindow* parent) :
wxScrolledWindow(parent)
{
    valid_Page = false;
	zoom = 1.0f;
	scrollSpeed = 50;
	posX = 0;
	posY = 0;
	scrollPosX = 0;
	scrollPosY = 0;
	
	controlKey = false;
	shiftKey = false;
	middleDoubleClick = false;
	middleClick = false;
	selectionMode = false;
	
	PanStartPoint = wxPoint(0,0);
	inProgressPanVector = wxPoint(0,0);
	
	
	SetDoubleBuffered(true);
	//SetBackgroundColour(wxColour(192, 192, 192));
	SetBackgroundStyle(wxBG_STYLE_PAINT);
	
	Bind(wxEVT_PAINT,&wxImageScrollWindow::paintEvent,this);	
    Bind(wxEVT_KEY_DOWN,&wxImageScrollWindow::keyDown,this);  
    Bind(wxEVT_MIDDLE_DOWN,&wxImageScrollWindow::mouseWheelMiddleDown,this);   
	Bind(wxEVT_MOUSEWHEEL,&wxImageScrollWindow::mouseWheelMoved,this);
	Bind(wxEVT_LEFT_DOWN,&wxImageScrollWindow::mouseLeftDown,this);
	
	//---------------------load pdf------------------------------//
	pdfium_init(); 	//start pdfium
	char s[] = "files/WSP-D-EL-0-001[T1].pdf";	//load file
	pdf.LoadPDF(s);
	valid_Page =  pdf.isloaded();
	
	if(pdf.isloaded())
	{
		image = pdf.CreateBitmap();
		this->SetVirtualSize(image.GetWidth(),image.GetHeight());
		//drawPane->SetScrollbars	(1,1,image.GetWidth(),image.GetHeight(),0,0);
	}
	else
	{
		std::cout<<"failed to load pdf"<<std::endl;
		pdf.ClosePDF();
	}
}



void wxImageScrollWindow::paintEvent(wxPaintEvent & evt)
{
	if(valid_Page)
	{
		wxBufferedPaintDC dc(this);
		dc.Clear();
		//dc.SetUserScale(zoom, zoom);
		render(dc);
	}
	else
	{
		wxLogError("Can't load the pdf");
	}
}

void wxImageScrollWindow::render(wxDC&  dc)
{
    dc.DrawBitmap( image, posX, posY, false );
	
	if(selectionMode)
	{
		dc.SetPen(*wxRED_PEN);
		dc.SetBrush(*wxTRANSPARENT_BRUSH);
		dc.DrawRectangle(selectionRect);
	}
}

void wxImageScrollWindow::mouseWheelMoved(wxMouseEvent& event)
{
	if(controlKey && panelFocus(event) ) //scroll zoom
	{
		wxPoint mousePos = event.GetPosition();
		double percentX, percentY;
		double offsetX, offsetY;
					
		if(event.GetWheelRotation() < 0) 	//zoom out
		{
			if( abs(zoom - pdf.zoomMin) > 1e-9 )
			{
				percentX = (mousePos.x-posX)/(image.GetWidth()*zoom);
				percentY = (mousePos.y-posY)/(image.GetHeight()*zoom);
		
				zoom -= pdf.zoomStep;
			
				offsetX = fabs(percentX) * (image.GetWidth()*zoom);
				offsetY = fabs(percentY) * (image.GetHeight()*zoom);
				
				posX = mousePos.x - offsetX;
				posY = mousePos.y - offsetY;
			}
			
		}
		else								//zoom in
		{
			if(zoom <= pdf.zoomMax)
			{

				percentX = (mousePos.x-posX)/(image.GetWidth()*zoom);
				percentY = (mousePos.y-posY)/(image.GetHeight()*zoom);
				
				zoom += pdf.zoomStep;
			
				offsetX = percentX * (image.GetWidth()*zoom);
				offsetY = percentY * (image.GetHeight()*zoom);
				
				posX = mousePos.x - offsetX;
				posY = mousePos.y - offsetY;
				
				
			}

		}
		
		scrollPosX = 0;
		scrollPosY = 0;
		this->Scroll(wxPoint(scrollPosX, scrollPosY));
		image = pdf.refreshBitmap(zoom);
		this->SetVirtualSize(image.GetWidth(),image.GetHeight());
		Refresh();
	}
	else //	normal scrolling
	{
		/*int x,y;
		GetScrollPixelsPerUnit(&x,&y);

		double XLimit = ceil((image.GetWidth()*zoom-this->GetClientSize().x)/double(x));
		double YLimit = ceil((image.GetHeight()*zoom-this->GetClientSize().y)/double(y));
		
		if(!shiftKey && event.GetWheelRotation() < 0 ) 	//scroll down
		{
			scrollPosY += scrollSpeed;
			if(scrollPosY > YLimit)
				scrollPosY = YLimit;
		}
		else if(!shiftKey && scrollPosY >0)				//scroll up
		{
				scrollPosY -= scrollSpeed;
				if(scrollPosY < 0)
					scrollPosY = 0;
		}
		else if(shiftKey && event.GetWheelRotation() < 0 ) //scroll left
		{
			scrollPosX -= scrollSpeed;
			if(scrollPosX < 0)
				scrollPosX = 0;
		}
		else if(shiftKey)	//scroll right
		{
			scrollPosX += scrollSpeed;
			if(scrollPosX > XLimit)
				scrollPosX = XLimit;
		}
		
		this->Scroll(wxPoint(scrollPosX, scrollPosY));
		posX = -scrollPosX*(1/zoom)*x;
		posY = -scrollPosY*(1/zoom)*y;*/
	}
}


void wxImageScrollWindow::ProcessPan(const wxPoint& pt, bool refresh)
{
    inProgressPanVector = PanStartPoint - pt;
	
	//std::cout<<"XY: "<<inProgressPanVector.x<<", "<<inProgressPanVector.y<<std::endl;
	
	PanStartPoint = pt;
	posX -= inProgressPanVector.x;
	posY -= inProgressPanVector.y;
	
    if ( refresh )
    {
        Refresh();
    }
}

bool wxImageScrollWindow::panelFocus(wxMouseEvent& event)
{
	if(event.GetPosition().y < this->GetSize().y
	&& event.GetPosition().y > 0
	&& event.GetPosition().x > 0
	&& event.GetPosition().x < this->GetSize().x
	)
	{
		return true;
	}
	else
	{
		return false;
	}
}

void wxImageScrollWindow::OnMotion(wxMouseEvent& event)
{
	if(panelFocus(event))
	{
		if(!selectionMode)
		{
			ProcessPan(event.GetPosition(), true);
		}
		else
		{
			wxPoint point1 = event.GetPosition();
			point1.x = event.GetPosition().x;
			point1.y = event.GetPosition().y;
			
			selectionRect = wxRect(selectionRect.GetTopLeft(), point1);
			Refresh();
		}
		//std::cout<<event.GetPosition().y<<", "<<this->GetSize().y<<std::endl;
	}
	else 
	{
		std::cout<<"out"<<std::endl;
		if(selectionMode)
		{
			mouseLeftUp(event);
			std::cout<<"dsf"<<std::endl;
		}
	}

}

void wxImageScrollWindow::mouseWheelMiddleDown(wxMouseEvent& event)
{
	if(panelFocus(event))
	{
		if(!middleDoubleClick && !selectionMode)
		{
			wxCursor cursor(wxCURSOR_HAND);
			SetCursor(cursor);	
			
			PanStartPoint = event.GetPosition();
			//std::cout<<PanStartPoint.x<<","<<PanStartPoint.y<<" | "<<image.GetWidth()*zoom<<","<<image.GetHeight()*zoom<<std::endl;
			
			Bind(wxEVT_MIDDLE_UP,&wxImageScrollWindow::mouseWheelMiddleUp,this);
			Bind(wxEVT_MIDDLE_DCLICK,&wxImageScrollWindow::mouseWheelMiddleDclick,this);
			Bind(wxEVT_MOTION,&wxImageScrollWindow::OnMotion,this);
			
			CaptureMouse();
			middleClick = true;
			std::cout<<"middleDown"<<std::endl;
		}
	}

}

void wxImageScrollWindow::mouseWheelMiddleUp(wxMouseEvent& event)
{
	if(!middleDoubleClick && middleClick && !selectionMode)
	{
		SetCursor(wxNullCursor);
		Unbind(wxEVT_MOTION,&wxImageScrollWindow::OnMotion,this);
		ReleaseMouse();
		middleClick = false;
		std::cout<<"middleUp"<<std::endl;
	}
	else if(middleDoubleClick)
	{
		middleDoubleClick = false;
	}
}

void wxImageScrollWindow::mouseWheelMiddleDclick(wxMouseEvent& event)
{
	posX = 0;
	posY = 0;
	zoom = 1;
	image = pdf.refreshBitmap(zoom);
	this->SetVirtualSize(image.GetWidth(),image.GetHeight());
	Refresh();
	middleDoubleClick = true;
	Unbind(wxEVT_MIDDLE_DCLICK,&wxImageScrollWindow::mouseWheelMiddleDclick,this);
	std::cout<<this->GetSize().x<<" | "<<this->GetSize().y<<std::endl;
}

void wxImageScrollWindow::scrollActions(wxScrollWinEvent& event)
{
	//std::cout<<event.GetPosition() << "|" << event.GetOrientation() << " | "<<this->GetSize().x<<" | "<<this->GetSize().y <<std::endl;
	if(event.GetOrientation() == wxHORIZONTAL)
	{
		scrollPosX = event.GetPosition();
	}
	else if(event.GetOrientation() == wxVERTICAL)
	{
		scrollPosY = event.GetPosition();
	}
	this->Scroll(wxPoint(scrollPosX, scrollPosY));
	int x,y;
	GetScrollPixelsPerUnit(&x,&y);
	posX = -scrollPosX*(1/zoom)*x;
	posY = -scrollPosY*(1/zoom)*y;
	Refresh();

}

void wxImageScrollWindow::keyDown(wxKeyEvent& event)
{
	if(event.GetKeyCode() == WXK_CONTROL)
	{
		if(!controlKey)
			controlKey = true;
	}
	
	if(event.GetKeyCode() == WXK_SHIFT)
	{
		if(!shiftKey)
			shiftKey = true;
	}
	 
	
	Bind(wxEVT_KEY_UP,&wxImageScrollWindow::keyUp,this);
}

void wxImageScrollWindow::keyUp(wxKeyEvent& event)
{
	if(event.GetKeyCode() == WXK_CONTROL && controlKey)
	{
		controlKey = false;
	}
	
	if(event.GetKeyCode() == WXK_SHIFT && shiftKey)
	{
		shiftKey = false;
	}
}

void wxImageScrollWindow::mouseLeftDown(wxMouseEvent& event)
{
	if(panelFocus(event))
	{
		if(!middleClick)
		{
			selectionMode = true;
			wxPoint selectionPoint;

			selectionPoint.x = event.GetPosition().x;
			selectionPoint.y = event.GetPosition().y;
			
			selectionRect = wxRect(selectionPoint, wxSize(0, 0));

			Bind(wxEVT_LEFT_UP,&wxImageScrollWindow::mouseLeftUp,this);
			Bind(wxEVT_MOTION,&wxImageScrollWindow::OnMotion,this);
			
			CaptureMouse();
		}
	}
}

void wxImageScrollWindow::mouseLeftUp(wxMouseEvent& event)
{
	if(!middleClick)
	{
		selectionMode = false;
		selectionRect = wxRect ();
		Refresh();
		
		Unbind(wxEVT_LEFT_UP,&wxImageScrollWindow::mouseLeftUp,this);
		Unbind(wxEVT_MOTION,&wxImageScrollWindow::OnMotion,this);
		
		ReleaseMouse();
		std::cout<<"release"<<std::endl;
	}
}