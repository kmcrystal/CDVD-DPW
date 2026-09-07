static tsize_t 
_tiffReadProc(thandle_t fd, tdata_t buf, tsize_t size)
{
	return (read((int) fd, buf, size));
}
