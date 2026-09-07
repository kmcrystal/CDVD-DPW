 */
BOOL LoadImageFile (HWND hwnd, PTSTR pstrPathName,
                png_byte **ppbImage, int *pxImgSize, int *pyImgSize,
                int *piChannels, png_color *pBkgColor)
{
    static TCHAR szTmp [MAX_PATH];
    
    if (*ppbImage)
    {
        free (*ppbImage);
        *ppbImage = NULL;
    }
    
    SetCursor (LoadCursor (NULL, IDC_WAIT));
    ShowCursor (TRUE);
    PngLoadImage (pstrPathName, ppbImage, pxImgSize, pyImgSize, piChannels,
                  pBkgColor);
    ShowCursor (FALSE);
    SetCursor (LoadCursor (NULL, IDC_ARROW));
    if (*ppbImage != NULL)
    {
        sprintf (szTmp, "VisualPng - %s", strrchr(pstrPathName, '\\') + 1);
        SetWindowText (hwnd, szTmp);
    }
    else
    {
        MessageBox (hwnd, TEXT ("Error in loading the PNG image"),
            szProgName, MB_ICONEXCLAMATION | MB_OK);
        return FALSE;
    }
    return TRUE;
}
 */
BOOL LoadImageFile (HWND hwnd, PTSTR pstrPathName,
                png_byte **ppbImage, int *pxImgSize, int *pyImgSize,
                int *piChannels, png_color *pBkgColor)
{
    static TCHAR szTmp [MAX_PATH];
    
    if (*ppbImage)
    {
        free (*ppbImage);
        *ppbImage = NULL;
    }
