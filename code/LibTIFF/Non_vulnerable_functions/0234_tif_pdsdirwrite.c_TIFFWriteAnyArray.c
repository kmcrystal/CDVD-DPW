static int
TIFFWriteAnyArray(TIFF* tif,
    TIFFDataType type, ttag_t tag, TIFFDirEntry* dir, uint32 n, double* v)
{
	char buf[10 * sizeof(double)];
	char* w = buf;
	int i, status = 0;
	if (n * TIFFDataWidth(type) > sizeof buf)
		w = (char*) _TIFFmalloc(n * TIFFDataWidth(type));
	switch (type) {
	case TIFF_BYTE:
		{ unsigned char* bp = (unsigned char*) w;
		  for (i = 0; i < n; i++)
			bp[i] = (unsigned char) v[i];
		  dir->tdir_tag = tag;
		  dir->tdir_type = (short) type;
		  dir->tdir_count = n;
		  if (!TIFFWriteByteArray(tif, dir, (char*) bp))
			goto out;
		}
		break;
	case TIFF_SBYTE:
		{ signed char* bp = (signed char*) w;
		  for (i = 0; i < n; i++)
			bp[i] = (signed char) v[i];
		  dir->tdir_tag = tag;
		  dir->tdir_type = (short) type;
		  dir->tdir_count = n;
		  if (!TIFFWriteByteArray(tif, dir, (char*) bp))
			goto out;
		}
		break;
	case TIFF_SHORT:
		{ uint16* bp = (uint16*) w;
		  for (i = 0; i < n; i++)
			bp[i] = (uint16) v[i];
		  if (!TIFFWriteShortArray(tif, type, tag, dir, n, (uint16*)bp))
				goto out;
		}
		break;
	case TIFF_SSHORT:
		{ int16* bp = (int16*) w;
		  for (i = 0; i < n; i++)
			bp[i] = (int16) v[i];
		  if (!TIFFWriteShortArray(tif, type, tag, dir, n, (uint16*)bp))
			goto out;
		}
		break;
	case TIFF_LONG:
		{ uint32* bp = (uint32*) w;
		  for (i = 0; i < n; i++)
			bp[i] = (uint32) v[i];
		  if (!TIFFWriteLongArray(tif, type, tag, dir, n, bp))
			goto out;
		}
		break;
	case TIFF_SLONG:
		{ int32* bp = (int32*) w;
		  for (i = 0; i < n; i++)
			bp[i] = (int32) v[i];
		  if (!TIFFWriteLongArray(tif, type, tag, dir, n, (uint32*) bp))
			goto out;
		}
		break;
	case TIFF_FLOAT:
		{ float* bp = (float*) w;
		  for (i = 0; i < n; i++)
			bp[i] = (float) v[i];
		  if (!TIFFWriteFloatArray(tif, type, tag, dir, n, bp))
			goto out;
		}
		break;
	case TIFF_DOUBLE:
		return (TIFFWriteDoubleArray(tif, type, tag, dir, n, v));
	default:
		
		
		
		
		
		goto out;
	}
	status = 1;
 out:
	if (w != buf)
		_TIFFfree(w);
	return (status);
}
