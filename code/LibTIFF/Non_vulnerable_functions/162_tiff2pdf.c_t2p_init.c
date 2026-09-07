 */
T2P* t2p_init()
{
	T2P* t2p = (T2P*) _TIFFmalloc(sizeof(T2P));
	if(t2p==NULL){
		TIFFError(
			TIFF2PDF_MODULE, 
			"Can't allocate %lu bytes of memory for t2p_init", 
			(unsigned long) sizeof(T2P));
		return( (T2P*) NULL );
	}
	_TIFFmemset(t2p, 0x00, sizeof(T2P));
	t2p->pdf_majorversion=1;
	t2p->pdf_minorversion=1;
	t2p->pdf_defaultxres=300.0;
	t2p->pdf_defaultyres=300.0;
	t2p->pdf_defaultpagewidth=612.0;
	t2p->pdf_defaultpagelength=792.0;
	t2p->pdf_xrefcount=3; 
	return(t2p);
}
