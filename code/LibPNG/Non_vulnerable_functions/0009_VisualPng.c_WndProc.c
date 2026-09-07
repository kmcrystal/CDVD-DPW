}
LRESULT CALLBACK WndProc (HWND hwnd, UINT message, WPARAM wParam,
        LPARAM lParam)
{
    static HINSTANCE          hInstance ;
    static HDC                hdc;
    static PAINTSTRUCT        ps;
    static HMENU              hMenu;
    static BITMAPFILEHEADER  *pbmfh;
    static BITMAPINFOHEADER  *pbmih;
    static BYTE              *pbImage;
    static int                cxWinSize, cyWinSize;
    static int                cxImgSize, cyImgSize;
    static int                cImgChannels;
    static png_color          bkgColor = {127, 127, 127};
    static BOOL               bStretched = TRUE;
    static BYTE              *pDib = NULL;
    static BYTE              *pDiData = NULL;
    static TCHAR              szImgPathName [MAX_PATH];
    static TCHAR              szTitleName [MAX_PATH];
    static TCHAR             *pPngFileList = NULL;
    static int                iPngFileCount;
    static int                iPngFileIndex;
    BOOL                      bOk;
    switch (message)
    {
    case WM_CREATE:
        hInstance = ((LPCREATESTRUCT) lParam)->hInstance ;
        PngFileInitialize (hwnd);
        strcpy (szImgPathName, "");
        
        if (szCmdFileName[0] != '\0')
        {
            strcpy (szImgPathName, szCmdFileName);
            
            
            BuildPngList (szImgPathName, &pPngFileList, &iPngFileCount,
                          &iPngFileIndex);
            
            if (!LoadImageFile (hwnd, szImgPathName,
                &pbImage, &cxImgSize, &cyImgSize, &cImgChannels, &bkgColor))
                return 0;
            
            InvalidateRect (hwnd, NULL, TRUE);
            
            DisplayImage (hwnd, &pDib, &pDiData, cxWinSize, cyWinSize,
                pbImage, cxImgSize, cyImgSize, cImgChannels, bStretched);
        }
        return 0;
    case WM_SIZE:
        cxWinSize = LOWORD (lParam);
        cyWinSize = HIWORD (lParam);
        
        InvalidateRect (hwnd, NULL, TRUE);
        
        DisplayImage (hwnd, &pDib, &pDiData, cxWinSize, cyWinSize,
            pbImage, cxImgSize, cyImgSize, cImgChannels, bStretched);
        return 0;
    case WM_INITMENUPOPUP:
        hMenu = GetMenu (hwnd);
        if (pbImage)
            EnableMenuItem (hMenu, IDM_FILE_SAVE, MF_ENABLED);
        else
            EnableMenuItem (hMenu, IDM_FILE_SAVE, MF_GRAYED);
        return 0;
    case WM_COMMAND:
        hMenu = GetMenu (hwnd);
        switch (LOWORD (wParam))
        {
        case IDM_FILE_OPEN:
            
            if (!PngFileOpenDlg (hwnd, szImgPathName, szTitleName))
                return 0;
            
            
            BuildPngList (szImgPathName, &pPngFileList, &iPngFileCount,
                          &iPngFileIndex);
            
            if (!LoadImageFile (hwnd, szImgPathName,
                &pbImage, &cxImgSize, &cyImgSize, &cImgChannels, &bkgColor))
                return 0;
            
            InvalidateRect (hwnd, NULL, TRUE);
            
            DisplayImage (hwnd, &pDib, &pDiData, cxWinSize, cyWinSize,
                pbImage, cxImgSize, cyImgSize, cImgChannels, bStretched);
            return 0;
        case IDM_FILE_SAVE:
            
            if (!PngFileSaveDlg (hwnd, szImgPathName, szTitleName))
                return 0;
            
            SetCursor (LoadCursor (NULL, IDC_WAIT));
            ShowCursor (TRUE);
            bOk = PngSaveImage (szImgPathName, pDiData, cxWinSize, cyWinSize,
                  bkgColor);
            ShowCursor (FALSE);
            SetCursor (LoadCursor (NULL, IDC_ARROW));
            if (!bOk)
                MessageBox (hwnd, TEXT ("Error in saving the PNG image"),
                szProgName, MB_ICONEXCLAMATION | MB_OK);
            return 0;
        case IDM_FILE_NEXT:
            
            if (SearchPngList (pPngFileList, iPngFileCount, &iPngFileIndex,
                NULL, szImgPathName))
            {
                if (strcmp (szImgPathName, "") == 0)
                    return 0;
                
                if (!LoadImageFile (hwnd, szImgPathName, &pbImage,
                        &cxImgSize, &cyImgSize, &cImgChannels, &bkgColor))
                    return 0;
                
                InvalidateRect (hwnd, NULL, TRUE);
                
                DisplayImage (hwnd, &pDib, &pDiData, cxWinSize, cyWinSize,
                    pbImage, cxImgSize, cyImgSize, cImgChannels, bStretched);
            }
            return 0;
        case IDM_FILE_PREVIOUS:
            
            if (SearchPngList (pPngFileList, iPngFileCount, &iPngFileIndex,
                szImgPathName, NULL))
            {
                if (strcmp (szImgPathName, "") == 0)
                    return 0;
                
                if (!LoadImageFile (hwnd, szImgPathName, &pbImage, &cxImgSize,
                    &cyImgSize, &cImgChannels, &bkgColor))
                    return 0;
                
                InvalidateRect (hwnd, NULL, TRUE);
                
                DisplayImage (hwnd, &pDib, &pDiData, cxWinSize, cyWinSize,
                    pbImage, cxImgSize, cyImgSize, cImgChannels, bStretched);
            }
            return 0;
        case IDM_FILE_EXIT:
            
            
            if (pDib != NULL)
            {
                free (pDib);
                pDib = NULL;
            }
            
            if (pPngFileList != NULL)
            {
                free (pPngFileList);
                pPngFileList = NULL;
            }
            
            exit (0);
            return 0;
        case IDM_OPTIONS_STRETCH:
            bStretched = !bStretched;
            if (bStretched)
                CheckMenuItem (hMenu, IDM_OPTIONS_STRETCH, MF_CHECKED);
            else
                CheckMenuItem (hMenu, IDM_OPTIONS_STRETCH, MF_UNCHECKED);
            
            InvalidateRect (hwnd, NULL, TRUE);
            
            DisplayImage (hwnd, &pDib, &pDiData, cxWinSize, cyWinSize,
                pbImage, cxImgSize, cyImgSize, cImgChannels, bStretched);
            return 0;
        case IDM_HELP_ABOUT:
            DialogBox (hInstance, TEXT ("AboutBox"), hwnd, AboutDlgProc) ;
            return 0;
        } 
        break;
    case WM_PAINT:
        hdc = BeginPaint (hwnd, &ps);
        if (pDib)
            SetDIBitsToDevice (hdc, 0, 0, cxWinSize, cyWinSize, 0, 0,
                0, cyWinSize, pDiData, (BITMAPINFO *) pDib, DIB_RGB_COLORS);
        EndPaint (hwnd, &ps);
        return 0;
    case WM_DESTROY:
        if (pbmfh)
        {
            free (pbmfh);
            pbmfh = NULL;
        }
        PostQuitMessage (0);
        return 0;
    }
    return DefWindowProc (hwnd, message, wParam, lParam);
}
