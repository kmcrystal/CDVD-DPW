static int extend_mem_file (int fd, int size)
{
    void *new_mem;
    int ret;
    if ((new_mem = realloc (buf[fd], size)) == (void *) NULL)
        ret = -1;
    else
    {
        buf[fd] = (char *) new_mem;
        ret = 0;
    }
    return (ret);
}
