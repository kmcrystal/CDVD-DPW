static void readpng2_error_handler(png_structp png_ptr, png_const_charp msg);
void readpng2_version_info(void)
{
    fprintf(stderr, "   Compiled with libpng %s; using libpng %s\n",
      PNG_LIBPNG_VER_STRING, png_libpng_ver);
    fprintf(stderr, "   and with zlib %s; using zlib %s.\n",
      ZLIB_VERSION, zlib_version);
}
