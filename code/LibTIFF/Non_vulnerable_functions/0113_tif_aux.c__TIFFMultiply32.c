uint32
_TIFFMultiply32(TIFF* tif, uint32 first, uint32 second, const char* where)
{
	uint32 bytes = first * second;
	if (second && bytes / second != first) {
		TIFFErrorExt(tif->tif_clientdata, where, "Integer overflow in %s", where);
		bytes = 0;
	}
	return bytes;
}
