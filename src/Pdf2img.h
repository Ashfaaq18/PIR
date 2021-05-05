#ifndef PDF2IMG_H
#define PDF2IMG_H

#include <winsock2.h>
#include "fpdfview.h"

#include <wx/wxprec.h>
#ifndef WX_PRECOMP
    #include <wx/wx.h>
#endif
#include <wx/rawbmp.h>
#include <vector>

void pdfium_init();
void pdfium_close();

class PDF
{
	public:
		PDF();
		
		wxBitmap CreateBitmap();
		wxBitmap refreshBitmap(double zoom);
		void LoadPDF(char doc_name[]);
		void ClosePDF();
		bool isloaded()
		{
			return loaded;
		}
		
		double zoomMax;
		double zoomMin;
		double zoomStep;
	private:
		FPDF_DOCUMENT doc;
		FPDF_PAGE page;
		double width;
		double height;
		bool loaded;
		std::vector<FPDF_BITMAP> bitmapVec;
};

#endif