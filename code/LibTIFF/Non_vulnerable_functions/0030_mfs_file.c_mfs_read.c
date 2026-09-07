int mfs_read (int fd, void *clnt_buf, int size)
{
    int ret;
    if (fds[fd] == -1 || buf_mode[fd] != 'r')
    {
        
        ret = -1;
        errno = EBADF;
    }
    else if (buf_off[fd] + size > buf_size[fd])
    {
        ret = 0;        
    }
    else
    {
        memcpy (clnt_buf, (void *) (buf[fd] + buf_off[fd]), size);
        buf_off[fd] = buf_off[fd] + size;
        ret = size;
    }
    return (ret);
}
