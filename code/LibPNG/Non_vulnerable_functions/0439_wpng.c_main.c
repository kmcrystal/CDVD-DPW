int main(int argc, char **argv)
{
#ifndef DOS_OS2_W32
    FILE *keybd;
#endif
#ifdef sgi
    FILE *tmpfile;      
    char tmpline[80];
#endif
    char *inname = NULL, outname[256];
    char *p, pnmchar, pnmline[256];
    char *bgstr, *textbuf = NULL;
    ulg rowbytes;
    int rc, len = 0;
    int error = 0;
    int text = FALSE;
    int maxval;
    double LUT_exponent;                
    double CRT_exponent = 2.2;          
    double default_display_exponent;    
    double default_gamma = 0.0;
    wpng_info.infile = NULL;
    wpng_info.outfile = NULL;
    wpng_info.image_data = NULL;
    wpng_info.row_pointers = NULL;
    wpng_info.filter = FALSE;
    wpng_info.interlaced = FALSE;
    wpng_info.have_bg = FALSE;
    wpng_info.have_time = FALSE;
    wpng_info.have_text = 0;
    wpng_info.gamma = 0.0;
    







#if defined(NeXT)
    
    LUT_exponent = 1.0 / 2.2;
    



#elif defined(sgi)
    LUT_exponent = 1.0 / 1.7;
    

    tmpfile = fopen("/etc/config/system.glGammaVal", "r");
    if (tmpfile) {
        double sgi_gamma;
        fgets(tmpline, 80, tmpfile);
        fclose(tmpfile);
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
    



    if ((p = getenv("SCREEN_GAMMA")) != NULL) {
        double exponent = atof(p);
        if (exponent > 0.0)
            default_gamma = 1.0 / exponent;
    }
    if (default_gamma == 0.0)
        default_gamma = 1.0 / default_display_exponent;
    
    while (*++argv && !error) {
        if (!strncmp(*argv, "-i", 2)) {
            wpng_info.interlaced = TRUE;
        } else if (!strncmp(*argv, "-time", 3)) {
            wpng_info.modtime = time(NULL);
            wpng_info.have_time = TRUE;
        } else if (!strncmp(*argv, "-text", 3)) {
            text = TRUE;
        } else if (!strncmp(*argv, "-gamma", 2)) {
            if (!*++argv)
                ++error;
            else {
                wpng_info.gamma = atof(*argv);
                if (wpng_info.gamma <= 0.0)
                    ++error;
                else if (wpng_info.gamma > 1.01)
                    fprintf(stderr, PROGNAME
                      " warning:  file gammas are usually less than 1.0\n");
            }
        } else if (!strncmp(*argv, "-bgcolor", 4)) {
            if (!*++argv)
                ++error;
            else {
                bgstr = *argv;
                if (strlen(bgstr) != 7 || bgstr[0] != '#')
                    ++error;
                else {
                    unsigned r, g, b;  
                    sscanf(bgstr+1, "%2x%2x%2x", &r, &g, &b);
                    wpng_info.bg_red   = (uch)r;
                    wpng_info.bg_green = (uch)g;
                    wpng_info.bg_blue  = (uch)b;
                    wpng_info.have_bg = TRUE;
                }
            }
        } else {
            if (**argv != '-') {
                inname = *argv;
                if (argv[1])   
                    ++error;
            } else
                ++error;   
        }
    }
    
    if (!inname) {
        if (isatty(0)) {
            fprintf(stderr, PROGNAME
              ":  must give input filename or provide image data via stdin\n");
            ++error;
        } else {
#ifdef DOS_OS2_W32
            
            setmode(fileno(stdin), O_BINARY);
            setmode(fileno(stdout), O_BINARY);
#endif
            if ((wpng_info.infile = fdopen(fileno(stdin), "rb")) == NULL) {
                fprintf(stderr, PROGNAME
                  ":  unable to reopen stdin in binary mode\n");
                ++error;
            } else
            if ((wpng_info.outfile = fdopen(fileno(stdout), "wb")) == NULL) {
                fprintf(stderr, PROGNAME
                  ":  unable to reopen stdout in binary mode\n");
                fclose(wpng_info.infile);
                ++error;
            } else
                wpng_info.filter = TRUE;
        }
    } else if ((len = strlen(inname)) > 250) {
        fprintf(stderr, PROGNAME ":  input filename is too long [%d chars]\n",
          len);
        ++error;
    } else if (!(wpng_info.infile = fopen(inname, "rb"))) {
        fprintf(stderr, PROGNAME ":  can't open input file [%s]\n", inname);
        ++error;
    }
    if (!error) {
        fgets(pnmline, 256, wpng_info.infile);
        if (pnmline[0] != 'P' || ((pnmchar = pnmline[1]) != '5' &&
            pnmchar != '6' && pnmchar != '8'))
        {
            fprintf(stderr, PROGNAME
              ":  input file [%s] is not a binary PGM, PPM or PAM file\n",
              inname);
            ++error;
        } else {
            wpng_info.pnmtype = (int)(pnmchar - '0');
            if (wpng_info.pnmtype != 8)
                wpng_info.have_bg = FALSE;  
            do {
                fgets(pnmline, 256, wpng_info.infile);  
            } while (pnmline[0] == '#');
            sscanf(pnmline, "%ld %ld", &wpng_info.width, &wpng_info.height);
            do {
                fgets(pnmline, 256, wpng_info.infile);  
            } while (pnmline[0] == '#');
            sscanf(pnmline, "%d", &maxval);
            if (wpng_info.width <= 0L || wpng_info.height <= 0L ||
                maxval != 255)
            {
                fprintf(stderr, PROGNAME
                  ":  only positive width/height, maxval == 255 allowed \n");
                ++error;
            }
            wpng_info.sample_depth = 8;  
            if (!wpng_info.filter) {
                
                if ((p = strrchr(inname, '.')) == NULL ||
                    (p - inname) != (len - 4))
                {
                    strcpy(outname, inname);
                    strcpy(outname+len, ".png");
                } else {
                    len -= 4;
                    strncpy(outname, inname, len);
                    strcpy(outname+len, ".png");
                }
                
                if ((wpng_info.outfile = fopen(outname, "rb")) != NULL) {
                    fprintf(stderr, PROGNAME ":  output file exists [%s]\n",
                      outname);
                    fclose(wpng_info.outfile);
                    ++error;
                } else if (!(wpng_info.outfile = fopen(outname, "wb"))) {
                    fprintf(stderr, PROGNAME ":  can't open output file [%s]\n",
                      outname);
                    ++error;
                }
            }
        }
        if (error) {
            fclose(wpng_info.infile);
            wpng_info.infile = NULL;
            if (wpng_info.filter) {
                fclose(wpng_info.outfile);
                wpng_info.outfile = NULL;
            }
        }
    }
    
    if (error) {
        fprintf(stderr, "\n%s %s:  %s\n", PROGNAME, VERSION, APPNAME);
        writepng_version_info();
        fprintf(stderr, "\n"
"Usage:  %s [-gamma exp] [-bgcolor bg] [-text] [-time] [-interlace] pnmfile\n"
"or: ... | %s [-gamma exp] [-bgcolor bg] [-text] [-time] [-interlace] | ...\n"
         "    exp \ttransfer-function exponent (``gamma'') of the image in\n"
         "\t\t  floating-point format (e.g., ``%.5f''); if image looks\n"
         "\t\t  correct on given display system, image gamma is equal to\n"
         "\t\t  inverse of display-system exponent, i.e., 1 / (LUT * CRT)\n"
         "\t\t  (where LUT = lookup-table exponent and CRT = CRT exponent;\n"
         "\t\t  first varies, second is usually 2.2, all are positive)\n"
         "    bg  \tdesired background color for alpha-channel images, in\n"
         "\t\t  7-character hex RGB format (e.g., ``#ff7700'' for orange:\n"
         "\t\t  same as HTML colors)\n"
         "    -text\tprompt interactively for text info (tEXt chunks)\n"
         "    -time\tinclude a tIME chunk (last modification time)\n"
         "    -interlace\twrite interlaced PNG image\n"
         "\n"
"pnmfile or stdin must be a binary PGM (`P5'), PPM (`P6') or (extremely\n"
"unofficial and unsupported!) PAM (`P8') file.  Currently it is required\n"
"to have maxval == 255 (i.e., no scaling).  If pnmfile is specified, it\n"
"is converted to the corresponding PNG file with the same base name but a\n"
"``.png'' extension; files read from stdin are converted and sent to stdout.\n"
"The conversion is progressive (low memory usage) unless interlacing is\n"
"requested; in that case the whole image will be buffered in memory and\n"
"written in one call.\n"
         "\n", PROGNAME, PROGNAME, default_gamma);
        exit(1);
    }
    


    if (text &&
#ifndef DOS_OS2_W32
        (keybd = fdopen(fileno(stderr), "r")) != NULL &&
#endif
        (textbuf = (char *)malloc((5 + 9)*75)) != NULL)
    {
        int i, valid, result;
        fprintf(stderr,
          "Enter text info (no more than 72 characters per line);\n");
        fprintf(stderr, "to skip a field, hit the <Enter> key.\n");
        
        do {
            valid = TRUE;
            p = textbuf + TEXT_TITLE_OFFSET;
            fprintf(stderr, "  Title: ");
            fflush(stderr);
            if (FGETS(p, 74, keybd) && (len = strlen(p)) > 1) {
                if (p[len-1] == '\n')
                    p[--len] = '\0';
                wpng_info.title = p;
                wpng_info.have_text |= TEXT_TITLE;
                if ((result = wpng_isvalid_latin1((uch *)p, len)) >= 0) {
                    fprintf(stderr, "    " PROGNAME " warning:  character code"
                      " %u is %sdiscouraged by the PNG\n    specification "
                      "[first occurrence was at character position #%d]\n",
                      (unsigned)p[result], (p[result] == 27)? "strongly " : "",
                      result+1);
                    fflush(stderr);
#ifdef FORBID_LATIN1_CTRL
                    wpng_info.have_text &= ~TEXT_TITLE;
                    valid = FALSE;
#else
                    if (p[result] == 27) {    
                        wpng_info.have_text &= ~TEXT_TITLE;
                        valid = FALSE;
                    }
#endif
                }
            }
        } while (!valid);
        do {
            valid = TRUE;
            p = textbuf + TEXT_AUTHOR_OFFSET;
            fprintf(stderr, "  Author: ");
            fflush(stderr);
            if (FGETS(p, 74, keybd) && (len = strlen(p)) > 1) {
                if (p[len-1] == '\n')
                    p[--len] = '\0';
                wpng_info.author = p;
                wpng_info.have_text |= TEXT_AUTHOR;
                if ((result = wpng_isvalid_latin1((uch *)p, len)) >= 0) {
                    fprintf(stderr, "    " PROGNAME " warning:  character code"
                      " %u is %sdiscouraged by the PNG\n    specification "
                      "[first occurrence was at character position #%d]\n",
                      (unsigned)p[result], (p[result] == 27)? "strongly " : "",
                      result+1);
                    fflush(stderr);
#ifdef FORBID_LATIN1_CTRL
                    wpng_info.have_text &= ~TEXT_AUTHOR;
                    valid = FALSE;
#else
                    if (p[result] == 27) {    
                        wpng_info.have_text &= ~TEXT_AUTHOR;
                        valid = FALSE;
                    }
#endif
                }
            }
        } while (!valid);
        do {
            valid = TRUE;
            p = textbuf + TEXT_DESC_OFFSET;
            fprintf(stderr, "  Description (up to 9 lines):\n");
            for (i = 1;  i < 10;  ++i) {
                fprintf(stderr, "    [%d] ", i);
                fflush(stderr);
                if (FGETS(p, 74, keybd) && (len = strlen(p)) > 1)
                    p += len;   
                else
                    break;
            }
            if ((len = p - (textbuf + TEXT_DESC_OFFSET)) > 1) {
                if (p[-1] == '\n') {
                    p[-1] = '\0';
                    --len;
                }
                wpng_info.desc = textbuf + TEXT_DESC_OFFSET;
                wpng_info.have_text |= TEXT_DESC;
                p = textbuf + TEXT_DESC_OFFSET;
                if ((result = wpng_isvalid_latin1((uch *)p, len)) >= 0) {
                    fprintf(stderr, "    " PROGNAME " warning:  character code"
                      " %u is %sdiscouraged by the PNG\n    specification "
                      "[first occurrence was at character position #%d]\n",
                      (unsigned)p[result], (p[result] == 27)? "strongly " : "",
                      result+1);
                    fflush(stderr);
#ifdef FORBID_LATIN1_CTRL
                    wpng_info.have_text &= ~TEXT_DESC;
                    valid = FALSE;
#else
                    if (p[result] == 27) {    
                        wpng_info.have_text &= ~TEXT_DESC;
                        valid = FALSE;
                    }
#endif
                }
            }
        } while (!valid);
        do {
            valid = TRUE;
            p = textbuf + TEXT_COPY_OFFSET;
            fprintf(stderr, "  Copyright: ");
            fflush(stderr);
            if (FGETS(p, 74, keybd) && (len = strlen(p)) > 1) {
                if (p[len-1] == '\n')
                    p[--len] = '\0';
                wpng_info.copyright = p;
                wpng_info.have_text |= TEXT_COPY;
                if ((result = wpng_isvalid_latin1((uch *)p, len)) >= 0) {
                    fprintf(stderr, "    " PROGNAME " warning:  character code"
                      " %u is %sdiscouraged by the PNG\n    specification "
                      "[first occurrence was at character position #%d]\n",
                      (unsigned)p[result], (p[result] == 27)? "strongly " : "",
                      result+1);
                    fflush(stderr);
#ifdef FORBID_LATIN1_CTRL
                    wpng_info.have_text &= ~TEXT_COPY;
                    valid = FALSE;
#else
                    if (p[result] == 27) {    
                        wpng_info.have_text &= ~TEXT_COPY;
                        valid = FALSE;
                    }
#endif
                }
            }
        } while (!valid);
        do {
            valid = TRUE;
            p = textbuf + TEXT_EMAIL_OFFSET;
            fprintf(stderr, "  E-mail: ");
            fflush(stderr);
            if (FGETS(p, 74, keybd) && (len = strlen(p)) > 1) {
                if (p[len-1] == '\n')
                    p[--len] = '\0';
                wpng_info.email = p;
                wpng_info.have_text |= TEXT_EMAIL;
                if ((result = wpng_isvalid_latin1((uch *)p, len)) >= 0) {
                    fprintf(stderr, "    " PROGNAME " warning:  character code"
                      " %u is %sdiscouraged by the PNG\n    specification "
                      "[first occurrence was at character position #%d]\n",
                      (unsigned)p[result], (p[result] == 27)? "strongly " : "",
                      result+1);
                    fflush(stderr);
#ifdef FORBID_LATIN1_CTRL
                    wpng_info.have_text &= ~TEXT_EMAIL;
                    valid = FALSE;
#else
                    if (p[result] == 27) {    
                        wpng_info.have_text &= ~TEXT_EMAIL;
                        valid = FALSE;
                    }
#endif
                }
            }
        } while (!valid);
        do {
            valid = TRUE;
            p = textbuf + TEXT_URL_OFFSET;
            fprintf(stderr, "  URL: ");
            fflush(stderr);
            if (FGETS(p, 74, keybd) && (len = strlen(p)) > 1) {
                if (p[len-1] == '\n')
                    p[--len] = '\0';
                wpng_info.url = p;
                wpng_info.have_text |= TEXT_URL;
                if ((result = wpng_isvalid_latin1((uch *)p, len)) >= 0) {
                    fprintf(stderr, "    " PROGNAME " warning:  character code"
                      " %u is %sdiscouraged by the PNG\n    specification "
                      "[first occurrence was at character position #%d]\n",
                      (unsigned)p[result], (p[result] == 27)? "strongly " : "",
                      result+1);
                    fflush(stderr);
#ifdef FORBID_LATIN1_CTRL
                    wpng_info.have_text &= ~TEXT_URL;
                    valid = FALSE;
#else
                    if (p[result] == 27) {    
                        wpng_info.have_text &= ~TEXT_URL;
                        valid = FALSE;
                    }
#endif
                }
            }
        } while (!valid);
#ifndef DOS_OS2_W32
        fclose(keybd);
#endif
    } else if (text) {
        fprintf(stderr, PROGNAME ":  unable to allocate memory for text\n");
        text = FALSE;
        wpng_info.have_text = 0;
    }
    
    if ((rc = writepng_init(&wpng_info)) != 0) {
        switch (rc) {
            case 2:
                fprintf(stderr, PROGNAME
                  ":  libpng initialization problem (longjmp)\n");
                break;
            case 4:
                fprintf(stderr, PROGNAME ":  insufficient memory\n");
                break;
            case 11:
                fprintf(stderr, PROGNAME
                  ":  internal logic error (unexpected PNM type)\n");
                break;
            default:
                fprintf(stderr, PROGNAME
                  ":  unknown writepng_init() error\n");
                break;
        }
        exit(rc);
    }
    

    if (text && textbuf) {
        free(textbuf);
        textbuf = NULL;
    }
    


    if (wpng_info.pnmtype == 5)
        rowbytes = wpng_info.width;
    else if (wpng_info.pnmtype == 6)
        rowbytes = wpng_info.width * 3;
    else 
        rowbytes = wpng_info.width * 4;
    

    fprintf(stderr, "Encoding image data...\n");
    fflush(stderr);
    if (wpng_info.interlaced) {
        long i;
        ulg bytes;
        ulg image_bytes = rowbytes * wpng_info.height;   
        wpng_info.image_data = (uch *)malloc(image_bytes);
        wpng_info.row_pointers = (uch **)malloc(wpng_info.height*sizeof(uch *));
        if (wpng_info.image_data == NULL || wpng_info.row_pointers == NULL) {
            fprintf(stderr, PROGNAME ":  insufficient memory for image data\n");
            writepng_cleanup(&wpng_info);
            wpng_cleanup();
            exit(5);
        }
        for (i = 0;  i < wpng_info.height;  ++i)
            wpng_info.row_pointers[i] = wpng_info.image_data + i*rowbytes;
        bytes = fread(wpng_info.image_data, 1, image_bytes, wpng_info.infile);
        if (bytes != image_bytes) {
            fprintf(stderr, PROGNAME ":  expected %lu bytes, got %lu bytes\n",
              image_bytes, bytes);
            fprintf(stderr, "  (continuing anyway)\n");
        }
        if (writepng_encode_image(&wpng_info) != 0) {
            fprintf(stderr, PROGNAME
              ":  libpng problem (longjmp) while writing image data\n");
            writepng_cleanup(&wpng_info);
            wpng_cleanup();
            exit(2);
        }
    } else  {
        long j;
        ulg bytes;
        wpng_info.image_data = (uch *)malloc(rowbytes);
        if (wpng_info.image_data == NULL) {
            fprintf(stderr, PROGNAME ":  insufficient memory for row data\n");
            writepng_cleanup(&wpng_info);
            wpng_cleanup();
            exit(5);
        }
        error = 0;
        for (j = wpng_info.height;  j > 0L;  --j) {
            bytes = fread(wpng_info.image_data, 1, rowbytes, wpng_info.infile);
            if (bytes != rowbytes) {
                fprintf(stderr, PROGNAME
                  ":  expected %lu bytes, got %lu bytes (row %ld)\n", rowbytes,
                  bytes, wpng_info.height-j);
                ++error;
                break;
            }
            if (writepng_encode_row(&wpng_info) != 0) {
                fprintf(stderr, PROGNAME
                  ":  libpng problem (longjmp) while writing row %ld\n",
                  wpng_info.height-j);
                ++error;
                break;
            }
        }
        if (error) {
            writepng_cleanup(&wpng_info);
            wpng_cleanup();
            exit(2);
        }
        if (writepng_encode_finish(&wpng_info) != 0) {
            fprintf(stderr, PROGNAME ":  error on final libpng call\n");
            writepng_cleanup(&wpng_info);
            wpng_cleanup();
            exit(2);
        }
    }
    
    fprintf(stderr, "Done.\n");
    fflush(stderr);
    writepng_cleanup(&wpng_info);
    wpng_cleanup();
    return 0;
}
