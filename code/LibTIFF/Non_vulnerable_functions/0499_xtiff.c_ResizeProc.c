void
ResizeProc()
{
    Dimension w_width, w_height;
    int xo, yo, ww, wh;
    XEvent fake_event;
    Arg args[2];
    if ((xOffset == 0) && (yOffset == 0))
        return;
    XtSetArg(args[0], XtNwidth, &w_width);
    XtSetArg(args[1], XtNheight, &w_height);
    XtGetValues(shellWidget, args, 2);
    ww = w_width;
    wh = w_height;
    XtGetValues(listWidget, args, 2);
    wh -= w_height;
    xo = xOffset; yo = yOffset;
    if ((xOffset + ww) >= tfImageWidth)
        xOffset = MAX((int) tfImageWidth - ww, 0);
    if ((yOffset + wh) >= tfImageHeight)
        yOffset = MAX((int) tfImageHeight - wh, 0);
    



    if ((xo != xOffset) || (yo != yOffset)) {
        fake_event.type = Expose;
        fake_event.xexpose.x = fake_event.xexpose.y = 0;
        fake_event.xexpose.width = tfImageWidth;
        fake_event.xexpose.height = tfImageHeight;
        EventProc(imageWidget, NULL, &fake_event);
    }
}
