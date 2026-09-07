static uint64 
t2p_seekproc(thandle_t handle, uint64 offset, int whence) 
{ 
	T2P *t2p = (T2P*) handle;
	if (t2p->outputdisable <= 0 && t2p->outputfile)
		return _TIFF_fseek_f(t2p->outputfile, (_TIFF_off_t) offset, whence);
	return offset;
}
