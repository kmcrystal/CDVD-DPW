static uint64
ReadDirectory(int fd, unsigned int ix, uint64 off)
{
	uint16 dircount;
	uint32 direntrysize;
	void* dirmem = NULL;
	uint64 nextdiroff = 0;
	uint32 n;
	uint8* dp;
	if (off == 0)			
		goto done;
	if (_TIFF_lseek_f(fd, (_TIFF_off_t)off, SEEK_SET) != (_TIFF_off_t)off) {
		Fatal("Seek error accessing TIFF directory");
		goto done;
	}
	if (!bigtiff) {
		if (read(fd, (char*) &dircount, sizeof (uint16)) != sizeof (uint16)) {
			ReadError("directory count");
			goto done;
		}
		if (swabflag)
			TIFFSwabShort(&dircount);
		direntrysize = 12;
	} else {
		uint64 dircount64 = 0;
		if (read(fd, (char*) &dircount64, sizeof (uint64)) != sizeof (uint64)) {
			ReadError("directory count");
			goto done;
		}
		if (swabflag)
			TIFFSwabLong8(&dircount64);
		if (dircount64>0xFFFF) {
			Error("Sanity check on directory count failed");
			goto done;
		}
		dircount = (uint16)dircount64;
		direntrysize = 20;
	}
	dirmem = _TIFFmalloc(TIFFSafeMultiply(tmsize_t,dircount,direntrysize));
	if (dirmem == NULL) {
		Fatal("No space for TIFF directory");
		goto done;
	}
	n = read(fd, (char*) dirmem, dircount*direntrysize);
	if (n != dircount*direntrysize) {
		n /= direntrysize;
		Error(
#if defined(__WIN32__) && defined(_MSC_VER)
	    "Could only read %lu of %u entries in directory at offset %#I64x",
		      (unsigned long)n, dircount, (unsigned __int64) off);
#else
	    "Could only read %lu of %u entries in directory at offset %#llx",
		      (unsigned long)n, dircount, (unsigned long long) off);
#endif
		dircount = n;
		nextdiroff = 0;
	} else {
		if (!bigtiff) {
			uint32 nextdiroff32;
			if (read(fd, (char*) &nextdiroff32, sizeof (uint32)) != sizeof (uint32))
				nextdiroff32 = 0;
			if (swabflag)
				TIFFSwabLong(&nextdiroff32);
			nextdiroff = nextdiroff32;
		} else {
			if (read(fd, (char*) &nextdiroff, sizeof (uint64)) != sizeof (uint64))
				nextdiroff = 0;
			if (swabflag)
				TIFFSwabLong8(&nextdiroff);
		}
	}
#if defined(__WIN32__) && (defined(_MSC_VER) || defined(__MINGW32__))
	printf("Directory %u: offset %I64u (%#I64x) next %I64u (%#I64x)\n", ix,
	    (unsigned __int64)off, (unsigned __int64)off,
	    (unsigned __int64)nextdiroff, (unsigned __int64)nextdiroff);
#else
	printf("Directory %u: offset %llu (%#llx) next %llu (%#llx)\n", ix,
	    (unsigned long long)off, (unsigned long long)off,
	    (unsigned long long)nextdiroff, (unsigned long long)nextdiroff);
