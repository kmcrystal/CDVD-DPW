
int WINAPI WinMain (HINSTANCE hInstance, HINSTANCE hPrevInstance,
                    PSTR szCmdLine, int iCmdShow)
{
    HACCEL   hAccel;
    HWND     hwnd;
    MSG      msg;
    WNDCLASS wndclass;
    int ixBorders, iyBorders;
    wndclass.style         = CS_HREDRAW | CS_VREDRAW;
    wndclass.lpfnWndProc   = WndProc;
    wndclass.cbClsExtra    = 0;
    wndclass.cbWndExtra    = 0;
    wndclass.hInstance     = hInstance;
    wndclass.hIcon         = LoadIcon (hInstance, szIconName) ;
    wndclass.hCursor       = LoadCursor (NULL, IDC_ARROW);
    wndclass.hbrBackground = NULL; 
    wndclass.lpszMenuName  = szProgName;
    wndclass.lpszClassName = szProgName;
    if (!RegisterClass (&wndclass))
    {
        MessageBox (NULL, TEXT ("Error: this program requires Windows NT!"),
            szProgName, MB_ICONERROR);
        return 0;
    }
    
    if ((szCmdLine != NULL) && (*szCmdLine != '\0'))
        if (szCmdLine[0] == '"')
            strncpy (szCmdFileName, szCmdLine + 1, strlen(szCmdLine) - 2);
        else
            strcpy (szCmdFileName, szCmdLine);
    else
        strcpy (szCmdFileName, "");
    
    ixBorders = 2 * (GetSystemMetrics (SM_CXBORDER) +
                     GetSystemMetrics (SM_CXDLGFRAME));
    iyBorders = 2 * (GetSystemMetrics (SM_CYBORDER) +
                     GetSystemMetrics (SM_CYDLGFRAME)) +
                     GetSystemMetrics (SM_CYCAPTION) +
                     GetSystemMetrics (SM_CYMENUSIZE) +
                     1; 
    hwnd = CreateWindow (szProgName, szAppName,
        WS_OVERLAPPEDWINDOW,
        CW_USEDEFAULT, CW_USEDEFAULT,
        512 + 2 * MARGIN + ixBorders, 384 + 2 * MARGIN + iyBorders,

        NULL, NULL, hInstance, NULL);
    ShowWindow (hwnd, iCmdShow);
    UpdateWindow (hwnd);
    hAccel = LoadAccelerators (hInstance, szProgName);
    while (GetMessage (&msg, NULL, 0, 0))
    {
        if (!TranslateAccelerator (hwnd, hAccel, &msg))
        {
            TranslateMessage (&msg);
            DispatchMessage (&msg);
        }
    }
    return msg.wParam;
}
