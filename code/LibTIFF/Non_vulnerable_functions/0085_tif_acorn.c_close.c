extern int close(int fd)
{
	return ((int) xosfind_close((os_f) fd));
}
