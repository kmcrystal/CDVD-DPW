static toff_t
_tiffSeekProc(thandle_t fd, toff_t off, int whence)
{
	DWORD dwMoveMethod, dwMoveHigh;
        
        if( off == 0xFFFFFFFF )
            return 0xFFFFFFFF;
	switch(whence)
	{
	case SEEK_SET:
		dwMoveMethod = FILE_BEGIN;
		break;
	case SEEK_CUR:
		dwMoveMethod = FILE_CURRENT;
		break;
	case SEEK_END:
		dwMoveMethod = FILE_END;
		break;
	default:
		dwMoveMethod = FILE_BEGIN;
		break;
	}
        dwMoveHigh = 0;
	return ((toff_t)SetFilePointer(fd, (LONG) off, (PLONG)&dwMoveHigh,
                                       dwMoveMethod));
}
