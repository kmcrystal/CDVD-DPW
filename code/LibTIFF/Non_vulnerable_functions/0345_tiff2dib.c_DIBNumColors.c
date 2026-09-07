WORD DIBNumColors(LPSTR lpDIB)
{
   WORD wBitCount;  
   




   if (IS_WIN30_DIB(lpDIB))
   {
      DWORD dwClrUsed;
      dwClrUsed = ((LPBITMAPINFOHEADER)lpDIB)->biClrUsed;
      if (dwClrUsed)
     return (WORD)dwClrUsed;
   }
   


   if (IS_WIN30_DIB(lpDIB))
      wBitCount = ((LPBITMAPINFOHEADER)lpDIB)->biBitCount;
   else
      wBitCount = ((LPBITMAPCOREHEADER)lpDIB)->bcBitCount;
   
   switch (wBitCount)
      {
   case 1:
      return 2;
   case 4:
      return 16;
   case 8:
      return 256;
   default:
      return 0;
      }
}
