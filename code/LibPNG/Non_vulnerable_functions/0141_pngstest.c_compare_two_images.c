static int
compare_two_images(Image *a, Image *b, int via_linear,
   png_const_colorp background)
{
   ptrdiff_t stridea = a->stride;
   ptrdiff_t strideb = b->stride;
   png_const_bytep rowa = a->buffer+16;
   png_const_bytep rowb = b->buffer+16;
   const png_uint_32 width = a->image.width;
   const png_uint_32 height = a->image.height;
   const png_uint_32 formata = a->image.format;
   const png_uint_32 formatb = b->image.format;
   const unsigned int a_sample = PNG_IMAGE_SAMPLE_SIZE(formata);
   const unsigned int b_sample = PNG_IMAGE_SAMPLE_SIZE(formatb);
   int alpha_added, alpha_removed;
   int bchannels;
   int btoa[4];
   png_uint_32 y;
   Transform tr;
   
   if (width != b->image.width || height != b->image.height)
      return logerror(a, a->file_name, ": width x height changed: ",
         b->file_name);
   
   transform_from_formats(&tr, a, b, background, via_linear);
   
   if (!(formata & PNG_FORMAT_FLAG_COLORMAP) &&
      (formata & PNG_FORMAT_FLAG_LINEAR))
      stridea *= 2;
   if (!(formatb & PNG_FORMAT_FLAG_COLORMAP) &&
      (formatb & PNG_FORMAT_FLAG_LINEAR))
      strideb *= 2;
   if (stridea < 0) rowa += (height-1) * (-stridea);
   if (strideb < 0) rowb += (height-1) * (-strideb);
   




   if (formata & formatb & PNG_FORMAT_FLAG_COLORMAP)
   {
      
      png_const_bytep ppa, ppb;
      int match;
      png_byte in_use[256], amax = 0, bmax = 0;
      memset(in_use, 0, sizeof in_use);
      ppa = rowa;
      ppb = rowb;
      



      for (y=0, match=1; y<height && match; ++y, ppa += stridea, ppb += strideb)
      {
         png_uint_32 x;
         for (x=0; x<width; ++x)
         {
            png_byte bval = ppb[x];
            png_byte aval = ppa[x];
            if (bval > bmax)
               bmax = bval;
            if (bval != aval)
               match = 0;
            in_use[aval] = 1;
            if (aval > amax)
               amax = aval;
         }
      }
      
      if (match)
      {
         


         png_const_bytep a_cmap = (png_const_bytep)a->colormap;
         png_const_bytep b_cmap = (png_const_bytep)b->colormap;
         int result = 1; 
         
         tr.is_palette = 1;
         for (y=0; y<256; ++y, a_cmap += a_sample, b_cmap += b_sample)
            if (in_use[y])
         {
            




            if (y >= a->image.colormap_entries)
            {
               if ((a->opts & ACCUMULATE) == 0)
               {
                  char pindex[9];
                  sprintf(pindex, "%lu[%lu]", (unsigned long)y,
                     (unsigned long)a->image.colormap_entries);
                  logerror(a, a->file_name, ": bad pixel index: ", pindex);
               }
               result = 0;
            }
            else if (y >= b->image.colormap_entries)
            {
               if ((a->opts & ACCUMULATE) == 0)
                  {
                  char pindex[9];
                  sprintf(pindex, "%lu[%lu]", (unsigned long)y,
                     (unsigned long)b->image.colormap_entries);
                  logerror(b, b->file_name, ": bad pixel index: ", pindex);
                  }
               result = 0;
            }
            
            else if (!cmppixel(&tr, a_cmap, b_cmap, 0, y))
               result = 0;
         }
         
         if (a->opts & ACCUMULATE)
         {
            tr.error_ptr[0] = tr.error[0];
            tr.error_ptr[1] = tr.error[1];
            tr.error_ptr[2] = tr.error[2];
            tr.error_ptr[3] = tr.error[3];
            result = 1; 
         }
         return result;
      }
      



      else if ((a->opts & ACCUMULATE) == 0)
      {
         


         if (amax >= a->image.colormap_entries)
         {
            char pindex[9];
            sprintf(pindex, "%d[%lu]", amax,
               (unsigned long)a->image.colormap_entries);
            return logerror(a, a->file_name, ": bad pixel index: ", pindex);
         }
         else if (bmax >= b->image.colormap_entries)
         {
            char pindex[9];
            sprintf(pindex, "%d[%lu]", bmax,
               (unsigned long)b->image.colormap_entries);
            return logerror(b, b->file_name, ": bad pixel index: ", pindex);
         }
      }
   }
   














   if (!(((formata ^ formatb) & PNG_FORMAT_FLAG_LINEAR) |
      (formata & (formatb ^ PNG_FORMAT_FLAG_COLOR) & PNG_FORMAT_FLAG_COLOR)))
   {
      
      const png_uint_32 alpha_changed = (formata ^ formatb) &
         PNG_FORMAT_FLAG_ALPHA;
      


      alpha_removed = (formata & alpha_changed) != 0;
      
      alpha_added = (formatb & alpha_changed) != 0;
      




      {
         int i;
         png_byte aloc[4];
         png_byte bloc[4];
         





         (void)component_loc(aloc, formata);
         bchannels = component_loc(bloc, formatb);
         
         for (i=0; i<4; ++i) if (bloc[i] < 4)
            btoa[bloc[i]] = aloc[i]; 
         if (alpha_added)
            alpha_added = bloc[0]; 
         else
            alpha_added = 4; 
         if (alpha_removed)
            alpha_removed = aloc[0]; 
         else
            alpha_removed = 4;
      }
   }
   else
   {
      


      bchannels = 0;
      alpha_removed = alpha_added = 4;
      btoa[3] = btoa[2] = btoa[1] = btoa[0] = 4; 
   }
   for (y=0; y<height; ++y, rowa += stridea, rowb += strideb)
   {
      png_const_bytep ppa, ppb;
      png_uint_32 x;
      for (x=0, ppa=rowa, ppb=rowb; x<width; ++x)
      {
         png_const_bytep psa, psb;
         if (formata & PNG_FORMAT_FLAG_COLORMAP)
            psa = (png_const_bytep)a->colormap + a_sample * *ppa++;
         else
            psa = ppa, ppa += a_sample;
         if (formatb & PNG_FORMAT_FLAG_COLORMAP)
            psb = (png_const_bytep)b->colormap + b_sample * *ppb++;
         else
            psb = ppb, ppb += b_sample;
         
         if (bchannels)
         {
            





            if (formatb & PNG_FORMAT_FLAG_LINEAR) 
            {
               png_const_uint_16p pua = aligncastconst(png_const_uint_16p, psa);
               png_const_uint_16p pub = aligncastconst(png_const_uint_16p, psb);
               switch (bchannels)
               {
                  case 4:
                     if (pua[btoa[3]] != pub[3]) break;
                  case 3:
                     if (pua[btoa[2]] != pub[2]) break;
                  case 2:
                     if (pua[btoa[1]] != pub[1]) break;
                  case 1:
                     if (pua[btoa[0]] != pub[0]) break;
                     if (alpha_added != 4 && pub[alpha_added] != 65535) break;
                     continue; 
                  default:
                     break; 
               }
            }
            else if (alpha_removed == 4 || psa[alpha_removed] == 255)
            {
               switch (bchannels)
               {
                  case 4:
                     if (psa[btoa[3]] != psb[3]) break;
                  case 3:
                     if (psa[btoa[2]] != psb[2]) break;
                  case 2:
                     if (psa[btoa[1]] != psb[1]) break;
                  case 1:
                     if (psa[btoa[0]] != psb[0]) break;
                     if (alpha_added != 4 && psb[alpha_added] != 255) break;
                     continue; 
                  default:
                     break; 
               }
            }
         }
         


         if (!cmppixel(&tr, psa, psb, x, y) && (a->opts & KEEP_GOING) == 0)
            return 0; 
      }
   }
   
   if (a->opts & ACCUMULATE)
   {
      tr.error_ptr[0] = tr.error[0];
      tr.error_ptr[1] = tr.error[1];
      tr.error_ptr[2] = tr.error[2];
      tr.error_ptr[3] = tr.error[3];
   }
   return 1;
}
