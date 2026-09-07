static int
_TIFFPrettyPrintField(TIFF* tif, const TIFFField *fip, FILE* fd, uint32 tag,
		      uint32 value_count, void *raw_data)
{
        (void) tif;
	
	if (strncmp(fip->field_name,"Tag ", 4) == 0) {
		return 0;
	}
	switch (tag)
	{
		case TIFFTAG_INKSET:
			if (value_count == 2 && fip->field_type == TIFF_SHORT) {
				fprintf(fd, "  Ink Set: ");
				switch (*((uint16*)raw_data)) {
				case INKSET_CMYK:
					fprintf(fd, "CMYK\n");
					break;
				default:
					fprintf(fd, "%u (0x%x)\n",
						*((uint16*)raw_data),
						*((uint16*)raw_data));
					break;
				}
				return 1;
			}
			return 0;
		case TIFFTAG_DOTRANGE:
			if (value_count == 2 && fip->field_type == TIFF_SHORT) {
				fprintf(fd, "  Dot Range: %u-%u\n",
					((uint16*)raw_data)[0], ((uint16*)raw_data)[1]);
				return 1;
			}
			return 0;
		case TIFFTAG_WHITEPOINT:
			if (value_count == 2 && fip->field_type == TIFF_RATIONAL) {
				fprintf(fd, "  White Point: %g-%g\n",
					((float *)raw_data)[0], ((float *)raw_data)[1]);
				return 1;
			} 
			return 0;
		case TIFFTAG_XMLPACKET:
		{
			uint32 i;
			fprintf(fd, "  XMLPacket (XMP Metadata):\n" );
			for(i = 0; i < value_count; i++)
				fputc(((char *)raw_data)[i], fd);
			fprintf( fd, "\n" );
			return 1;
		}
		case TIFFTAG_RICHTIFFIPTC:
			



			fprintf(fd,
			    "  RichTIFFIPTC Data: <present>, %lu bytes\n",
			    (unsigned long) value_count * 4);
			return 1;
		case TIFFTAG_PHOTOSHOP:
			fprintf(fd, "  Photoshop Data: <present>, %lu bytes\n",
			    (unsigned long) value_count);
			return 1;
		case TIFFTAG_ICCPROFILE:
			fprintf(fd, "  ICC Profile: <present>, %lu bytes\n",
			    (unsigned long) value_count);
			return 1;
		case TIFFTAG_STONITS:
			if (value_count == 1 && fip->field_type == TIFF_DOUBLE) { 
				fprintf(fd,
					"  Sample to Nits conversion factor: %.4e\n",
					*((double*)raw_data));
				return 1;
			}
			return 0;
	}
	return 0;
}
