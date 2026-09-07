 */
BOOL DisplayImage (HWND hwnd, BYTE **ppDib,
        BYTE **ppDiData, int cxWinSize, int cyWinSize,
        BYTE *pbImage, int cxImgSize, int cyImgSize, int cImgChannels,
        BOOL bStretched)
{
    BYTE                       *pDib = *ppDib;
    BYTE                       *pDiData = *ppDiData;
    
    BITMAPINFOHEADER           *pbmih;
    WORD                        wDIRowBytes;
    png_color                   bkgBlack = {0, 0, 0};
    png_color                   bkgGray  = {127, 127, 127};
    png_color                   bkgWhite = {255, 255, 255};
    
    wDIRowBytes = (WORD) ((3 * cxWinSize + 3L) >> 2) << 2;
    if (pDib)
    {
        free (pDib);
        pDib = NULL;
    }
    if (!(pDib = (BYTE *) malloc (sizeof(BITMAPINFOHEADER) +
        wDIRowBytes * cyWinSize)))
    {
        MessageBox (hwnd, TEXT ("Error in displaying the PNG image"),
            szProgName, MB_ICONEXCLAMATION | MB_OK);
        *ppDib = pDib = NULL;
        return FALSE;
    }
    *ppDib = pDib;
    memset (pDib, 0, sizeof(BITMAPINFOHEADER));
    
    pbmih = (BITMAPINFOHEADER *) pDib;
    pbmih->biSize = sizeof(BITMAPINFOHEADER);
    pbmih->biWidth = cxWinSize;
    pbmih->biHeight = -((long) cyWinSize);
    pbmih->biPlanes = 1;
    pbmih->biBitCount = 24;
    pbmih->biCompression = 0;
    pDiData = pDib + sizeof(BITMAPINFOHEADER);
    *ppDiData = pDiData;
    
    InitBitmap (pDiData, cxWinSize, cyWinSize);
    
    if (pbImage)
    {
        FillBitmap (
            pDiData, cxWinSize, cyWinSize,
            pbImage, cxImgSize, cyImgSize, cImgChannels,
            bStretched);
    }
    return TRUE;
}
