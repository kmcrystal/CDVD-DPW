static void PNGCBAPI
write_function(png_structp pp, png_bytep data, png_size_t size)
{
   buffer_write(get_dp(pp), get_buffer(pp), data, size);
}
