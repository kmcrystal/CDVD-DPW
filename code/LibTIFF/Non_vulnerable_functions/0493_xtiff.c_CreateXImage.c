void
CreateXImage()
{
    XGCValues gc_values;
    GC bitmap_gc;
    xOffset = yOffset = 0;
    grabX = grabY = -1;
    xImage = XCreateImage(xDisplay, xVisual, xImageDepth,
        xImageDepth == 1 ? XYBitmap : ZPixmap,  0,
        (char *) imageMemory, tfImageWidth, tfImageHeight,
         8,  0);
    


    if (xImageDepth == 1)
        xImage->bitmap_bit_order = MSBFirst;
    if (xImageDepth <= 8)
        xImage->byte_order = MSBFirst;
    


    gc_values.function = GXcopy;
    gc_values.plane_mask = AllPlanes;
    if (tfPhotometricInterpretation == PHOTOMETRIC_MINISBLACK) {
        gc_values.foreground = XWhitePixel(xDisplay, xScreen);
        gc_values.background = XBlackPixel(xDisplay, xScreen);
    } else {
        gc_values.foreground = XBlackPixel(xDisplay, xScreen);
        gc_values.background = XWhitePixel(xDisplay, xScreen);
    }
    xWinGc = XCreateGC(xDisplay, XtWindow(shellWidget),
        GCFunction | GCPlaneMask | GCForeground | GCBackground, &gc_values);
    


    if (appData.usePixmap == True) {
        xImagePixmap = XCreatePixmap(xDisplay, RootWindow(xDisplay, xScreen),
            xImage->width, xImage->height, xImageDepth);
        





        if (xImageDepth == 1) {         
            gc_values.foreground = 1;   
            gc_values.background = 0;   
            bitmap_gc = XCreateGC(xDisplay, xImagePixmap,
                GCForeground | GCBackground, &gc_values);
            XPutImage(xDisplay, xImagePixmap, bitmap_gc, xImage,
                0, 0, 0, 0, xImage->width, xImage->height);
        } else
            XPutImage(xDisplay, xImagePixmap, xWinGc, xImage,
                0, 0, 0, 0, xImage->width, xImage->height);
        XDestroyImage(xImage);
        free(imageMemory);
    }
}
