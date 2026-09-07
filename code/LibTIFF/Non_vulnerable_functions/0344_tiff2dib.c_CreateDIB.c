 ************************************************************************/
HDIB CreateDIB(DWORD dwWidth, DWORD dwHeight, WORD wBitCount)
{
   BITMAPINFOHEADER bi;         
   LPBITMAPINFOHEADER lpbi;     
   DWORD dwLen;                 
   HDIB hDIB;
   DWORD dwBytesPerLine;        
   
   if (wBitCount <= 1)
      wBitCount = 1;
   else if (wBitCount <= 4)
      wBitCount = 4;
   else if (wBitCount <= 8)
      wBitCount = 8;
   else if (wBitCount <= 24)
      wBitCount = 24;
   else
      wBitCount = 4;  
   
   bi.biSize = sizeof(BITMAPINFOHEADER);
   bi.biWidth = dwWidth;         
   bi.biHeight = dwHeight;       
   bi.biPlanes = 1;              
   bi.biBitCount = wBitCount;    
   bi.biCompression = BI_RGB;    
   bi.biSizeImage = (dwWidth*dwHeight*wBitCount)/8; 
mean "default"
   bi.biXPelsPerMeter = 2834; 
   bi.biYPelsPerMeter = 2834; 
   bi.biClrUsed = 0;
   bi.biClrImportant = 0;
   
   
   
   dwBytesPerLine =   (((wBitCount * dwWidth) + 31) / 32 * 4);
   dwLen = bi.biSize + PaletteSize((LPSTR)&bi) + (dwBytesPerLine * dwHeight);
   
   hDIB = GlobalAlloc(GHND, dwLen);
   
   if (!hDIB)
   {
      return NULL;
   }
   
   lpbi = (VOID FAR *)GlobalLock(hDIB);
   
   
   *lpbi = bi;
   
   
   GlobalUnlock(hDIB);
   
   return hDIB;
}
