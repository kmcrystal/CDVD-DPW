static int
TIFFFetchNormalSubTag(TIFF* tif, TIFFDirEntry* dp, const TIFFFieldInfo* fip,
		      int (*setFieldFn)(TIFF *tif, ttag_t tag, ...))
{
	static char mesg[] = "to fetch tag value";
	int ok = 0;
	if (dp->tdir_count > 1) {		
		char* cp = NULL;
		switch (dp->tdir_type) {
		case TIFF_BYTE:
		case TIFF_SBYTE:
			
			cp = CheckMalloc(tif,
			    dp->tdir_count * sizeof (uint16), mesg);
			ok = cp && TIFFFetchByteArray(tif, dp, (uint16*) cp);
			break;
		case TIFF_SHORT:
		case TIFF_SSHORT:
			cp = CheckMalloc(tif,
			    dp->tdir_count * sizeof (uint16), mesg);
			ok = cp && TIFFFetchShortArray(tif, dp, (uint16*) cp);
			break;
		case TIFF_LONG:
		case TIFF_SLONG:
			cp = CheckMalloc(tif,
			    dp->tdir_count * sizeof (uint32), mesg);
			ok = cp && TIFFFetchLongArray(tif, dp, (uint32*) cp);
			break;
		case TIFF_RATIONAL:
		case TIFF_SRATIONAL:
			cp = CheckMalloc(tif,
			    dp->tdir_count * sizeof (float), mesg);
			ok = cp && TIFFFetchRationalArray(tif, dp, (float*) cp);
			break;
		case TIFF_FLOAT:
			cp = CheckMalloc(tif,
			    dp->tdir_count * sizeof (float), mesg);
			ok = cp && TIFFFetchFloatArray(tif, dp, (float*) cp);
			break;
		case TIFF_DOUBLE:
			cp = CheckMalloc(tif,
			    dp->tdir_count * sizeof (double), mesg);
			ok = cp && TIFFFetchDoubleArray(tif, dp, (double*) cp);
			break;
		case TIFF_ASCII:
		case TIFF_UNDEFINED:		
			



			cp = CheckMalloc(tif, dp->tdir_count+1, mesg);
			if (ok = (cp && TIFFFetchString(tif, dp, cp)))
				cp[dp->tdir_count] = '\0';	
			break;
		}
		if (ok) {
			ok = (fip->field_passcount ?
			    (*setFieldFn)(tif, dp->tdir_tag, dp->tdir_count, cp)
			  : (*setFieldFn)(tif, dp->tdir_tag, cp));
		}
		if (cp != NULL)
			_TIFFfree(cp);
	} else if (CheckDirCount(tif, dp, 1)) {	
		switch (dp->tdir_type) {
		case TIFF_BYTE:
		case TIFF_SBYTE:
		case TIFF_SHORT:
		case TIFF_SSHORT:
			












			{ TIFFDataType type = fip->field_type;
			  if (type != TIFF_LONG && type != TIFF_SLONG) {
				uint16 v = (uint16)
			   TIFFExtractData(tif, dp->tdir_type, dp->tdir_offset);
				ok = (fip->field_passcount ?
				    (*setFieldFn)(tif, dp->tdir_tag, 1, &v)
				  : (*setFieldFn)(tif, dp->tdir_tag, v));
				break;
			  }
			}
			
		case TIFF_LONG:
		case TIFF_SLONG:
			{ uint32 v32 =
		    TIFFExtractData(tif, dp->tdir_type, dp->tdir_offset);
			  ok = (fip->field_passcount ? 
			      (*setFieldFn)(tif, dp->tdir_tag, 1, &v32)
			    : (*setFieldFn)(tif, dp->tdir_tag, v32));
			}
			break;
		case TIFF_RATIONAL:
		case TIFF_SRATIONAL:
		case TIFF_FLOAT:
			{ float v = (dp->tdir_type == TIFF_FLOAT ? 
			      TIFFFetchFloat(tif, dp)
			    : TIFFFetchRational(tif, dp));
			  ok = (fip->field_passcount ?
			      (*setFieldFn)(tif, dp->tdir_tag, 1, &v)
			    : (*setFieldFn)(tif, dp->tdir_tag, v));
			}
			break;
		case TIFF_DOUBLE:
			{ double v;
			  ok = (TIFFFetchDoubleArray(tif, dp, &v) &&
			    (fip->field_passcount ?
			      (*setFieldFn)(tif, dp->tdir_tag, 1, &v)
			    : (*setFieldFn)(tif, dp->tdir_tag, v))
			  );
			}
			break;
		case TIFF_ASCII:
		case TIFF_UNDEFINED:		
			{ char c[2];
			  if (ok = (TIFFFetchString(tif, dp, c) != 0)) {
				c[1] = '\0';		
				ok = (*setFieldFn)(tif, dp->tdir_tag, c);
			  }
			}
			break;
		}
	}
	return (ok);
}
