static void mem_init ()
{
    int i;
    for (i = 0; i < MAX_BUFFS; i++)
    {
        fds[i] = -1;
        buf[i] = (char *)NULL;
        buf_size[i] = 0;
        buf_off[i] = 0;
    }
}
