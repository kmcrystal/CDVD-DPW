static int global_showmode;
int WINAPI WinMain(HINSTANCE hInst, HINSTANCE hPrevInst, PSTR cmd, int showmode)
{
    char *args[1024];                 
    char **argv = args;
    char *p, *q, *bgstr = NULL;
    int argc = 0;
    int rc, alen, flen;
    int error = 0;
    int timing = FALSE;
    int have_bg = FALSE;
    double LUT_exponent;              
    double CRT_exponent = 2.2;        
    double default_display_exponent;  
    MSG msg;
    


    global_hInst = hInst;
    global_showmode = showmode;
    filename = (char *)NULL;
    memset(&rpng2_info, 0, sizeof(mainprog_info));
#ifndef __CYGWIN__
    



    AllocConsole();
    freopen("CONOUT$", "a", stderr);
    freopen("CONOUT$", "a", stdout);
#endif
    





#if defined(NeXT)
    
    LUT_exponent = 1.0 / 2.2;
    



#elif defined(sgi)
    LUT_exponent = 1.0 / 1.7;
    

    infile = fopen("/etc/config/system.glGammaVal", "r");
    if (infile) {
        double sgi_gamma;
        fgets(tmpline, 80, infile);
        fclose(infile);
        sgi_gamma = atof(tmpline);
        if (sgi_gamma > 0.0)
            LUT_exponent = 1.0 / sgi_gamma;
    }
#elif defined(Macintosh)
    LUT_exponent = 1.8 / 2.61;
    



#else
    LUT_exponent = 1.0;   
#endif
    
    default_display_exponent = LUT_exponent * CRT_exponent;
    



    if ((p = getenv("SCREEN_GAMMA")) != NULL)
        rpng2_info.display_exponent = atof(p);
    else
        rpng2_info.display_exponent = default_display_exponent;
    


    argv[argc++] = PROGNAME;
    p = cmd;
    for (;;) {
        if (*p == ' ')
            while (*++p == ' ')
                ;
        
        if (*p == '\0')
            break;    
        argv[argc++] = q = p;
        while (*q && *q != ' ')
            ++q;
        
        if (*q == '\0')
            break;    
        *q = '\0';    
        p = q + 1;
    }
    argv[argc] = NULL;   
    
    while (*++argv && !error) {
        if (!strncmp(*argv, "-gamma", 2)) {
            if (!*++argv)
                ++error;
            else {
                rpng2_info.display_exponent = atof(*argv);
                if (rpng2_info.display_exponent <= 0.0)
                    ++error;
            }
        } else if (!strncmp(*argv, "-bgcolor", 4)) {
            if (!*++argv)
                ++error;
            else {
                bgstr = *argv;
                if (strlen(bgstr) != 7 || bgstr[0] != '#')
                    ++error;
                else {
                    have_bg = TRUE;
                    bg_image = FALSE;
                }
            }
        } else if (!strncmp(*argv, "-bgpat", 4)) {
            if (!*++argv)
                ++error;
            else {
                pat = atoi(*argv) - 1;
                if (pat < 0 || pat >= num_bgpat)
                    ++error;
                else {
                    bg_image = TRUE;
                    have_bg = FALSE;
                }
            }
        } else if (!strncmp(*argv, "-timing", 2)) {
            timing = TRUE;
        } else {
            if (**argv != '-') {
                filename = *argv;
                if (argv[1])   
                    ++error;
            } else
                ++error;   
        }
    }
    if (!filename)
        ++error;
    
    if (error) {
#ifndef __CYGWIN__
        int ch;
#endif
        fprintf(stderr, "\n%s %s:  %s\n\n", PROGNAME, VERSION, appname);
        readpng2_version_info();
        fprintf(stderr, "\n"
          "Usage:  %s [-gamma exp] [-bgcolor bg | -bgpat pat] [-timing]\n"
          "        %*s file.png\n\n"
          "    exp \ttransfer-function exponent (``gamma'') of the display\n"
          "\t\t  system in floating-point format (e.g., ``%.1f''); equal\n"
          "\t\t  to the product of the lookup-table exponent (varies)\n"
          "\t\t  and the CRT exponent (usually 2.2); must be positive\n"
          "    bg  \tdesired background color in 7-character hex RGB format\n"
          "\t\t  (e.g., ``#ff7700'' for orange:  same as HTML colors);\n"
          "\t\t  used with transparent images; overrides -bgpat option\n"
          "    pat \tdesired background pattern number (1-%d); used with\n"
          "\t\t  transparent images; overrides -bgcolor option\n"
          "    -timing\tenables delay for every block read, to simulate modem\n"
          "\t\t  download of image (~36 Kbps)\n"
          "\nPress Q, Esc or mouse button 1 after image is displayed to quit.\n"
#ifndef __CYGWIN__
          "Press Q or Esc to quit this usage screen. ",
#else
          ,
#endif
          PROGNAME,
#if (defined(__i386__) || defined(_M_IX86) || defined(__x86_64__)) && \
    !(defined(__CYGWIN__) || defined(__MINGW32__))
          (int)strlen(PROGNAME), " ",
#endif
          (int)strlen(PROGNAME), " ", default_display_exponent, num_bgpat);
        fflush(stderr);
#ifndef __CYGWIN__
        do
            ch = _getch();
        while (ch != 'q' && ch != 'Q' && ch != 0x1B);
#endif
        exit(1);
    }
    if (!(infile = fopen(filename, "rb"))) {
        fprintf(stderr, PROGNAME ":  can't open PNG file [%s]\n", filename);
        ++error;
    } else {
        incount = fread(inbuf, 1, INBUFSIZE, infile);
        if (incount < 8 || !readpng2_check_sig(inbuf, 8)) {
            fprintf(stderr, PROGNAME
              ":  [%s] is not a PNG file: incorrect signature\n",
              filename);
            ++error;
        } else if ((rc = readpng2_init(&rpng2_info)) != 0) {
            switch (rc) {
                case 2:
                    fprintf(stderr, PROGNAME
                      ":  [%s] has bad IHDR (libpng longjmp)\n", filename);
                    break;
                case 4:
                    fprintf(stderr, PROGNAME ":  insufficient memory\n");
                    break;
                default:
                    fprintf(stderr, PROGNAME
                      ":  unknown readpng2_init() error\n");
                    break;
            }
            ++error;
        }
        if (error)
            fclose(infile);
    }
    if (error) {
#ifndef __CYGWIN__
        int ch;
#endif
        fprintf(stderr, PROGNAME ":  aborting.\n");
#ifndef __CYGWIN__
        do
            ch = _getch();
        while (ch != 'q' && ch != 'Q' && ch != 0x1B);
#endif
        exit(2);
    } else {
        fprintf(stderr, "\n%s %s:  %s\n", PROGNAME, VERSION, appname);
#ifndef __CYGWIN__
        fprintf(stderr,
          "\n   [console window:  closing this window will terminate %s]\n\n",
          PROGNAME);
#endif
        fflush(stderr);
    }
    
    alen = strlen(appname);
    flen = strlen(filename);
    if (alen + flen + 3 > 1023)
        sprintf(titlebar, "%s:  ...%s", appname, filename+(alen+flen+6-1023));
    else
        sprintf(titlebar, "%s:  %s", appname, filename);
    
    if (have_bg) {
        unsigned r, g, b;   
        sscanf(bgstr+1, "%2x%2x%2x", &r, &g, &b);
        rpng2_info.bg_red   = (uch)r;
        rpng2_info.bg_green = (uch)g;
        rpng2_info.bg_blue  = (uch)b;
    } else
        rpng2_info.need_bgcolor = TRUE;
    rpng2_info.state = kPreInit;
    rpng2_info.mainprog_init = rpng2_win_init;
    rpng2_info.mainprog_display_row = rpng2_win_display_row;
    rpng2_info.mainprog_finish_display = rpng2_win_finish_display;
    







    for (;;) {
        Trace((stderr, "about to call readpng2_decode_data()\n"))
        if (readpng2_decode_data(&rpng2_info, inbuf, incount))
            ++error;
        Trace((stderr, "done with readpng2_decode_data()\n"))
        if (error || incount != INBUFSIZE || rpng2_info.state == kDone) {
            if (rpng2_info.state == kDone) {
                Trace((stderr, "done decoding PNG image\n"))
            } else if (ferror(infile)) {
                fprintf(stderr, PROGNAME
                  ":  error while reading PNG image file\n");
                exit(3);
            } else if (feof(infile)) {
                fprintf(stderr, PROGNAME ":  end of file reached "
                  "(unexpectedly) while reading PNG image file\n");
                exit(3);
            } else  {
                
            }
            break;
        }
        if (timing)
            Sleep(1000L);
        incount = fread(inbuf, 1, INBUFSIZE, infile);
    }
    
    fclose(infile);
    Trace((stderr, "about to call readpng2_cleanup()\n"))
    readpng2_cleanup(&rpng2_info);
    if (error) {
        fprintf(stderr, PROGNAME ":  libpng error while decoding PNG image\n");
        exit(3);
    }
    
    while (GetMessage(&msg, NULL, 0, 0)) {
        TranslateMessage(&msg);
        DispatchMessage(&msg);
    }
    
    Trace((stderr, "about to call rpng2_win_cleanup()\n"))
    rpng2_win_cleanup();
    return msg.wParam;
}
