WORD FAR PaletteSize(LPSTR lpDIB)
{
   
   if (IS_WIN30_DIB (lpDIB))
      return (DIBNumColors(lpDIB) * sizeof(RGBQUAD));
   else
      return (DIBNumColors(lpDIB) * sizeof(RGBTRIPLE));
}
