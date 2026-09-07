static void
_TIFFPrintField(FILE* fd, const TIFFField *fip,
		uint32 value_count, void *raw_data)
{
	uint32 j;
	fprintf(fd, "  %s: ", fip->field_name);
	for(j = 0; j < value_count; j++) {
		if(fip->field_type == TIFF_BYTE)
			fprintf(fd, "%u", ((uint8 *) raw_data)[j]);
		else if(fip->field_type == TIFF_UNDEFINED)
			fprintf(fd, "0x%x",
			    (unsigned int) ((unsigned char *) raw_data)[j]);
		else if(fip->field_type == TIFF_SBYTE)
			fprintf(fd, "%d", ((int8 *) raw_data)[j]);
		else if(fip->field_type == TIFF_SHORT)
			fprintf(fd, "%u", ((uint16 *) raw_data)[j]);
		else if(fip->field_type == TIFF_SSHORT)
			fprintf(fd, "%d", ((int16 *) raw_data)[j]);
		else if(fip->field_type == TIFF_LONG)
			fprintf(fd, "%lu",
			    (unsigned long)((uint32 *) raw_data)[j]);
		else if(fip->field_type == TIFF_SLONG)
			fprintf(fd, "%ld", (long)((int32 *) raw_data)[j]);
		else if(fip->field_type == TIFF_IFD)
			fprintf(fd, "0x%lx",
				(unsigned long)((uint32 *) raw_data)[j]);
		else if(fip->field_type == TIFF_RATIONAL
			|| fip->field_type == TIFF_SRATIONAL
			|| fip->field_type == TIFF_FLOAT)
			fprintf(fd, "%f", ((float *) raw_data)[j]);
		else if(fip->field_type == TIFF_LONG8)
#if defined(__WIN32__) && (defined(_MSC_VER) || defined(__MINGW32__))
			fprintf(fd, "%I64u",
			    (unsigned __int64)((uint64 *) raw_data)[j]);
#else
			fprintf(fd, "%llu",
			    (unsigned long long)((uint64 *) raw_data)[j]);
#endif
		else if(fip->field_type == TIFF_SLONG8)
#if defined(__WIN32__) && (defined(_MSC_VER) || defined(__MINGW32__))
			fprintf(fd, "%I64d", (__int64)((int64 *) raw_data)[j]);
#else
			fprintf(fd, "%lld", (long long)((int64 *) raw_data)[j]);
#endif
		else if(fip->field_type == TIFF_IFD8)
#if defined(__WIN32__) && (defined(_MSC_VER) || defined(__MINGW32__))
			fprintf(fd, "0x%I64x",
				(unsigned __int64)((uint64 *) raw_data)[j]);
#else
			fprintf(fd, "0x%llx",
				(unsigned long long)((uint64 *) raw_data)[j]);
#endif
		else if(fip->field_type == TIFF_FLOAT)
			fprintf(fd, "%f", ((float *)raw_data)[j]);
		else if(fip->field_type == TIFF_DOUBLE)
			fprintf(fd, "%f", ((double *) raw_data)[j]);
		else if(fip->field_type == TIFF_ASCII) {
			fprintf(fd, "%s", (char *) raw_data);
			break;
		}
		else {
			fprintf(fd, "<unsupported data type in TIFFPrint>");
			break;
		}
		if(j < value_count - 1)
			fprintf(fd, ",");
	}
	fprintf(fd, "\n");
}
