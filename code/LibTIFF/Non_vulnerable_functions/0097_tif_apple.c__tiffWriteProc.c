static tsize_t
_tiffWriteProc(thandle_t fd, tdata_t buf, tsize_t size)
{
	return (FSWrite((short) fd, (long*) &size, (char*) buf) == noErr ?
	    size : (tsize_t) -1);
}
