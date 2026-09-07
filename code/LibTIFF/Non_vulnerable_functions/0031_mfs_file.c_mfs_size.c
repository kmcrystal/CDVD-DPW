int mfs_size (int fd)
{
    int ret;
    if (fds[fd] == -1)  
    {
        ret = -1;
        errno = EBADF;
    }
    else
        ret = buf_size[fd];
    return (ret);
}
