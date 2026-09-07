static void
resetimage(Image *image)
{
   if (image->input_file != NULL)
      rewind(image->input_file);
}
