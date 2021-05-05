#include "Pdf2img.h"
#include <iostream>

void pdfium_init()
{
	FPDF_LIBRARY_CONFIG config;
	config.version = 3;
  
	FPDF_InitLibraryWithConfig(&config);
}

void pdfium_close()
{
	FPDF_DestroyLibrary();
}

wxBitmap PDF::CreateBitmap()
{
	FPDF_BITMAP bitmap = FPDFBitmap_Create(width, height, 0);
	FPDFBitmap_FillRect(bitmap, 0, 0, width, height, 0xFFFFFFFF);
	FPDF_RenderPageBitmap(bitmap, page, 0, 0, width, height, 0, 0);

	//Convert BGRA image data from PDF SDK to RGB image data
	wxBitmap bmp(reinterpret_cast<const char *>(FPDFBitmap_GetBuffer(bitmap)), width,height, 32);
	
	FPDFBitmap_Destroy(bitmap);
	
	return bmp;
}

wxBitmap PDF::refreshBitmap(double zoom)
{
	FPDF_BITMAP bitmap = FPDFBitmap_Create(width*zoom, height*zoom, 0);
	FPDFBitmap_FillRect(bitmap, 0, 0, width*zoom, height*zoom, 0xFFFFFFFF);

	FPDF_RenderPageBitmap(bitmap, page, 0, 0, width*zoom, height*zoom, 0, 0);
	
	wxBitmap bmp(reinterpret_cast<const char *>(FPDFBitmap_GetBuffer(bitmap)), width*zoom,height*zoom, 32);
	
	FPDFBitmap_Destroy(bitmap);
	
	return bmp;
}

PDF::PDF()
{
	zoomMax = 3;
	zoomMin = 0.1;
	zoomStep = 0.1;
}

void PDF::LoadPDF(char doc_name[])
{
	FPDF_STRING test_doc = doc_name;
	doc = FPDF_LoadDocument(test_doc, NULL);
	if (!doc) 
	{
	  loaded = false ;
	}
	else
	{
		page = FPDF_LoadPage(doc,0);
		width = FPDF_GetPageWidth(page);
		height = FPDF_GetPageHeight(page);		
		loaded = true ;
	}
	
}

void PDF::ClosePDF()
{
	FPDF_CloseDocument(doc);
}


