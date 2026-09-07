int
main(int argc, char* argv[])
{
        int c;
        int dirnum = -1;
        uint32 diroff = 0;
        oerror = TIFFSetErrorHandler(NULL);
        owarning = TIFFSetWarningHandler(NULL);
        while ((c = getopt(argc, argv, "d:o:p:eflmsvw?")) != -1)
            switch (c) {
            case 'd':
                dirnum = atoi(optarg);
                break;
            case 'e':
                oerror = TIFFSetErrorHandler(oerror);
                break;
            case 'l':
                order0 = FILLORDER_LSB2MSB;
                break;
            case 'm':
                order0 = FILLORDER_MSB2LSB;
                break;
            case 'o':
                diroff = strtoul(optarg, NULL, 0);
                break;
            case 'p':
                photo0 = photoArg(optarg);
                break;
            case 's':
                stoponerr = 1;
                break;
            case 'w':
                owarning = TIFFSetWarningHandler(owarning);
                break;
            case 'v':
                verbose = 1;
                break;
            case '?':
                usage();
                
            }
        filenum = argc - optind;
        if ( filenum < 1)
                usage();
        glutInit(&argc, argv);
        glutInitDisplayMode(GLUT_SINGLE | GLUT_RGB);
        


        xmax = glutGet(GLUT_SCREEN_WIDTH);
        ymax = glutGet(GLUT_SCREEN_HEIGHT);
        


        xmax = xmax - xmax / 10.0;
        ymax = ymax - ymax / 10.0;
        filelist = (char **) _TIFFmalloc(filenum * sizeof(char*));
        if (!filelist) {
                TIFFError(argv[0], "Can not allocate space for the file list.");
                return 1;
        }
        _TIFFmemcpy(filelist, argv + optind, filenum * sizeof(char*));
        fileindex = -1;
        if (nextImage() < 0) {
                _TIFFfree(filelist);
                return 2;
        }
        



        if (dirnum != -1 && !TIFFSetDirectory(tif, dirnum))
            TIFFError(argv[0], "Error, seeking to directory %d", dirnum);
        if (diroff != 0 && !TIFFSetSubDirectory(tif, diroff))
            TIFFError(argv[0], "Error, setting subdirectory at %#x", diroff);
        order = order0;
        photo = photo0;
	if (initImage() < 0){
                _TIFFfree(filelist);
                return 3;
        }
        



        glutInitWindowSize(width, height);
        snprintf(title, TITLE_LENGTH - 1, "%s [%u]", filelist[fileindex],
                (unsigned int) TIFFCurrentDirectory(tif));
        glutCreateWindow(title);
        glutDisplayFunc(raster_draw);
        glutReshapeFunc(raster_reshape);
        glutKeyboardFunc(raster_keys);
        glutSpecialFunc(raster_special);
        glutMainLoop();
        cleanup_and_exit();
        return 0;
}
