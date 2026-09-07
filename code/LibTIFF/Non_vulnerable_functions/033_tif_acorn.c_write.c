extern int write(int fd, const char *buf, int nbytes)
{
	
	return (nbytes - osgbpb_write((os_f) fd, (const byte*) buf, nbytes));
}
