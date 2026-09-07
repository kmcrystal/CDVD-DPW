static int
TIFFAdvanceDirectory(TIFF* tif, uint32* nextdir, toff_t* off)
{
    static const char module[] = "TIFFAdvanceDirectory";
    uint16 dircount;
    if (isMapped(tif))
    {
        toff_t poff=*nextdir;
        if (poff+sizeof(uint16) > tif->tif_size)
        {
			TIFFErrorExt(tif->tif_clientdata, module, "%s: Error fetching directory count",
                      tif->tif_name);
            return (0);
        }
        _TIFFmemcpy(&dircount, tif->tif_base+poff, sizeof (uint16));
        if (tif->tif_flags & TIFF_SWAB)
            TIFFSwabShort(&dircount);
        poff+=sizeof (uint16)+dircount*sizeof (TIFFDirEntry);
        if (off != NULL)
            *off = poff;
        if (((toff_t) (poff+sizeof (uint32))) > tif->tif_size)
        {
			TIFFErrorExt(tif->tif_clientdata, module, "%s: Error fetching directory link",
                      tif->tif_name);
            return (0);
        }
        _TIFFmemcpy(nextdir, tif->tif_base+poff, sizeof (uint32));
        if (tif->tif_flags & TIFF_SWAB)
            TIFFSwabLong(nextdir);
        return (1);
    }
    else
    {
        if (!SeekOK(tif, *nextdir) ||
            !ReadOK(tif, &dircount, sizeof (uint16))) {
			TIFFErrorExt(tif->tif_clientdata, module, "%s: Error fetching directory count",
                      tif->tif_name);
            return (0);
        }
        if (tif->tif_flags & TIFF_SWAB)
            TIFFSwabShort(&dircount);
        if (off != NULL)
            *off = TIFFSeekFile(tif,
                                dircount*sizeof (TIFFDirEntry), SEEK_CUR);
        else
            (void) TIFFSeekFile(tif,
                                dircount*sizeof (TIFFDirEntry), SEEK_CUR);
        if (!ReadOK(tif, nextdir, sizeof (uint32))) {
			TIFFErrorExt(tif->tif_clientdata, module, "%s: Error fetching directory link",
                      tif->tif_name);
            return (0);
        }
        if (tif->tif_flags & TIFF_SWAB)
            TIFFSwabLong(nextdir);
        return (1);
    }
}
