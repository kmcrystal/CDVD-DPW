int mfs_close (int fd)
{
    int ret; 
    if (fds[fd] == -1)  
    {
        ret = -1;
        errno = EBADF;
    }
    else
    {
        fds[fd] = -1;
        ret = 0;
    }
    return (ret);
}
