void
SetNameLabel()
{
    char buffer[BUFSIZ];
    Arg args[1];
    if (tfMultiPage)
        sprintf(buffer, "%s - page %d", fileName, tfDirectory);
    else
        strcpy(buffer, fileName);
    XtSetArg(args[0], XtNlabel, buffer);
    XtSetValues(labelWidget, args, 1);
}
