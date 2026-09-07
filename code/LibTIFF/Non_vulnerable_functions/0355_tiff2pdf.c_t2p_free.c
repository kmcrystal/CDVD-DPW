 */
void t2p_free(T2P* t2p)
{
	int i = 0;
	if (t2p != NULL) {
		if(t2p->pdf_xrefoffsets != NULL){
			_TIFFfree( (tdata_t) t2p->pdf_xrefoffsets);
		}
		if(t2p->tiff_pages != NULL){
			_TIFFfree( (tdata_t) t2p->tiff_pages);
		}
		for(i=0;i<t2p->tiff_pagecount;i++){
			if(t2p->tiff_tiles[i].tiles_tiles != NULL){
				_TIFFfree( (tdata_t) t2p->tiff_tiles[i].tiles_tiles);
			}
		}
		if(t2p->tiff_tiles != NULL){
			_TIFFfree( (tdata_t) t2p->tiff_tiles);
		}
		if(t2p->pdf_palette != NULL){
			_TIFFfree( (tdata_t) t2p->pdf_palette);
		}
#ifdef OJPEG_SUPPORT
		if(t2p->pdf_ojpegdata != NULL){
			_TIFFfree( (tdata_t) t2p->pdf_ojpegdata);
		}
#endif
		_TIFFfree( (tdata_t) t2p );
	}
	return;
}
