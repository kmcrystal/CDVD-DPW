static int
compare_read(struct display *dp, int applied_transforms)
{
   
   size_t rowbytes;
   png_uint_32 width, height;
   int bit_depth, color_type;
   int interlace_method, compression_method, filter_method;
   const char *e = NULL;
   png_get_IHDR(dp->read_pp, dp->read_ip, &width, &height, &bit_depth,
      &color_type, &interlace_method, &compression_method, &filter_method);
#  define C(item) if (item != dp->item) \
      display_log(dp, APP_WARNING, "IHDR " #item "(%lu) changed to %lu",\
         (unsigned long)dp->item, (unsigned long)item), e = #item
   
   C(width);
   C(height);
   C(bit_depth);
   C(color_type);
   C(interlace_method);
   C(compression_method);
   C(filter_method);
   
   if (e)
      display_log(dp, APP_ERROR, "IHDR changed (%s)", e);
   


   {
      unsigned long chunks =
         png_get_valid(dp->read_pp, dp->read_ip, 0xffffffff);
      if (chunks != dp->chunks)
         display_log(dp, APP_FAIL, "PNG chunks changed from 0x%lx to 0x%lx",
            (unsigned long)dp->chunks, chunks);
   }
   
   rowbytes = png_get_rowbytes(dp->read_pp, dp->read_ip);
   


   if (rowbytes != dp->original_rowbytes)
      display_log(dp, APP_ERROR, "PNG rowbytes changed from %lu to %lu",
         (unsigned long)dp->original_rowbytes, (unsigned long)rowbytes);
   


   {
      png_bytepp rows = png_get_rows(dp->read_pp, dp->read_ip);
      unsigned int mask;  
      if (bit_depth < 8)
      {
         



         mask = 0xff & (0xff00 >> ((bit_depth * width) & 7));
      }
      else
         mask = 0;
      if (rows == NULL)
         display_log(dp, LIBPNG_BUG, "png_get_rows returned NULL");
      if ((applied_transforms & PNG_TRANSFORM_SHIFT) == 0 ||
         (dp->active_transforms & PNG_TRANSFORM_SHIFT) == 0 ||
         color_type == PNG_COLOR_TYPE_PALETTE)
      {
         unsigned long y;
         for (y=0; y<height; ++y)
         {
            png_bytep row = rows[y];
            png_bytep orig = dp->original_rows[y];
            if (memcmp(row, orig, rowbytes-(mask != 0)) != 0 || (mask != 0 &&
               ((row[rowbytes-1] & mask) != (orig[rowbytes-1] & mask))))
            {
               size_t x;
               
               for (x=0; x<rowbytes-1; ++x) if (row[x] != orig[x])
                  break;
               display_log(dp, APP_FAIL,
                  "byte(%lu,%lu) changed 0x%.2x -> 0x%.2x",
                  (unsigned long)x, (unsigned long)y, orig[x], row[x]);
               return 0; 
            }
         }
      }
      else
      {
         unsigned long y;
         int bpp;   
         
         png_byte sig_bits[8];
         png_color_8p sBIT;
         if (png_get_sBIT(dp->read_pp, dp->read_ip, &sBIT) != PNG_INFO_sBIT)
            display_log(dp, INTERNAL_ERROR,
               "active shift transform but no sBIT in file");
         switch (color_type)
         {
            case PNG_COLOR_TYPE_GRAY:
               sig_bits[0] = sBIT->gray;
               bpp = bit_depth;
               break;
            case PNG_COLOR_TYPE_GA:
               sig_bits[0] = sBIT->gray;
               sig_bits[1] = sBIT->alpha;
               bpp = 2 * bit_depth;
               break;
            case PNG_COLOR_TYPE_RGB:
               sig_bits[0] = sBIT->red;
               sig_bits[1] = sBIT->green;
               sig_bits[2] = sBIT->blue;
               bpp = 3 * bit_depth;
               break;
            case PNG_COLOR_TYPE_RGBA:
               sig_bits[0] = sBIT->red;
               sig_bits[1] = sBIT->green;
               sig_bits[2] = sBIT->blue;
               sig_bits[3] = sBIT->alpha;
               bpp = 4 * bit_depth;
               break;
            default:
               display_log(dp, LIBPNG_ERROR, "invalid colour type %d",
                  color_type);
               
               bpp = 0;
               break;
         }
         {
            int b;
            for (b=0; 8*b<bpp; ++b)
            {
               



               if (sig_bits[b] == 0 || sig_bits[b] > bit_depth)
                  display_log(dp, LIBPNG_BUG,
                     "invalid sBIT[%u]  value %d returned for PNG bit depth %d",
                     b, sig_bits[b], bit_depth);
            }
         }
         if (bpp < 8 && bpp != bit_depth)
         {
            


            display_log(dp, INTERNAL_ERROR, "invalid bpp %u for bit_depth %u",
               bpp, bit_depth);
         }
         switch (bit_depth)
         {
            int b;
            case 16: 
               for (b = (bpp >> 4); b > 0; )
               {
                  unsigned int sig = (unsigned int)(0xffff0000 >> sig_bits[b]);
                  sig_bits[2*b+1] = (png_byte)sig;
                  sig_bits[2*b+0] = (png_byte)(sig >> 8); 
               }
               break;
            case 8: 
               for (b=0; b*8 < bpp; ++b)
                  sig_bits[b] = (png_byte)(0xff00 >> sig_bits[b]);
               break;
            case 1: 
               
               sig_bits[0] = 0xff;
               break;
            case 2: 
               
               b = 0x3 & ((0x3<<2) >> sig_bits[0]);
               b |= b << 2;
               b |= b << 4;
               sig_bits[0] = (png_byte)b;
               break;
            case 4: 
               
               b = 0xf & ((0xf << 4) >> sig_bits[0]);
               b |= b << 4;
               sig_bits[0] = (png_byte)b;
               break;
            default:
               display_log(dp, LIBPNG_BUG, "invalid bit depth %d", bit_depth);
               break;
         }
         


         bpp = (bpp+7) >> 3;
         
         if (mask != 0)
         {
            mask &= sig_bits[0];
            if (bpp != 1 || mask == 0)
               display_log(dp, INTERNAL_ERROR, "mask calculation error %u, %u",
                  bpp, mask);
         }
         for (y=0; y<height; ++y)
         {
            png_bytep row = rows[y];
            png_bytep orig = dp->original_rows[y];
            unsigned long x;
            for (x=0; x<(width-(mask!=0)); ++x)
            {
               int b;
               for (b=0; b<bpp; ++b)
               {
                  if ((*row++ & sig_bits[b]) != (*orig++ & sig_bits[b]))
                  {
                     display_log(dp, APP_FAIL,
                        "significant bits at (%lu[%u],%lu) changed %.2x->%.2x",
                        x, b, y, orig[-1], row[-1]);
                     return 0;
                  }
               }
            }
            if (mask != 0 && (*row & mask) != (*orig & mask))
            {
               display_log(dp, APP_FAIL,
                  "significant bits at (%lu[end],%lu) changed", x, y);
               return 0;
            }
         } 
      }
   }
   return 1; 
}
