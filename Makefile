PROG = main
CC = g++
CPPFLAGS = -g -Wall -I"C:\DevLibs\wxWidgets\include\wx-3.1" \
-I"C:\DevLibs\wxWidgets\lib\wx\include\msw-unicode-static-3.1" \
-I"C:\DevLibs\pdfium-windows-x64\include" 

LDFLAGS = -L"C:\DevLibs\mingw64\lib" -L"C:\DevLibs\wxWidgets\lib" \
-L"C:\DevLibs\pdfium-windows-x64\x64\bin"\
-lwx_mswu_aui-3.1 \
-lwx_mswu_core-3.1 \
-lwx_baseu-3.1 \
-lwxexpat \
-lwxjpeg \
-lwxpng \
-lwxregexu \
-lwxscintilla \
-lwxtiff \
-lwxzlib \
-lkernel32 \
-luser32 \
-lgdi32 \
-lcomdlg32 \
-lwinspool \
-lwinmm \
-lshell32 \
-lshlwapi \
-lcomctl32 \
-lole32 \
-loleaut32 \
-luuid \
-lrpcrt4 \
-ladvapi32 \
-lversion \
-lwsock32 \
-lwininet \
-loleacc \
-luxtheme \
-lpdfium 

ODIR=obj
_OBJ = Frame.o App.o Pdf2img.o ImagePanel.o templatePanel.o
OBJ = $(patsubst %,$(ODIR)/%,$(_OBJ))

$(ODIR)/%.o: src/%.cpp
	$(CC) $(CPPFLAGS) -c -o $@ $< 

main: $(OBJ)
	$(CC) -o $@ $^ $(CPPFLAGS) $(LDFLAGS)

clean:
		rm -f *.o *~ 
