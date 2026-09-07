static int
ContigCompare(int sample, uint32 row,
	      unsigned char* p1, unsigned char* p2, tsize_t size)
{
    uint32 pix;
    int ppb = 8 / bitspersample;
    int	 samples_to_test;
    if (memcmp(p1, p2, size) == 0)
        return 0;
    samples_to_test = (sample == -1) ? samplesperpixel : 1;
    switch (bitspersample) {
      case 1: case 2: case 4: case 8: 
      {
          unsigned char *pix1 = p1, *pix2 = p2;
          for (pix = 0; pix < imagewidth; pix += ppb) {
              int		s;
              for(s = 0; s < samples_to_test; s++) {
                  if (*pix1 != *pix2) {
                      if( sample == -1 )
                          PrintIntDiff(row, s, pix, *pix1, *pix2);
                      else
                          PrintIntDiff(row, sample, pix, *pix1, *pix2);
                  }
                  pix1++;
                  pix2++;
              }
          }
          break;
      }
      case 16: 
      {
          uint16 *pix1 = (uint16 *)p1, *pix2 = (uint16 *)p2;
          for (pix = 0; pix < imagewidth; pix++) {
              int	s;
              for(s = 0; s < samples_to_test; s++) {
                  if (*pix1 != *pix2)
                      PrintIntDiff(row, sample, pix, *pix1, *pix2);
                  pix1++;
                  pix2++;
              }
          }
          break;
      }
      case 32: 
	if (sampleformat == SAMPLEFORMAT_UINT
	    || sampleformat == SAMPLEFORMAT_INT) {
		uint32 *pix1 = (uint32 *)p1, *pix2 = (uint32 *)p2;
		for (pix = 0; pix < imagewidth; pix++) {
			int	s;
			for(s = 0; s < samples_to_test; s++) {
				if (*pix1 != *pix2) {
					PrintIntDiff(row, sample, pix,
						     *pix1, *pix2);
				}
				pix1++;
				pix2++;
			}
		}
	} else if (sampleformat == SAMPLEFORMAT_IEEEFP) {
		float *pix1 = (float *)p1, *pix2 = (float *)p2;
		for (pix = 0; pix < imagewidth; pix++) {
			int	s;
			for(s = 0; s < samples_to_test; s++) {
				if (fabs(*pix1 - *pix2) < 0.000000000001) {
					PrintFloatDiff(row, sample, pix,
						       *pix1, *pix2);
				}
				pix1++;
				pix2++;
			}
		}
	} else {
		  fprintf(stderr, "Sample format %d is not supported.\n",
			  sampleformat);
		  return -1;
	}
        break;
      default:
	fprintf(stderr, "Bit depth %d is not supported.\n", bitspersample);
	return -1;
    }
    return 0;
}
