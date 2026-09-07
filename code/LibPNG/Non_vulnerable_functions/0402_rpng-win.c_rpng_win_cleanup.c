}
static void rpng_win_cleanup()
{
    if (image_data) {
        free(image_data);
        image_data = NULL;
    }
    if (dib) {
        free(dib);
        dib = NULL;
    }
}
