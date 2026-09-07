static png_uint_32
get_valid(display *d, png_infop info_ptr)
{
   png_uint_32 flags = png_get_valid(d->png_ptr, info_ptr, (png_uint_32)~0);
   
   {
      png_textp text;
      png_uint_32 ntext = png_get_text(d->png_ptr, info_ptr, &text, NULL);
      while (ntext-- > 0) switch (text[ntext].compression)
      {
         case -1:
            flags |= PNG_INFO_tEXt;
            break;
         case 0:
            flags |= PNG_INFO_zTXt;
            break;
         case 1:
         case 2:
            flags |= PNG_INFO_iTXt;
            break;
         default:
            fprintf(stderr, "%s(%s): unknown text compression %d\n", d->file,
               d->test, text[ntext].compression);
            display_exit(d);
      }
   }
   return flags;
}
