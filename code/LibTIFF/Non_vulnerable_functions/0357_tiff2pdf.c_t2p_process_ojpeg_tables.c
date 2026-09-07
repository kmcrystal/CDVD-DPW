int t2p_process_ojpeg_tables(T2P* t2p, TIFF* input){
	uint16 proc=0;
	void* q;
	uint32 q_length=0;
	void* dc;
	uint32 dc_length=0;
	void* ac;
	uint32 ac_length=0;
	uint16* lp;
	uint16* pt;
	uint16 h_samp=1;
	uint16 v_samp=1;
	unsigned char* ojpegdata;
	uint16 table_count;
	uint32 offset_table;
	uint32 offset_ms_l;
	uint32 code_count;
	uint32 i=0;
	uint32 dest=0;
	uint16 ri=0;
	uint32 rows=0;
	if(!TIFFGetField(input, TIFFTAG_JPEGPROC, &proc)){
		TIFFError(TIFF2PDF_MODULE, 
			"Missing JPEGProc field in OJPEG image %s", 
			TIFFFileName(input));
			t2p->t2p_error = T2P_ERR_ERROR;
		return(0);
	}
	if(proc!=JPEGPROC_BASELINE && proc!=JPEGPROC_LOSSLESS){
		TIFFError(TIFF2PDF_MODULE, 
			"Bad JPEGProc field in OJPEG image %s", 
			TIFFFileName(input));
			t2p->t2p_error = T2P_ERR_ERROR;
		return(0);
	}
	if(!TIFFGetField(input, TIFFTAG_JPEGQTABLES, &q_length, &q)){
		TIFFError(TIFF2PDF_MODULE, 
			"Missing JPEGQTables field in OJPEG image %s", 
			TIFFFileName(input));
			t2p->t2p_error = T2P_ERR_ERROR;
		return(0);
	}
	if(q_length < (64U * t2p->tiff_samplesperpixel)){
		TIFFError(TIFF2PDF_MODULE, 
			"Bad JPEGQTables field in OJPEG image %s", 
			TIFFFileName(input));
			t2p->t2p_error = T2P_ERR_ERROR;
		return(0);
	} 
	if(!TIFFGetField(input, TIFFTAG_JPEGDCTABLES, &dc_length, &dc)){
		TIFFError(TIFF2PDF_MODULE, 
			"Missing JPEGDCTables field in OJPEG image %s", 
			TIFFFileName(input));
			t2p->t2p_error = T2P_ERR_ERROR;
		return(0);
	}
	if(proc==JPEGPROC_BASELINE){
		if(!TIFFGetField(input, TIFFTAG_JPEGACTABLES, &ac_length, &ac)){
			TIFFError(TIFF2PDF_MODULE, 
				"Missing JPEGACTables field in OJPEG image %s", 
				TIFFFileName(input));
				t2p->t2p_error = T2P_ERR_ERROR;
			return(0);
		}
	} else {
		if(!TIFFGetField(input, TIFFTAG_JPEGLOSSLESSPREDICTORS, &lp)){
			TIFFError(TIFF2PDF_MODULE, 
				"Missing JPEGLosslessPredictors field in OJPEG image %s", 
				TIFFFileName(input));
				t2p->t2p_error = T2P_ERR_ERROR;
				return(0);
		}
		if(!TIFFGetField(input, TIFFTAG_JPEGPOINTTRANSFORM, &pt)){
			TIFFError(TIFF2PDF_MODULE, 
				"Missing JPEGPointTransform field in OJPEG image %s", 
				TIFFFileName(input));
				t2p->t2p_error = T2P_ERR_ERROR;
			return(0);
		}
	}
	if(!TIFFGetField(input, TIFFTAG_YCBCRSUBSAMPLING, &h_samp, &v_samp)){
		h_samp=1;
		v_samp=1;
	}
	if(t2p->pdf_ojpegdata != NULL){
		_TIFFfree(t2p->pdf_ojpegdata);
		t2p->pdf_ojpegdata=NULL;
	} 
	t2p->pdf_ojpegdata = _TIFFmalloc(2048);
	if(t2p->pdf_ojpegdata == NULL){
		TIFFError(TIFF2PDF_MODULE, 
			"Can't allocate %u bytes of memory for t2p_process_ojpeg_tables, %s", 
			2048, 
			TIFFFileName(input));
		t2p->t2p_error = T2P_ERR_ERROR;
		return(0);
	}
	_TIFFmemset(t2p->pdf_ojpegdata, 0x00, 2048);
	t2p->pdf_ojpegdatalength = 0;
	table_count=t2p->tiff_samplesperpixel;
	if(proc==JPEGPROC_BASELINE){
		if(table_count>2) table_count=2;
	}
	ojpegdata=(unsigned char*)t2p->pdf_ojpegdata;
	ojpegdata[t2p->pdf_ojpegdatalength++]=0xff;
	ojpegdata[t2p->pdf_ojpegdatalength++]=0xd8;
	ojpegdata[t2p->pdf_ojpegdatalength++]=0xff;
	if(proc==JPEGPROC_BASELINE){
		ojpegdata[t2p->pdf_ojpegdatalength++]=0xc0;
	} else {
		ojpegdata[t2p->pdf_ojpegdatalength++]=0xc3;
	}
	ojpegdata[t2p->pdf_ojpegdatalength++]=0x00;
	ojpegdata[t2p->pdf_ojpegdatalength++]=(8 + 3*t2p->tiff_samplesperpixel);
	ojpegdata[t2p->pdf_ojpegdatalength++]=(t2p->tiff_bitspersample & 0xff);
	if(TIFFIsTiled(input)){
		ojpegdata[t2p->pdf_ojpegdatalength++]=
			(t2p->tiff_tiles[t2p->pdf_page].tiles_tilelength >> 8) & 0xff;
		ojpegdata[t2p->pdf_ojpegdatalength++]=
			(t2p->tiff_tiles[t2p->pdf_page].tiles_tilelength ) & 0xff;
		ojpegdata[t2p->pdf_ojpegdatalength++]=
			(t2p->tiff_tiles[t2p->pdf_page].tiles_tilewidth >> 8) & 0xff;
		ojpegdata[t2p->pdf_ojpegdatalength++]=
			(t2p->tiff_tiles[t2p->pdf_page].tiles_tilewidth ) & 0xff;
	} else {
		ojpegdata[t2p->pdf_ojpegdatalength++]=
			(t2p->tiff_length >> 8) & 0xff;
		ojpegdata[t2p->pdf_ojpegdatalength++]=
			(t2p->tiff_length ) & 0xff;
		ojpegdata[t2p->pdf_ojpegdatalength++]=
			(t2p->tiff_width >> 8) & 0xff;
		ojpegdata[t2p->pdf_ojpegdatalength++]=
			(t2p->tiff_width ) & 0xff;
	}
	ojpegdata[t2p->pdf_ojpegdatalength++]=(t2p->tiff_samplesperpixel & 0xff);
	for(i=0;i<t2p->tiff_samplesperpixel;i++){
		ojpegdata[t2p->pdf_ojpegdatalength++]=i;
		if(i==0){
			ojpegdata[t2p->pdf_ojpegdatalength] |= h_samp<<4 & 0xf0;;
			ojpegdata[t2p->pdf_ojpegdatalength++] |= v_samp & 0x0f;
		} else {
				ojpegdata[t2p->pdf_ojpegdatalength++]= 0x11;
		}
		ojpegdata[t2p->pdf_ojpegdatalength++]=i;
	}
	for(dest=0;dest<t2p->tiff_samplesperpixel;dest++){
		ojpegdata[t2p->pdf_ojpegdatalength++]=0xff;
		ojpegdata[t2p->pdf_ojpegdatalength++]=0xdb;
		ojpegdata[t2p->pdf_ojpegdatalength++]=0x00;
		ojpegdata[t2p->pdf_ojpegdatalength++]=0x43;
		ojpegdata[t2p->pdf_ojpegdatalength++]=dest;
		_TIFFmemcpy( &(ojpegdata[t2p->pdf_ojpegdatalength++]), 
			&(((unsigned char*)q)[64*dest]), 64);
		t2p->pdf_ojpegdatalength+=64;
	}
	offset_table=0;
	for(dest=0;dest<table_count;dest++){
		ojpegdata[t2p->pdf_ojpegdatalength++]=0xff;
		ojpegdata[t2p->pdf_ojpegdatalength++]=0xc4;
		offset_ms_l=t2p->pdf_ojpegdatalength;
		t2p->pdf_ojpegdatalength+=2;
		ojpegdata[t2p->pdf_ojpegdatalength++]=dest & 0x0f;
		_TIFFmemcpy( &(ojpegdata[t2p->pdf_ojpegdatalength]), 
			&(((unsigned char*)dc)[offset_table]), 16);
		code_count=0;
		offset_table+=16;
		for(i=0;i<16;i++){
			code_count+=ojpegdata[t2p->pdf_ojpegdatalength++];
		}
		ojpegdata[offset_ms_l]=((19+code_count)>>8) & 0xff;
		ojpegdata[offset_ms_l+1]=(19+code_count) & 0xff;
		_TIFFmemcpy( &(ojpegdata[t2p->pdf_ojpegdatalength]), 
			&(((unsigned char*)dc)[offset_table]), code_count);
		offset_table+=code_count;
		t2p->pdf_ojpegdatalength+=code_count;
	}
	if(proc==JPEGPROC_BASELINE){
	offset_table=0;
		for(dest=0;dest<table_count;dest++){
			ojpegdata[t2p->pdf_ojpegdatalength++]=0xff;
			ojpegdata[t2p->pdf_ojpegdatalength++]=0xc4;
			offset_ms_l=t2p->pdf_ojpegdatalength;
			t2p->pdf_ojpegdatalength+=2;
			ojpegdata[t2p->pdf_ojpegdatalength] |= 0x10;
			ojpegdata[t2p->pdf_ojpegdatalength++] |=dest & 0x0f;
			_TIFFmemcpy( &(ojpegdata[t2p->pdf_ojpegdatalength]), 
				&(((unsigned char*)ac)[offset_table]), 16);
			code_count=0;
			offset_table+=16;
			for(i=0;i<16;i++){
				code_count+=ojpegdata[t2p->pdf_ojpegdatalength++];
			}	
			ojpegdata[offset_ms_l]=((19+code_count)>>8) & 0xff;
			ojpegdata[offset_ms_l+1]=(19+code_count) & 0xff;
			_TIFFmemcpy( &(ojpegdata[t2p->pdf_ojpegdatalength]), 
				&(((unsigned char*)ac)[offset_table]), code_count);
			offset_table+=code_count;
			t2p->pdf_ojpegdatalength+=code_count;
		}
	}
	if(TIFFNumberOfStrips(input)>1){
		ojpegdata[t2p->pdf_ojpegdatalength++]=0xff;
		ojpegdata[t2p->pdf_ojpegdatalength++]=0xdd;
		ojpegdata[t2p->pdf_ojpegdatalength++]=0x00;
		ojpegdata[t2p->pdf_ojpegdatalength++]=0x04;
		h_samp*=8;
		v_samp*=8;
		ri=(t2p->tiff_width+h_samp-1) / h_samp;
		TIFFGetField(input, TIFFTAG_ROWSPERSTRIP, &rows);
		ri*=(rows+v_samp-1)/v_samp;
		ojpegdata[t2p->pdf_ojpegdatalength++]= (ri>>8) & 0xff;
		ojpegdata[t2p->pdf_ojpegdatalength++]= ri & 0xff;
	}
	ojpegdata[t2p->pdf_ojpegdatalength++]=0xff;
	ojpegdata[t2p->pdf_ojpegdatalength++]=0xda;
	ojpegdata[t2p->pdf_ojpegdatalength++]=0x00;
	ojpegdata[t2p->pdf_ojpegdatalength++]=(6 + 2*t2p->tiff_samplesperpixel);
	ojpegdata[t2p->pdf_ojpegdatalength++]=t2p->tiff_samplesperpixel & 0xff;
	for(i=0;i<t2p->tiff_samplesperpixel;i++){
		ojpegdata[t2p->pdf_ojpegdatalength++]= i & 0xff;
		if(proc==JPEGPROC_BASELINE){
			ojpegdata[t2p->pdf_ojpegdatalength] |= 
				( ( (i>(table_count-1U)) ? (table_count-1U) : i) << 4U) & 0xf0;
			ojpegdata[t2p->pdf_ojpegdatalength++] |= 
				( (i>(table_count-1U)) ? (table_count-1U) : i) & 0x0f;
		} else {
			ojpegdata[t2p->pdf_ojpegdatalength++] =  (i << 4) & 0xf0;
		}
	}
	if(proc==JPEGPROC_BASELINE){
		t2p->pdf_ojpegdatalength++;
		ojpegdata[t2p->pdf_ojpegdatalength++]=0x3f;
		t2p->pdf_ojpegdatalength++;
	} else {
		ojpegdata[t2p->pdf_ojpegdatalength++]= (lp[0] & 0xff);
		t2p->pdf_ojpegdatalength++;
		ojpegdata[t2p->pdf_ojpegdatalength++]= (pt[0] & 0x0f);
	}
	return(1);
}
