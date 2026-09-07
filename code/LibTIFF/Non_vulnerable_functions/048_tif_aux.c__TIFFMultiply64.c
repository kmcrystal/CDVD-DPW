uint64
_TIFFMultiply64(TIFF* tif, uint64 first, uint64 second, const char* where)
{
	uint64 bytes = first * second;
	if (second && bytes / second != first) {
		TIFFErrorExt(tif->tif_clientdata, where, "Integer overflow in %s", where);
		bytes = 0;
	}
	return bytes;
}
