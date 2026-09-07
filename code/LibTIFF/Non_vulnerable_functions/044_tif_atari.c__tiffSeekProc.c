static toff_t
_tiffSeekProc(thandle_t fd, off_t off, int whence)
{
	char buf[256];
	long current_off, expected_off, new_off;
	if (whence == SEEK_END || off <= 0)
		return Fseek(off, (int) fd, whence);
	current_off = Fseek(0, (int) fd, SEEK_CUR); 
	if (whence == SEEK_SET)
		expected_off = off;
	else
		expected_off = off + current_off;
	new_off = Fseek(off, (int) fd, whence);
	if (new_off == expected_off)
		return new_off;
	
	if (new_off < 0)            
		new_off = Fseek(0, (int) fd, SEEK_END); 
	_TIFFmemset(buf, 0, sizeof(buf));
	while (expected_off > new_off) {
		off = expected_off - new_off;
		if (off > sizeof(buf))
			off = sizeof(buf);
		if ((current_off = Fwrite((int) fd, off, buf)) != off)
			return (current_off > 0) ?
			    new_off + current_off : new_off;
		new_off += off;
	}
	return new_off;
}
