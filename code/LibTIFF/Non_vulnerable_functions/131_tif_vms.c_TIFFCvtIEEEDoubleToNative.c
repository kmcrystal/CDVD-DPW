void
TIFFCvtIEEEDoubleToNative(TIFF* tif, u_int n, double* f)
{
	double_t* fp = (double_t*) f;
	while (n-- > 0) {
		IEEEDOUBLE2NATIVE(fp);
		fp++;
	}
}
