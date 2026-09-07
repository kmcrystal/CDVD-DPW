static void
dump(int fd, uint64 diroff)
{
	unsigned i, j;
	uint64* visited_diroff = NULL;
	unsigned int count_visited_dir = 0;
	_TIFF_lseek_f(fd, (_TIFF_off_t) 0, 0);
	if (read(fd, (char*) &hdr, sizeof (TIFFHeaderCommon)) != sizeof (TIFFHeaderCommon))
		ReadError("TIFF header");
	if (hdr.common.tiff_magic != TIFF_BIGENDIAN
	    && hdr.common.tiff_magic != TIFF_LITTLEENDIAN &&
#if HOST_BIGENDIAN
	    
	    MDI_BIGENDIAN != hdr.common.tiff_magic
#else
	    MDI_LITTLEENDIAN != hdr.common.tiff_magic
#endif
	   ) {
		Fatal("Not a TIFF or MDI file, bad magic number %u (%#x)",
		    hdr.common.tiff_magic, hdr.common.tiff_magic);
	}
	if (hdr.common.tiff_magic == TIFF_BIGENDIAN
	    || hdr.common.tiff_magic == MDI_BIGENDIAN)
		swabflag = !bigendian;
	else
		swabflag = bigendian;
	if (swabflag)
		TIFFSwabShort(&hdr.common.tiff_version);
	if (hdr.common.tiff_version==42)
	{
		if (read(fd, (char*) &hdr.classic.tiff_diroff, 4) != 4)
			ReadError("TIFF header");
		if (swabflag)
			TIFFSwabLong(&hdr.classic.tiff_diroff);
		printf("Magic: %#x <%s-endian> Version: %#x <%s>\n",
		    hdr.classic.tiff_magic,
		    hdr.classic.tiff_magic == TIFF_BIGENDIAN ? "big" : "little",
		    42,"ClassicTIFF");
		if (diroff == 0)
			diroff = hdr.classic.tiff_diroff;
	}
	else if (hdr.common.tiff_version==43)
	{
		if (read(fd, (char*) &hdr.big.tiff_offsetsize, 12) != 12)
			ReadError("TIFF header");
		if (swabflag)
		{
			TIFFSwabShort(&hdr.big.tiff_offsetsize);
			TIFFSwabShort(&hdr.big.tiff_unused);
			TIFFSwabLong8(&hdr.big.tiff_diroff);
		}
		printf("Magic: %#x <%s-endian> Version: %#x <%s>\n",
		    hdr.big.tiff_magic,
		    hdr.big.tiff_magic == TIFF_BIGENDIAN ? "big" : "little",
		    43,"BigTIFF");
		printf("OffsetSize: %#x Unused: %#x\n",
		    hdr.big.tiff_offsetsize,hdr.big.tiff_unused);
		if (diroff == 0)
			diroff = hdr.big.tiff_diroff;
		bigtiff = 1;
	}
	else
		Fatal("Not a TIFF file, bad version number %u (%#x)",
		    hdr.common.tiff_version, hdr.common.tiff_version);
	for (i = 0; diroff != 0; i++) {
		for(j=0; j<count_visited_dir; j++)
		{
		    if( visited_diroff[j] == diroff )
		    {
			free(visited_diroff);
			Fatal("Cycle detected in chaining of TIFF directories!");
		    }
		}
                {
                    size_t alloc_size;
                    alloc_size=TIFFSafeMultiply(tmsize_t,(count_visited_dir + 1),
                                                sizeof(uint64));
                    if (alloc_size == 0)
                    {
                        if (visited_diroff)
                            free(visited_diroff);
                        visited_diroff = 0;
                    }
                    else
                    {
                        visited_diroff = (uint64*) realloc(visited_diroff,alloc_size);
                    }
                }
		if( !visited_diroff )
		    Fatal("Out of memory");
		visited_diroff[count_visited_dir] = diroff;
		count_visited_dir ++;
		if (i > 0)
			putchar('\n');
		diroff = ReadDirectory(fd, i, diroff);
	}
	if( visited_diroff )
	    free(visited_diroff);
}
