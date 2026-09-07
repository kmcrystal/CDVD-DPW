 * read and processed--i.e., we now have enough info to finish initializing */
static void rpng2_win_init()
{
    ulg i;
    ulg rowbytes = rpng2_info.rowbytes;
    Trace((stderr, "beginning rpng2_win_init()\n"))
    Trace((stderr, "  rowbytes = %d\n", rpng2_info.rowbytes))
    Trace((stderr, "  width  = %ld\n", rpng2_info.width))
    Trace((stderr, "  height = %ld\n", rpng2_info.height))
    rpng2_info.image_data = (uch *)malloc(rowbytes * rpng2_info.height);
    if (!rpng2_info.image_data) {
        readpng2_cleanup(&rpng2_info);
        return;
    }
    rpng2_info.row_pointers = (uch **)malloc(rpng2_info.height * sizeof(uch *));
    if (!rpng2_info.row_pointers) {
        free(rpng2_info.image_data);
        rpng2_info.image_data = NULL;
        readpng2_cleanup(&rpng2_info);
        return;
    }
    for (i = 0;  i < rpng2_info.height;  ++i)
        rpng2_info.row_pointers[i] = rpng2_info.image_data + i*rowbytes;




    if (rpng2_win_create_window()) {
        readpng2_cleanup(&rpng2_info);
        return;
    }
    rpng2_info.state = kWindowInit;
}
