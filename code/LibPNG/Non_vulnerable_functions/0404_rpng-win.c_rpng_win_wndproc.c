}
LRESULT CALLBACK rpng_win_wndproc(HWND hwnd, UINT iMsg, WPARAM wP, LPARAM lP)
{
    HDC         hdc;
    PAINTSTRUCT ps;
    int rc;
    switch (iMsg) {
        case WM_CREATE:
            
            return 0;
        case WM_PAINT:
            hdc = BeginPaint(hwnd, &ps);
                    
            rc = StretchDIBits(hdc, 0, 0, image_width, image_height,
                    
                                    0, 0, image_width, image_height,
                                    wimage_data, (BITMAPINFO *)bmih,
                    
                                    0, SRCCOPY);
            EndPaint(hwnd, &ps);
            return 0;
        
        case WM_CHAR:
            switch (wP) {      
                case 'q':
                case 'Q':
                case 0x1B:     
                    PostQuitMessage(0);
            }
            return 0;
        case WM_LBUTTONDOWN:   
        case WM_DESTROY:
            PostQuitMessage(0);
            return 0;
    }
    return DefWindowProc(hwnd, iMsg, wP, lP);
}
