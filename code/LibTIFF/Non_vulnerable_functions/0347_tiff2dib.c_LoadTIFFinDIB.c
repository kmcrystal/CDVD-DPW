 ************************************************************************/
HDIB LoadTIFFinDIB(LPSTR lpFileName)    
{
    TIFF          *tif;
    unsigned long imageLength; 
    unsigned long imageWidth; 
    unsigned int  BitsPerSample;
    unsigned long LineSize;
    unsigned int  SamplePerPixel;
    unsigned long RowsPerStrip;  
    int           PhotometricInterpretation;
    long          nrow;
	unsigned long row;
    char          *buf;          
    LPBITMAPINFOHEADER lpDIB; 
    HDIB          hDIB;
    char          *lpBits;
    HGLOBAL       hStrip;
    int           i,l;
    int           Align; 
    tif = TIFFOpen(lpFileName, "r");
    if (!tif)
        goto TiffOpenError;
    TIFFGetField(tif, TIFFTAG_IMAGEWIDTH, &imageWidth);
    TIFFGetField(tif, TIFFTAG_IMAGELENGTH, &imageLength);  
    TIFFGetField(tif, TIFFTAG_BITSPERSAMPLE, &BitsPerSample);
    TIFFGetField(tif, TIFFTAG_ROWSPERSTRIP, &RowsPerStrip);  
    TIFFGetField(tif, TIFFTAG_ROWSPERSTRIP, &RowsPerStrip);   
    TIFFGetField(tif, TIFFTAG_PHOTOMETRIC, &PhotometricInterpretation);
    LineSize = TIFFScanlineSize(tif); 
    SamplePerPixel = (int) (LineSize/imageWidth);
    
    Align = 4 - (LineSize % 4);
    if (Align == 4)	Align = 0;
    
    hDIB = CreateDIB((DWORD) imageWidth, (DWORD) imageLength, (WORD)
(BitsPerSample*SamplePerPixel));
    lpDIB  = (LPBITMAPINFOHEADER) GlobalLock(hDIB);
    if (!lpDIB)
          goto OutOfDIBMemory;
    if (lpDIB)
       lpBits = FindDIBBits((LPSTR) lpDIB);
    
	
    if (lpBits)
      {
        lpBits = FindDIBBits((LPSTR) lpDIB);
        lpBits+=((imageWidth*SamplePerPixel)+Align)*(imageLength-1);
		
        hStrip = GlobalAlloc(GHND,TIFFStripSize(tif));
        buf = GlobalLock(hStrip);           
        if (!buf)
           goto OutOfBufMemory;
        
        
        if (PhotometricInterpretation == 3)
        {
          uint16* red;
          uint16* green;
          uint16* blue;
          int16 i;
          LPBITMAPINFO lpbmi;   
          int   Palette16Bits;          
          TIFFGetField(tif, TIFFTAG_COLORMAP, &red, &green, &blue); 
		  
          if (checkcmap(1<<BitsPerSample, red, green, blue) == 16) 
             Palette16Bits = TRUE;
          else
             Palette16Bits = FALSE;
          lpbmi = (LPBITMAPINFO)lpDIB;                      
          
          for (i = (1<<BitsPerSample)-1; i >= 0; i--) 
            {             
             if (Palette16Bits)
                {
                  lpbmi->bmiColors[i].rgbRed =(BYTE) CVT(red[i]);
                  lpbmi->bmiColors[i].rgbGreen = (BYTE) CVT(green[i]);
                  lpbmi->bmiColors[i].rgbBlue = (BYTE) CVT(blue[i]);           
                }
             else
                {
                  lpbmi->bmiColors[i].rgbRed = (BYTE) red[i];
                  lpbmi->bmiColors[i].rgbGreen = (BYTE) green[i];
                  lpbmi->bmiColors[i].rgbBlue = (BYTE) blue[i];        
                }
            }  
        }
        
		
<=> BGR
        for (row = 0; row < imageLength; row += RowsPerStrip) 
          {     
            nrow = (row + RowsPerStrip > imageLength ? imageLength - row :
RowsPerStrip);
            if (TIFFReadEncodedStrip(tif, TIFFComputeStrip(tif, row, 0),
                buf, nrow*LineSize)==-1)
                  {
                     goto TiffReadError;
                  } 
            else
                  {  
                    for (l = 0; l < nrow; l++) 
                      {
                         if (SamplePerPixel  == 3)
                           for (i=0;i< (int) (imageWidth);i++)
                              {
                               lpBits[i*SamplePerPixel+0]=buf[l*LineSize+i*Sample
PerPixel+2]; 
                               lpBits[i*SamplePerPixel+1]=buf[l*LineSize+i*Sample
PerPixel+1];
                               lpBits[i*SamplePerPixel+2]=buf[l*LineSize+i*Sample
PerPixel+0];
                              }
                         else
                           memcpy(lpBits, &buf[(int) (l*LineSize)], (int)
imageWidth*SamplePerPixel); 
                         lpBits-=imageWidth*SamplePerPixel+Align;
                      }
                 }
          }
        GlobalUnlock(hStrip);
        GlobalFree(hStrip);
        GlobalUnlock(hDIB); 
        TIFFClose(tif);
      }
    return hDIB;
    OutOfBufMemory:
    TiffReadError:
       GlobalUnlock(hDIB); 
       GlobalFree(hStrip);
    OutOfDIBMemory:
       TIFFClose(tif);
    TiffOpenError:
       return (HANDLE) 0;
}
