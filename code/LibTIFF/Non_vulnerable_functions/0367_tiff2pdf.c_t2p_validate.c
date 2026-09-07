void t2p_validate(T2P* t2p){
#ifdef JPEG_SUPPORT
	if(t2p->pdf_defaultcompression==T2P_COMPRESS_JPEG){
		if(t2p->pdf_defaultcompressionquality>100 ||
			t2p->pdf_defaultcompressionquality<1){
			t2p->pdf_defaultcompressionquality=0;
		}
	}
#endif
#ifdef ZIP_SUPPORT
	if(t2p->pdf_defaultcompression==T2P_COMPRESS_ZIP){
 		uint16 m=t2p->pdf_defaultcompressionquality%100;
 		if(t2p->pdf_defaultcompressionquality/100 > 9 ||
 			(m>1 && m<10) || m>15){
 			t2p->pdf_defaultcompressionquality=0;
		}
		if(t2p->pdf_defaultcompressionquality%100 !=0){
 			t2p->pdf_defaultcompressionquality/=100;
 			t2p->pdf_defaultcompressionquality*=100;
			TIFFError(
				TIFF2PDF_MODULE, 
				"PNG Group predictor differencing not implemented, assuming compression quality %u", 
				t2p->pdf_defaultcompressionquality);
		}
		t2p->pdf_defaultcompressionquality%=100;
		if(t2p->pdf_minorversion<2){t2p->pdf_minorversion=2;}
	}
#endif
	(void)0;
	return;
}
