static void
store_image_free(png_store *ps, png_const_structp pp)
{
   if (ps->image != NULL)
   {
      png_bytep image = ps->image;
      if (image[-1] != 0xed || image[ps->cb_image] != 0xfe)
      {
         if (pp != NULL)
            png_error(pp, "png_store image overwrite (1)");
         else
            store_log(ps, NULL, "png_store image overwrite (2)", 1);
      }
      ps->image = NULL;
      ps->cb_image = 0;
      --image;
      free(image);
   }
}
