extern off_t lseek(int fd, off_t offset, int whence)
{
	int absolute = 0;
	switch (whence)
	{
		case SEEK_SET:
		{
			absolute = (int) offset;
			break;
		}
		case SEEK_CUR:
		{
			absolute = osargs_read_ptr((os_f) fd) + (int) offset;
			break;
		}
		case SEEK_END:
		{
			absolute = osargs_read_ext((os_f) fd) + (int) offset;
			break;
		}
	}
	osargs_set_ptr((os_f) fd, absolute);
	return ((off_t) osargs_read_ptr((os_f) fd));
}
