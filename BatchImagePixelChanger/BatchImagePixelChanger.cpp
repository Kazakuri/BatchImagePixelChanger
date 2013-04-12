#include "stdafx.h"
#include <atlstr.h>
#include <atlimage.h>
#include <GdiPlus.h>
#include <GdiPlusImaging.h>
#include <string>

//command line usage exe filename.ext num_copies

int _tmain(int argc, _TCHAR* argv[])
{	
	//Create and load our image
	CImage orig;
	orig.Load(argv[1]);

	//We want to create num_copies images
	for(int i = 1; i <= _ttoi(argv[2]); i++) {

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
			color = RGB((GetRValue(color)-5)%255, (GetGValue(color)-5)%255, (GetBValue(color)-5)%255);

			//replace our pixel
			(*img).SetPixel(width, height, color);
		}

		//Create our output file's name: i.png
		CString t;
		t.Format(_T("%d"), i);
		t += ".png";

		//save our file
		(*img).Save(t, Gdiplus::ImageFormatPNG);

		//free our stored copy
		free(img);
	}

	return 0;
}

