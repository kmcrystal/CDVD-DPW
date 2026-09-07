static int 
t2p_closeproc(thandle_t handle)
{ 
	T2P *t2p = (T2P*) handle;
	return fclose(t2p->outputfile);
}
