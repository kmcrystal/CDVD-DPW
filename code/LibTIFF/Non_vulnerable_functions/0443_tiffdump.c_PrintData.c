static void
PrintData(FILE* fd, uint16 type, uint32 count, unsigned char* data)
{
	char* sep = "";
	switch (type) {
	case TIFF_BYTE:
		while (count-- > 0)
			fprintf(fd, bytefmt, sep, *data++), sep = " ";
		break;
	case TIFF_SBYTE:
		while (count-- > 0)
			fprintf(fd, sbytefmt, sep, *(char *)data++), sep = " ";
		break;
	case TIFF_UNDEFINED:
		while (count-- > 0)
			fprintf(fd, bytefmt, sep, *data++), sep = " ";
		break;
	case TIFF_ASCII:
		PrintASCII(fd, count, data);
		break;
	case TIFF_SHORT: {
		uint16 *wp = (uint16*)data;
		while (count-- > 0)
			fprintf(fd, shortfmt, sep, *wp++), sep = " ";
		break;
	}
	case TIFF_SSHORT: {
		int16 *wp = (int16*)data;
		while (count-- > 0)
			fprintf(fd, sshortfmt, sep, *wp++), sep = " ";
		break;
	}
	case TIFF_LONG: {
		uint32 *lp = (uint32*)data;
		while (count-- > 0) {
			fprintf(fd, longfmt, sep, (unsigned long) *lp++);
			sep = " ";
		}
		break;
	}
	case TIFF_SLONG: {
		int32 *lp = (int32*)data;
		while (count-- > 0)
			fprintf(fd, slongfmt, sep, (long) *lp++), sep = " ";
		break;
	}
	case TIFF_LONG8: {
		uint64 *llp = (uint64*)data;
		while (count-- > 0) {
                        uint64 val;
                        memcpy(&val, llp, sizeof(uint64));
                        llp ++;
			fprintf(fd, long8fmt, sep, val);
			sep = " ";
		}
		break;
	}
	case TIFF_SLONG8: {
		int64 *llp = (int64*)data;
		while (count-- > 0) {
                        int64 val;
                        memcpy(&val, llp, sizeof(int64));
                        llp ++;
                        fprintf(fd, slong8fmt, sep, val);
                        sep = " ";
                }
		break;
	}
	case TIFF_RATIONAL: {
		uint32 *lp = (uint32*)data;
		while (count-- > 0) {
			if (lp[1] == 0)
				fprintf(fd, "%sNan (%lu/%lu)", sep,
				    (unsigned long) lp[0],
				    (unsigned long) lp[1]);
			else
				fprintf(fd, rationalfmt, sep,
				    (double)lp[0] / (double)lp[1]);
			sep = " ";
			lp += 2;
		}
		break;
	}
	case TIFF_SRATIONAL: {
		int32 *lp = (int32*)data;
		while (count-- > 0) {
			if (lp[1] == 0)
				fprintf(fd, "%sNan (%ld/%ld)", sep,
				    (long) lp[0], (long) lp[1]);
			else
				fprintf(fd, srationalfmt, sep,
				    (double)lp[0] / (double)lp[1]);
			sep = " ";
			lp += 2;
		}
		break;
	}
	case TIFF_FLOAT: {
		float *fp = (float *)data;
		while (count-- > 0)
			fprintf(fd, floatfmt, sep, *fp++), sep = " ";
		break;
	}
	case TIFF_DOUBLE: {
		double *dp = (double *)data;
		while (count-- > 0)
			fprintf(fd, doublefmt, sep, *dp++), sep = " ";
		break;
	}
	case TIFF_IFD: {
		uint32 *lp = (uint32*)data;
		while (count-- > 0) {
			fprintf(fd, ifdfmt, sep, (unsigned long) *lp++);
			sep = " ";
		}
		break;
	}
	case TIFF_IFD8: {
		uint64 *llp = (uint64*)data;
		while (count-- > 0) {
#if defined(__WIN32__) && defined(_MSC_VER)
			fprintf(fd, ifd8fmt, sep, (unsigned __int64) *llp++);
#else
			fprintf(fd, ifd8fmt, sep, (unsigned long long) *llp++);
#endif
			sep = " ";
		}
		break;
	}
	}
}
