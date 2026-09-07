void PNGCBAPI
count_filters(png_structp png_ptr, png_row_infop row_info, png_bytep data)
{
   if (png_ptr != NULL && row_info != NULL)
      ++filters_used[*(data - 1)];
}
