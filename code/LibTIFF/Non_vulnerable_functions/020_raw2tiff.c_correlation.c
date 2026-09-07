static double
correlation(void *buf1, void *buf2, uint32 n_elem, TIFFDataType dtype)
{
	double	X, Y, M1 = 0.0, M2 = 0.0, D1 = 0.0, D2 = 0.0, K = 0.0;
	uint32	i;
	switch (dtype) {
		case TIFF_BYTE:
		default:
                        for (i = 0; i < n_elem; i++) {
				X = ((unsigned char *)buf1)[i];
				Y = ((unsigned char *)buf2)[i];
				M1 += X, M2 += Y;
				D1 += X * X, D2 += Y * Y;
				K += X * Y;
                        }
			break;
		case TIFF_SBYTE:
                        for (i = 0; i < n_elem; i++) {
				X = ((signed char *)buf1)[i];
				Y = ((signed char *)buf2)[i];
				M1 += X, M2 += Y;
				D1 += X * X, D2 += Y * Y;
				K += X * Y;
                        }
			break;
		case TIFF_SHORT:
                        for (i = 0; i < n_elem; i++) {
				X = ((uint16 *)buf1)[i];
				Y = ((uint16 *)buf2)[i];
				M1 += X, M2 += Y;
				D1 += X * X, D2 += Y * Y;
				K += X * Y;
                        }
			break;
		case TIFF_SSHORT:
                        for (i = 0; i < n_elem; i++) {
				X = ((int16 *)buf1)[i];
				Y = ((int16 *)buf2)[i];
				M1 += X, M2 += Y;
				D1 += X * X, D2 += Y * Y;
				K += X * Y;
                        }
			break;
		case TIFF_LONG:
                        for (i = 0; i < n_elem; i++) {
				X = ((uint32 *)buf1)[i];
				Y = ((uint32 *)buf2)[i];
				M1 += X, M2 += Y;
				D1 += X * X, D2 += Y * Y;
				K += X * Y;
                        }
			break;
		case TIFF_SLONG:
                        for (i = 0; i < n_elem; i++) {
				X = ((int32 *)buf1)[i];
				Y = ((int32 *)buf2)[i];
				M1 += X, M2 += Y;
				D1 += X * X, D2 += Y * Y;
				K += X * Y;
                        }
			break;
		case TIFF_FLOAT:
                        for (i = 0; i < n_elem; i++) {
				X = ((float *)buf1)[i];
				Y = ((float *)buf2)[i];
				M1 += X, M2 += Y;
				D1 += X * X, D2 += Y * Y;
				K += X * Y;
                        }
			break;
		case TIFF_DOUBLE:
                        for (i = 0; i < n_elem; i++) {
				X = ((double *)buf1)[i];
				Y = ((double *)buf2)[i];
				M1 += X, M2 += Y;
				D1 += X * X, D2 += Y * Y;
				K += X * Y;
                        }
			break;
	}
	M1 /= n_elem;
	M2 /= n_elem;
	D1 -= M1 * M1 * n_elem;
	D2 -= M2 * M2 * n_elem;
	K = (K - M1 * M2 * n_elem) / sqrt(D1 * D2);
	return K;
}
