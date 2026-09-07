static tmsize_t 
t2p_writeproc(thandle_t handle, tdata_t data, tmsize_t size) 
{
	T2P *t2p = (T2P*) handle;
	if (t2p->outputdisable <= 0 && t2p->outputfile) {
		tsize_t written = fwrite(data, 1, size, t2p->outputfile);
		t2p->outputwritten += written;
		return written;
	}
	return size; 
}
