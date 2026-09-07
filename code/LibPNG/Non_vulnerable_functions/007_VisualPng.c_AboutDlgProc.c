}
BOOL CALLBACK AboutDlgProc (HWND hDlg, UINT message,
                            WPARAM wParam, LPARAM lParam)
{
     switch (message)
     {
     case WM_INITDIALOG :
          ShowWindow (hDlg, SW_HIDE);
          CenterAbout (hDlg, GetWindow (hDlg, GW_OWNER));
          ShowWindow (hDlg, SW_SHOW);
          return TRUE ;
     case WM_COMMAND :
          switch (LOWORD (wParam))
          {
          case IDOK :
          case IDCANCEL :
               EndDialog (hDlg, 0) ;
               return TRUE ;
          }
          break ;
     }
     return FALSE ;
}
