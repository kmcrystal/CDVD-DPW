static toff_t
_tiffSeekProc(thandle_t fd, toff_t off, int whence)
{
	long fpos, size;
	if (GetEOF((short) fd, &size) != noErr)
		return EOF;
	(void) GetFPos((short) fd, &fpos);
	switch (whence) {
	case SEEK_CUR:
		if (off + fpos > size)
			SetEOF((short) fd, off + fpos);
		if (SetFPos((short) fd, fsFromMark, off) != noErr)
			return EOF;
		break;
	case SEEK_END:
		if (off > 0)
			SetEOF((short) fd, off + size);
		if (SetFPos((short) fd, fsFromStart, off + size) != noErr)
			return EOF;
		break;
	case SEEK_SET:
		if (off > size)
			SetEOF((short) fd, off);
		if (SetFPos((short) fd, fsFromStart, off) != noErr)
			return EOF;
		break;
	}
	return (toff_t)(GetFPos((short) fd, &fpos) == noErr ? fpos : EOF);
}
