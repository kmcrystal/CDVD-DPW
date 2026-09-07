 */
static void initimage(Image *image, png_uint_32 opts, const char *file_name,
   int stride_extra)
{
   freeimage(image);
   memset(&image->image, 0, sizeof image->image);
   image->opts = opts;
   image->file_name = file_name;
   image->stride_extra = stride_extra;
}
