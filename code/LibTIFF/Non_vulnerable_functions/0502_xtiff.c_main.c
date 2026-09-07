int
main(int argc, char **argv)
{
    XSetWindowAttributes window_attributes;
    Widget widget_list[3];
    Arg args[5];
    setbuf(stdout, NULL); setbuf(stderr, NULL);
    shellWidget = XtInitialize(argv[0], "XTiff", shellOptions,
        XtNumber(shellOptions), &argc, argv);
    XSetErrorHandler(XTiffErrorHandler);
    XtGetApplicationResources(shellWidget, &appData,
        (XtResourceList) clientResources, (Cardinal) XtNumber(clientResources),
        (ArgList) NULL, (Cardinal) 0);
    if ((argc <= 1) || (argc > 2) || appData.help)
        Usage();
    if (appData.verbose == False) {
        TIFFSetErrorHandler(0);
        TIFFSetWarningHandler(0);
    }
    fileName = argv[1];
    xDisplay = XtDisplay(shellWidget);
    xScreen = DefaultScreen(xDisplay);
    OpenTIFFFile();
    GetTIFFHeader();
    SimpleGammaCorrection();
    GetVisual();
    GetTIFFImage();
    



    XtSetArg(args[0], XtNvisual, xVisual);
    XtSetArg(args[1], XtNcolormap, xColormap);
    XtSetArg(args[2], XtNdepth,
        xImageDepth == 1 ? DefaultDepth(xDisplay, xScreen) : xImageDepth);
    XtSetArg(args[3], XtNiconPixmap,
        XCreateBitmapFromData(xDisplay, RootWindow(xDisplay, xScreen),
            xtifficon_bits, xtifficon_width, xtifficon_height));
    XtSetArg(args[4], XtNallowShellResize, True);
    XtSetValues(shellWidget, args, 5);
    


    formWidget = XtCreateManagedWidget("form", formWidgetClass,
        shellWidget, formArgs, XtNumber(formArgs));
        widget_list[0] = listWidget = XtCreateWidget("list",
            listWidgetClass, formWidget, listArgs, XtNumber(listArgs));
        widget_list[1] = labelWidget = XtCreateWidget("label",
            labelWidgetClass, formWidget, labelArgs, XtNumber(labelArgs));
        widget_list[2] = imageWidget = XtCreateWidget("image",
            widgetClass, formWidget, imageArgs, XtNumber(imageArgs));
    XtManageChildren(widget_list, XtNumber(widget_list));
    


    if (tfImageWidth >= appData.viewportWidth) {
        XtSetArg(args[0], XtNwidth, appData.viewportWidth);
        XtSetValues(shellWidget, args, 1);
    }
    if (tfImageHeight >= appData.viewportHeight) {
        XtSetArg(args[0], XtNheight, appData.viewportHeight);
        XtSetValues(shellWidget, args, 1);
    }
    XtSetArg(args[0], XtNwidth, tfImageWidth);
    XtSetArg(args[1], XtNheight, tfImageHeight);
    XtSetValues(imageWidget, args, 2);
    


    XtSetArg(args[0], XtNfromVert, listWidget);
    XtSetValues(imageWidget, args, 1);
    XtSetArg(args[0], XtNfromHoriz, listWidget);
    XtSetValues(labelWidget, args, 1);
    SetNameLabel();
    XtAddCallback(listWidget, XtNcallback, (XtCallbackProc) SelectProc,
        (XtPointer) NULL);
    XtAddActions(actionsTable, XtNumber(actionsTable));
    XtSetArg(args[0], XtNtranslations,
        XtParseTranslationTable(translationsTable));
    XtSetValues(formWidget, &args[0], 1);
    XtSetValues(imageWidget, &args[0], 1);
    



    XtAddEventHandler(imageWidget, ExposureMask | ButtonPressMask
        | ButtonReleaseMask | Button1MotionMask | KeyPressMask,
        False, EventProc, NULL);
    XtRealizeWidget(shellWidget);
    window_attributes.cursor = XCreateFontCursor(xDisplay, XC_fleur);
    XChangeWindowAttributes(xDisplay, XtWindow(imageWidget),
        CWCursor, &window_attributes);
    CreateXImage();
    XtMainLoop();
    return 0;
}
