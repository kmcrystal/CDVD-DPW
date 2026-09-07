}
int readpng2_check_sig(uch *sig, int num)
{
    return !png_sig_cmp(sig, 0, num);
}
