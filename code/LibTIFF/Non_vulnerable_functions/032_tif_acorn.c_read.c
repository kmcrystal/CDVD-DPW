extern int read(int fd, char *buf, int nbytes)
{
	
	return (nbytes - osgbpb_read((os_f) fd, (byte*) buf, nbytes));
}
