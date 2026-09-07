void
OpenTIFFFile()
{
    if (tfFile != NULL)
        TIFFClose(tfFile);
    if ((tfFile = TIFFOpen(fileName, "r")) == NULL) {
	fprintf(appData.verbose ? stderr : stdout,
	    "xtiff: can't open %s as a TIFF file\n", fileName);
        exit(0);
    }
    tfMultiPage = (TIFFLastDirectory(tfFile) ? False : True);
}
