int mfs_write (int fd, void *clnt_buf, int size)
{
    int ret;
    if (fds[fd] == -1 || buf_mode[fd] == 'r')
    {
        
        ret = -1;
        errno = EBADF;
    }
    else if (buf_mode[fd] == 'w')
    {
        
        if (buf_off[fd] + size > buf_size[fd])
        {       
            extend_mem_file (fd, buf_off[fd] + size);
            buf_size[fd] = (buf_off[fd] + size);
        }
        memcpy ((buf[fd] + buf_off[fd]), clnt_buf, size);
        buf_off[fd] = buf_off[fd] + size;
        ret = size;
    }
    else
    {
        
        if (buf_off[fd] != buf_size[fd])
            buf_off[fd] = buf_size[fd];
        extend_mem_file (fd, buf_off[fd] + size);
        buf_size[fd] += size;
        memcpy ((buf[fd] + buf_off[fd]), clnt_buf, size);
        buf_off[fd] = buf_off[fd] + size;
        ret = size;
    }
    return (ret);
}
