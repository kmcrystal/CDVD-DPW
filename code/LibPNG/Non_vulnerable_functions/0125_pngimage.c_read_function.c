static void PNGCBAPI
read_function(png_structp pp, png_bytep data, png_size_t size)
{
   buffer_read(get_dp(pp), get_buffer(pp), data, size);
}
