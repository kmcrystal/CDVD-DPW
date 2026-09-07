int mfs_map (int fd, char **addr, size_t *len)
{
    int ret; 
    if (fds[fd] == -1)  
    {
        ret = -1;
        errno = EBADF;
    }
    else
    {
        *addr = buf[fd];
        *len = buf_size[fd];
        ret = 0;
    }
    return (ret);
}
