#ifndef IMAGEPANEL_H
#define IMAGEPANEL_H

#include "templatePanel.h"

class wxImageScrollWindow : public wxScrolledWindow
{

	public:
		wxImageScrollWindow(wxWindow* parent);
		
		void paintEvent(wxPaintEvent & evt);
		void render(wxDC& dc);

	
	private:
		void mouseWheelMoved(wxMouseEvent& event);
		void scrollActions(wxScrollWinEvent& event);
		void keyDown(wxKeyEvent& event);
		void keyUp(wxKeyEvent& event);
		void mouseWheelMiddleDown(wxMouseEvent& event);
		void mouseWheelMiddleUp(wxMouseEvent& event);
		void mouseWheelMiddleDclick(wxMouseEvent& event);
		void mouseLeftDown(wxMouseEvent& event);
		void mouseLeftUp(wxMouseEvent& event);
		
		void OnMotion(wxMouseEvent& event);
		void ProcessPan(const wxPoint& pt, bool refresh);
		bool panelFocus(wxMouseEvent& event);
		
		wxBitmap image;
		bool valid_Page;
		double zoom;
		
		double posX;
		double posY;
		int scrollSpeed;
		int scrollPosX;
		int scrollPosY;
		
		bool controlKey;
		bool shiftKey;
		bool middleDoubleClick;
		bool middleClick;
		bool selectionMode;
		
		PDF pdf;
		wxRect selectionRect;
		wxPoint PanStartPoint;
		wxPoint inProgressPanVector;
		
	DECLARE_EVENT_TABLE()
};

#endif