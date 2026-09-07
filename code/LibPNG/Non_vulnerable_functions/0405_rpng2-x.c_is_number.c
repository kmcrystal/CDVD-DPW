} 
static int is_number(char *p)
{
    while (*p) {
        if (!isdigit(*p))
            return FALSE;
        ++p;
    }
    return TRUE;
}
