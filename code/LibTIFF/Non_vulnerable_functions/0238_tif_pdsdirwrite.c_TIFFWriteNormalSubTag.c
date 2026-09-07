static int
TIFFWriteNormalSubTag(TIFF* tif, TIFFDirEntry* dir, const TIFFFieldInfo* fip,
		      int (*getFieldFn)(TIFF *tif, ttag_t tag, ...))
{
	u_short wc = (u_short) fip->field_writecount;
	dir->tdir_tag = fip->field_tag;
	dir->tdir_type = (u_short) fip->field_type;
	dir->tdir_count = wc;
#define	WRITEF(x,y)	x(tif, fip->field_type, fip->field_tag, dir, wc, y)
	switch (fip->field_type) {
	case TIFF_SHORT:
	case TIFF_SSHORT:
		if (wc > 1) {
			uint16* wp;
			if (wc == (u_short) TIFF_VARIABLE) {
				(*getFieldFn)(tif, fip->field_tag, &wc, &wp);
				dir->tdir_count = wc;
			} else
				(*getFieldFn)(tif, fip->field_tag, &wp);
			if (!WRITEF(TIFFWriteShortArray, wp))
				return (0);
		} else {
			uint16 sv;
			(*getFieldFn)(tif, fip->field_tag, &sv);
			dir->tdir_offset =
			    TIFFInsertData(tif, dir->tdir_type, sv);
		}
		break;
	case TIFF_LONG:
	case TIFF_SLONG:
		if (wc > 1) {
			uint32* lp;
			if (wc == (u_short) TIFF_VARIABLE) {
				(*getFieldFn)(tif, fip->field_tag, &wc, &lp);
				dir->tdir_count = wc;
			} else
				(*getFieldFn)(tif, fip->field_tag, &lp);
			if (!WRITEF(TIFFWriteLongArray, lp))
				return (0);
		} else {
			
			(*getFieldFn)(tif, fip->field_tag, &dir->tdir_offset);
		}
		break;
	case TIFF_RATIONAL:
	case TIFF_SRATIONAL:
		if (wc > 1) {
			float* fp;
			if (wc == (u_short) TIFF_VARIABLE) {
				(*getFieldFn)(tif, fip->field_tag, &wc, &fp);
				dir->tdir_count = wc;
			} else
				(*getFieldFn)(tif, fip->field_tag, &fp);
			if (!WRITEF(TIFFWriteRationalArray, fp))
				return (0);
		} else {
			float fv;
			(*getFieldFn)(tif, fip->field_tag, &fv);
			if (!WRITEF(TIFFWriteRationalArray, &fv))
				return (0);
		}
		break;
	case TIFF_FLOAT:
		if (wc > 1) {
			float* fp;
			if (wc == (u_short) TIFF_VARIABLE) {
				(*getFieldFn)(tif, fip->field_tag, &wc, &fp);
				dir->tdir_count = wc;
			} else
				(*getFieldFn)(tif, fip->field_tag, &fp);
			if (!WRITEF(TIFFWriteFloatArray, fp))
				return (0);
		} else {
			float fv;
			(*getFieldFn)(tif, fip->field_tag, &fv);
			if (!WRITEF(TIFFWriteFloatArray, &fv))
				return (0);
		}
		break;
	case TIFF_DOUBLE:
		

















#if (0)
		{ double* dp;
		  if (wc == (u_short) TIFF_VARIABLE) {
			(*getFieldFn)(tif, fip->field_tag, &wc, &dp);
			dir->tdir_count = wc;
		  } else
			(*getFieldFn)(tif, fip->field_tag, &dp);
		  TIFFCvtNativeToIEEEDouble(tif, wc, dp);
		  if (!TIFFWriteData(tif, dir, (char*) dp))
			return (0);
		}
#else
		if (wc > 1) {
			double* dp;
			if (wc == (u_short) TIFF_VARIABLE) {
				(*getFieldFn)(tif, fip->field_tag, &wc, &dp);
				dir->tdir_count = wc;
			} else
				(*getFieldFn)(tif, fip->field_tag, &dp);
			if (!WRITEF(TIFFWriteDoubleArray, dp))
				return (0);
		} else {
			double dv;
			(*getFieldFn)(tif, fip->field_tag, &dv);
			if (!WRITEF(TIFFWriteDoubleArray, &dv))
				return (0);
		}
#endif
		break;
	case TIFF_ASCII:
		{ char* cp;
		  (*getFieldFn)(tif, fip->field_tag, &cp);
		  dir->tdir_count = (uint32) (strlen(cp) + 1);
		  if (!TIFFWriteByteArray(tif, dir, cp))
			return (0);
		}
		break;
	case TIFF_UNDEFINED:
		{ char* cp;
		  if (wc == (u_short) TIFF_VARIABLE) {
			(*getFieldFn)(tif, fip->field_tag, &wc, &cp);
			dir->tdir_count = wc;
		  } else 
			(*getFieldFn)(tif, fip->field_tag, &cp);
		  if (!TIFFWriteByteArray(tif, dir, cp))
			return (0);
		}
		break;
	}
	return (1);
}
