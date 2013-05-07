#include "stdafx.h"
#include <atlstr.h>
#include <atlimage.h>
#include <GdiPlus.h>
#include <GdiPlusImaging.h>
#include <string>
#include <math.h>

//command line usage exe filename.ext num_copies

int _tmain(int argc, _TCHAR* argv[])
{	
	if(argc < 3) {
		printf("Error: too few arguments passed, please specify a file name and number of copies to make\n");
		exit(1);
	}

	//Create and load our image
	CImage orig;
	try {	
		orig.Load(argv[1]);
	}
	catch(...) {
		printf("Error: can not load image: %s\n", argv[1]);
		exit(1);
	}
	
	//We want to create num_copies images
	int numCpy = _ttoi(argv[2]);
	
	if(numCpy == 0) {
		printf("Error: can not process number\n");
		exit(1);
	}

	try {
		for(int i = 1; i <= numCpy; i++) {
			//Create a back up of our image that we will modify
			CImage *img = new CImage();
			(*img).Create(orig.GetWidth(), orig.GetHeight(), 32);
			orig.BitBlt((*img).GetDC(), 0, 0);
			(*img).ReleaseDC();

			//Get pixel count of our image
			int pixels = (*img).GetWidth() * (*img).GetHeight();

			//Replace 5% of the pixels
			for(int x = 0; x < pixels * 0.05; x++) {
				//Find a random location
				int width = (rand()%(*img).GetWidth());
				int height = (rand()%(*img).GetHeight());

				//Get the current color
				COLORREF color = (*img).GetPixel(width, height);

				//subtract 5 from the RGB values
				color = RGB(max(GetRValue(color)-5, 0), max(GetGValue(color)-5, 0), max(GetBValue(color)-5, 0));

				//replace our pixel
				(*img).SetPixel(width, height, color);
			}

			CString name = "";

			if(argc == 4){
				name = argv[3];
			}

			//Create our output file's named: prefex i.png
			CString t;
			t.Format(_T("%d"), i);
			CString ext = argv[1];
			ext = ext.Mid(ext.GetLength() - 4);
			if(ext == "jpeg")
				ext += ".";
			t += ext;

			//save our file
			if(ext == ".png")
				(*img).Save(name + t, Gdiplus::ImageFormatPNG);
			if(ext == ".jpg" || ext == ".jpeg")
				(*img).Save(name + t, Gdiplus::ImageFormatJPEG);
			if(ext == ".gif")
				(*img).Save(name + t, Gdiplus::ImageFormatGIF);
			else
				(*img).Save(name + t, Gdiplus::ImageFormatPNG);

			//free our stored copy
			free(img);
		}
	}
	catch(...) {
		printf("Something went wrong.\n");
		exit(1);
	}

	return 0;
}