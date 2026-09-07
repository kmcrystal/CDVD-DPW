int
TIFFReadPrivateDataSubDirectory(TIFF* tif, toff_t pdir_offset,
				TIFFFieldInfo *field_info,
				int (*setFieldFn)(TIFF *tif, ttag_t tag, ...))
{
	register TIFFDirEntry* dp;
	register int n;
	register TIFFDirectory* td;
	TIFFDirEntry* dir;
	int iv;
	long v;
	double dv;
	const TIFFFieldInfo* fip;
	int fix;
	uint16 dircount;
	uint32 nextdiroff;
	char* cp;
	int diroutoforderwarning = 0;
	
	if (!isMapped(tif)) {
		if (!SeekOK(tif, pdir_offset)) {
			TIFFErrorExt(tif->tif_clientdata, tif->tif_name,
			    "Seek error accessing TIFF private subdirectory");
			return (0);
		}
		if (!ReadOK(tif, &dircount, sizeof (uint16))) {
			TIFFErrorExt(tif->tif_clientdata, tif->tif_name,
			    "Can not read TIFF private subdirectory count");
			return (0);
		}
		if (tif->tif_flags & TIFF_SWAB)
			TIFFSwabShort(&dircount);
		dir = (TIFFDirEntry *)CheckMalloc(tif,
		    dircount * sizeof (TIFFDirEntry), "to read TIFF private subdirectory");
		if (dir == NULL)
			return (0);
		if (!ReadOK(tif, dir, dircount*sizeof (TIFFDirEntry))) {
			TIFFErrorExt(tif->tif_clientdata, tif->tif_name, "Can not read TIFF private subdirectory");
			goto bad;
		}
		


		(void) ReadOK(tif, &nextdiroff, sizeof (uint32));
	} else {
		toff_t off = pdir_offset;
		if (off + sizeof (short) > tif->tif_size) {
			TIFFErrorExt(tif->tif_clientdata, tif->tif_name,
			    "Can not read TIFF private subdirectory count");
			return (0);
		} else
			_TIFFmemcpy(&dircount, tif->tif_base + off, sizeof (uint16));
		off += sizeof (uint16);
		if (tif->tif_flags & TIFF_SWAB)
			TIFFSwabShort(&dircount);
		dir = (TIFFDirEntry *)CheckMalloc(tif,
		    dircount * sizeof (TIFFDirEntry), "to read TIFF private subdirectory");
		if (dir == NULL)
			return (0);
		if (off + dircount*sizeof (TIFFDirEntry) > tif->tif_size) {
			TIFFErrorExt(tif->tif_clientdata, tif->tif_name, "Can not read TIFF private subdirectory");
			goto bad;
		} else
			_TIFFmemcpy(dir, tif->tif_base + off,
			    dircount*sizeof (TIFFDirEntry));
		off += dircount* sizeof (TIFFDirEntry);
		if (off + sizeof (uint32) < tif->tif_size)
			_TIFFmemcpy(&nextdiroff, tif->tif_base+off, sizeof (uint32));
	}
	if (tif->tif_flags & TIFF_SWAB)
		TIFFSwabLong(&nextdiroff);
	






	td = &tif->tif_dir;
	for (fip = field_info, dp = dir, n = dircount;
	     n > 0; n--, dp++) {
		if (tif->tif_flags & TIFF_SWAB) {
			TIFFSwabArrayOfShort(&dp->tdir_tag, 2);
			TIFFSwabArrayOfLong(&dp->tdir_count, 2);
		}
		


		




		if (dp->tdir_tag < fip->field_tag) {
			if (!diroutoforderwarning) {
				TIFFWarning(tif->tif_name,
	"invalid TIFF private subdirectory; tags are not sorted in ascending order");
				diroutoforderwarning = 1;
			}
			fip = field_info;    
		}
		while (fip->field_tag && fip->field_tag < dp->tdir_tag)
			fip++;
		if (!fip->field_tag || fip->field_tag != dp->tdir_tag) {
			TIFFWarning(tif->tif_name,
			    "unknown field with tag %d (0x%x) in private subdirectory ignored",
			    dp->tdir_tag,  dp->tdir_tag);
			dp->tdir_tag = IGNORE;
			fip = field_info;
			continue;
		}
		


		

		if (0 ) {
	ignore:
			dp->tdir_tag = IGNORE;
			continue;
		}
		


		while (dp->tdir_type != (u_short)fip->field_type) {
			if (fip->field_type == TIFF_ANY)	
				break;
			fip++;
			if (!fip->field_tag || fip->field_tag != dp->tdir_tag) {
				TIFFWarning(tif->tif_name,
				   "wrong data type %d for \"%s\"; tag ignored",
				    dp->tdir_type, fip[-1].field_name);
				goto ignore;
			}
		}
		


		if (fip->field_readcount != TIFF_VARIABLE) {
			uint32 expected = (fip->field_readcount == TIFF_SPP) ?
			    (uint32) td->td_samplesperpixel :
			    (uint32) fip->field_readcount;
			if (!CheckDirCount(tif, dp, expected))
				goto ignore;
		}
		
		if (!TIFFFetchNormalSubTag(tif, dp, fip, setFieldFn))
		    goto bad;
	}
	if (dir)
		_TIFFfree(dir);
	return (1);
bad:
	if (dir)
		_TIFFfree(dir);
	return (0);
}
