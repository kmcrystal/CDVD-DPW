static void
store_ensure_image(png_store *ps, png_const_structp pp, int nImages,
   png_size_t cbRow, png_uint_32 cRows)
{
   png_size_t cb = nImages * cRows * (cbRow + 5);
   if (ps->cb_image < cb)
   {
      png_bytep image;
      store_image_free(ps, pp);
      
      image = voidcast(png_bytep, malloc(cb+2));
      if (image == NULL)
      {
         
         if (pp == NULL)
            return;
         png_error(pp, "OOM allocating image buffer");
      }
      
      ++image;
      image[-1] = 0xed;
      image[cb] = 0xfe;
      ps->image = image;
      ps->cb_image = cb;
   }
   



   ps->cb_row = cbRow;
   ps->image_h = cRows;
   





   memset(ps->image, 178, cb);
   
   while (--nImages >= 0)
   {
      png_uint_32 y;
      for (y=0; y<cRows; ++y)
      {
         png_bytep row = store_image_row(ps, pp, nImages, y);
         
         row[-2] = 190;
         row[-1] = 239;
         row[cbRow] = 222;
         row[cbRow+1] = 173;
         row[cbRow+2] = 17;
      }
   }
}
