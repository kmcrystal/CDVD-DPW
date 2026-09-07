static tsize_t
_tiffReadProc(thandle_t fd, tdata_t buf, tsize_t size)
{
	return (FSRead((short) fd, (long*) &size, (char*) buf) == noErr ?
	    size : (tsize_t) -1);
}
