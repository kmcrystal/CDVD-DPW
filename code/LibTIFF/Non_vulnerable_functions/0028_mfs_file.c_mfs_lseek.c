int mfs_lseek (int fd, int offset, int whence)
{
    int ret;
    long test_off;
    if (fds[fd] == -1)  
    {
        ret = -1;
        errno = EBADF;
    }
    else if (offset < 0 && whence == SEEK_SET)
    {
        ret = -1;
        errno = EINVAL;
    }
    else
    {
        switch (whence)
        {
            case SEEK_SET:
                if (offset > buf_size[fd])
                    extend_mem_file (fd, offset);
                buf_off[fd] = offset;
                ret = offset;
                break;
            case SEEK_CUR:
                test_off = buf_off[fd] + offset;
                if (test_off < 0)
                {
                    ret = -1;
                    errno = EINVAL;
                }
                else
                {
                    if (test_off > buf_size[fd])
                        extend_mem_file (fd, test_off);
                    buf_off[fd] = test_off;
                    ret = test_off;
                }
                break;
            case SEEK_END:
                test_off = buf_size[fd] + offset;
                if (test_off < 0)
                {
                    ret = -1;
                    errno = EINVAL;
                }
                else
                {
                    if (test_off > buf_size[fd])
                        extend_mem_file (fd, test_off);
                    buf_off[fd] = test_off;
                    ret = test_off;
                }
                break;
            default:
                errno = EINVAL;
                ret = -1;
                break;
        }
    }
    return (ret);
}   
