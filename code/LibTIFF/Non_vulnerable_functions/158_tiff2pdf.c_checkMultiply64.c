static uint64
checkMultiply64(uint64 first, uint64 second, T2P* t2p)
{
	uint64 bytes = first * second;
	if (second && bytes / second != first) {
		TIFFError(TIFF2PDF_MODULE, "Integer overflow");
		t2p->t2p_error = T2P_ERR_ERROR;
		bytes = 0;
	}
	return bytes;
}
