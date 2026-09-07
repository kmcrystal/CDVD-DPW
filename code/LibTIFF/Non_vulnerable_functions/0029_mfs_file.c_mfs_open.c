int mfs_open (void *buffer, int size, char *mode)
{
    int ret, i;
    void *tmp;
    if (library_init_done == FALSE)
    {
        mem_init ();
        library_init_done = TRUE;
    }
    ret = -1;
    
    for (i = 0; i < MAX_BUFFS; i++)
    {
        if (fds[i] == -1)
        {
            ret = i;
            break;
        }
    }
    if (i == MAX_BUFFS)     
    {
        ret = -1;
        errno = EMFILE;
    }
    if (ret >= 0 && *mode == 'r')
    {
        if (buffer == (void *)NULL)
        {
            ret = -1;
            errno = EINVAL;
        }
        else
        {
            buf[ret] = (char *)buffer;
            buf_size[ret] = size;
            buf_off[ret] = 0;
        }
    }
    else if (ret >= 0 && *mode == 'w')
    {
        if (buffer != (void *)NULL)
        {
            ret = -1;
            errno = EINVAL;
        }
        else
        {
            tmp = malloc (0);   
            if (tmp == (void *)NULL)
            {
                ret = -1;
                errno = EDQUOT;
            }
            else
            {
                buf[ret] = (char *)tmp;
                buf_size[ret] = 0;
                buf_off[ret] = 0;
            }
        }
    }
    else if (ret >= 0 && *mode == 'a')
    {
        if (buffer == (void *) NULL)    
        {
            tmp = malloc (0);   
            if (tmp == (void *)NULL)
            {
                ret = -1;
                errno = EDQUOT;
            }
            else
            {
                buf[ret] = (char *)tmp;
                buf_size[ret] = 0;
                buf_off[ret] = 0;
            }
        }
        else                            
        {
            buf[ret] = (char *)buffer;
            buf_size[ret] = size;
            buf_off[ret] = 0;
        }
    }
    else        
    {
        ret = -1;
        errno = EINVAL;
    }
    if (ret != -1)
    {
        fds[ret] = 0;
        buf_mode[ret] = *mode;
    }
    return (ret);
}