#endif
	for (dp = (uint8*)dirmem, n = dircount; n > 0; n--) {
		uint16 tag;
		uint16 type;
		uint16 typewidth;
		uint64 count;
		uint64 datasize;
		int datafits;
		void* datamem;
		uint64 dataoffset;
		int datatruncated;
                int datasizeoverflow;
		tag = *(uint16*)dp;
		if (swabflag)
			TIFFSwabShort(&tag);
		dp += sizeof(uint16);
		type = *(uint16*)dp;
		dp += sizeof(uint16);
		if (swabflag)
			TIFFSwabShort(&type);
		PrintTag(stdout, tag);
		putchar(' ');
		PrintType(stdout, type);
		putchar(' ');
		if (!bigtiff)
		{
			uint32 count32;
			count32 = *(uint32*)dp;
			if (swabflag)
				TIFFSwabLong(&count32);
			dp += sizeof(uint32);
			count = count32;
		}
		else
		{
			memcpy(&count, dp, sizeof(uint64));
			if (swabflag)
				TIFFSwabLong8(&count);
			dp += sizeof(uint64);
		}
#if defined(__WIN32__) && (defined(_MSC_VER) || defined(__MINGW32__))
		printf("%I64u<", (unsigned __int64)count);
#else
		printf("%llu<", (unsigned long long)count);
#endif
		if (type >= NWIDTHS)
			typewidth = 0;
		else
			typewidth = datawidth[type];
		datasize = TIFFSafeMultiply(tmsize_t,count,typewidth);
                datasizeoverflow = (typewidth > 0 && datasize / typewidth != count);
		datafits = 1;
		datamem = dp;
		dataoffset = 0;
		datatruncated = 0;
		if (!bigtiff)
		{
			if (datasizeoverflow || datasize>4)
			{
				uint32 dataoffset32;
				datafits = 0;
				datamem = NULL;
				dataoffset32 = *(uint32*)dp;
				if (swabflag)
					TIFFSwabLong(&dataoffset32);
				dataoffset = dataoffset32;
			}
			dp += sizeof(uint32);
		}
		else
		{
			if (datasizeoverflow || datasize>8)
			{
				datafits = 0;
				datamem = NULL;
				dataoffset = *(uint64*)dp;
				if (swabflag)
					TIFFSwabLong8(&dataoffset);
			}
			dp += sizeof(uint64);
		}
		if (datasizeoverflow || datasize>0x10000)
		{
			datatruncated = 1;
			count = 0x10000/typewidth;
			datasize = TIFFSafeMultiply(tmsize_t,count,typewidth);
		}
		if (count>maxitems)
		{
			datatruncated = 1;
			count = maxitems;
                        datasize = TIFFSafeMultiply(tmsize_t,count,typewidth);
		}
		if (!datafits)
		{
			datamem = _TIFFmalloc(datasize);
			if (datamem) {
				if (_TIFF_lseek_f(fd, (_TIFF_off_t)dataoffset, 0) !=
				    (_TIFF_off_t)dataoffset)
				{
					Error(
				"Seek error accessing tag %u value", tag);
					_TIFFfree(datamem);
					datamem = NULL;
				}
				else if (read(fd, datamem, (size_t)datasize) != (TIFF_SSIZE_T)datasize)
				{
					Error(
				"Read error accessing tag %u value", tag);
					_TIFFfree(datamem);
					datamem = NULL;
				}
			} else
				Error("No space for data for tag %u",tag);
		}
		if (datamem)
		{
			if (swabflag)
			{
				switch (type)
				{
					case TIFF_BYTE:
					case TIFF_ASCII:
					case TIFF_SBYTE:
					case TIFF_UNDEFINED:
						break;
					case TIFF_SHORT:
					case TIFF_SSHORT:
						TIFFSwabArrayOfShort((uint16*)datamem,(tmsize_t)count);
						break;
					case TIFF_LONG:
					case TIFF_SLONG:
					case TIFF_FLOAT:
					case TIFF_IFD:
						TIFFSwabArrayOfLong((uint32*)datamem,(tmsize_t)count);
						break;
					case TIFF_RATIONAL:
					case TIFF_SRATIONAL:
						TIFFSwabArrayOfLong((uint32*)datamem,(tmsize_t)count*2);
						break;
					case TIFF_DOUBLE:
					case TIFF_LONG8:
					case TIFF_SLONG8:
					case TIFF_IFD8:
						TIFFSwabArrayOfLong8((uint64*)datamem,(tmsize_t)count);
						break;
				}
			}
			PrintData(stdout,type,(uint32)count,datamem);
			if (datatruncated)
				printf(" ...");
			if (!datafits)
                                {
                                        _TIFFfree(datamem);
                                        datamem = NULL;
                                }
		}
		printf(">\n");
	}
done:
	if (dirmem)
		_TIFFfree((char *)dirmem);
	return (nextdiroff);
}
