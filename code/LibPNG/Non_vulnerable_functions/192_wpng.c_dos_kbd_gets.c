#ifdef DOS_OS2_W32
static char *dos_kbd_gets(char *buf, int len)
{
    int ch, count=0;
    do {
        buf[count++] = ch = getche();
    } while (ch != '\r' && count < len-1);
    buf[count--] = '\0';        
    if (buf[count] == '\r')     
        buf[count] = '\n';
    fprintf(stderr, "\n");      
    fflush(stderr);
    return buf;
}
