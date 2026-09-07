static void
swapBytesInScanline(void *buf, uint32 width, TIFFDataType dtype)
{
	switch (dtype) {
		case TIFF_SHORT:
		case TIFF_SSHORT:
			TIFFSwabArrayOfShort((uint16*)buf,
                                             (unsigned long)width);
			break;
		case TIFF_LONG:
		case TIFF_SLONG:
			TIFFSwabArrayOfLong((uint32*)buf,
                                            (unsigned long)width);
			break;
			
		case TIFF_DOUBLE:
			TIFFSwabArrayOfDouble((double*)buf,
                                              (unsigned long)width);
			break;
		default:
			break;
	}
}
