}
static void rpng2_win_cleanup()
{
    if (bg_image && bg_data) {
        free(bg_data);
        bg_data = NULL;
    }
    if (rpng2_info.image_data) {
        free(rpng2_info.image_data);
        rpng2_info.image_data = NULL;
    }
    if (rpng2_info.row_pointers) {
        free(rpng2_info.row_pointers);
        rpng2_info.row_pointers = NULL;
    }
    if (dib) {
        free(dib);
        dib = NULL;
    }
}
