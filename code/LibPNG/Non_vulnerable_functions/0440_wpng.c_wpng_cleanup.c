}
static void wpng_cleanup(void)
{
    if (wpng_info.outfile) {
        fclose(wpng_info.outfile);
        wpng_info.outfile = NULL;
    }
    if (wpng_info.infile) {
        fclose(wpng_info.infile);
        wpng_info.infile = NULL;
    }
    if (wpng_info.image_data) {
        free(wpng_info.image_data);
        wpng_info.image_data = NULL;
    }
    if (wpng_info.row_pointers) {
        free(wpng_info.row_pointers);
        wpng_info.row_pointers = NULL;
    }
}
