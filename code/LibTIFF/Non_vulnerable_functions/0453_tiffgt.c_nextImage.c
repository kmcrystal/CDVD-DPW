static int
nextImage(void)
{
        if (fileindex < filenum - 1)
                fileindex++;
        else if (tif)
                return fileindex;
        if (tif)
                TIFFClose(tif);
        tif = TIFFOpen(filelist[fileindex], "r");
        if (tif == NULL)
                return -1;
        return fileindex;
}
