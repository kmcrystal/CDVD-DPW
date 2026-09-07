static void
transform_from_formats(Transform *result, Image *in_image,
   const Image *out_image, png_const_colorp background, int via_linear)
{
   png_uint_32 in_format, out_format;
   png_uint_32 in_base, out_base;
   memset(result, 0, sizeof *result);
   
   result->in_image = in_image;
   result->out_image = out_image;
   in_format = in_image->image.format;
   out_format = out_image->image.format;
   if (in_format & PNG_FORMAT_FLAG_LINEAR)
      result->in_opaque = 65535;
   else
      result->in_opaque = 255;
   result->output_8bit = (out_format & PNG_FORMAT_FLAG_LINEAR) == 0;
   result->is_palette = 0; 
   result->accumulate = (in_image->opts & ACCUMULATE) != 0;
   
   result->in_gp = get_pixel(in_format);
   result->out_gp = get_pixel(out_format);
   
   in_format &= BASE_FORMATS | PNG_FORMAT_FLAG_COLORMAP;
   in_base = in_format & BASE_FORMATS;
   out_format &= BASE_FORMATS | PNG_FORMAT_FLAG_COLORMAP;
   out_base = out_format & BASE_FORMATS;
   if (via_linear)
   {
      
      if (out_format & (PNG_FORMAT_FLAG_LINEAR|PNG_FORMAT_FLAG_COLORMAP))
      {
         fprintf(stderr, "internal transform via linear error 0x%x->0x%x\n",
            in_format, out_format);
         exit(1);
      }
      result->transform = gpc_fn[in_base][out_base | PNG_FORMAT_FLAG_LINEAR];
      result->from_linear = gpc_fn[out_base | PNG_FORMAT_FLAG_LINEAR][out_base];
      result->error_ptr = gpc_error_via_linear[in_format][out_format];
   }
   else if (~in_format & out_format & PNG_FORMAT_FLAG_COLORMAP)
   {
      



      result->transform = gpc_fn[in_base][out_base];
      result->from_linear = NULL;
      result->error_ptr = gpc_error_to_colormap[in_base][out_base];
   }
   else
   {
      





      if (in_format & out_format & PNG_FORMAT_FLAG_COLORMAP)
         result->transform = gpc_fn_colormapped[in_base][out_base];
      else
         result->transform = gpc_fn[in_base][out_base];
      result->from_linear = NULL;
      result->error_ptr = gpc_error[in_format][out_format];
   }
   



   result->background = NULL; 
   
   if (in_format & ~out_format & PNG_FORMAT_FLAG_ALPHA)
   {
      






      result->background = &result->background_color;
      if (out_format & PNG_FORMAT_FLAG_LINEAR || via_linear)
      {
         if (out_format & PNG_FORMAT_FLAG_COLORMAP)
         {
            result->background_color.ir =
               result->background_color.ig =
               result->background_color.ib = 0;
            result->background_color.dr =
               result->background_color.dg =
               result->background_color.db = 0;
         }
         else
         {
            result->background_color.ir =
               result->background_color.ig =
               result->background_color.ib = BUFFER_INIT8 * 257;
            result->background_color.dr =
               result->background_color.dg =
               result->background_color.db = 0;
         }
      }
      else 
      {
         if (background != NULL)
         {
            if (out_format & PNG_FORMAT_FLAG_COLOR)
            {
               result->background_color.ir = background->red;
               result->background_color.ig = background->green;
               result->background_color.ib = background->blue;
               


               result->background_color.dr = sRGB_to_d[background->red];
               result->background_color.dg = sRGB_to_d[background->green];
               result->background_color.db = sRGB_to_d[background->blue];
            }
            else 
            {
               result->background_color.ir =
                  result->background_color.ig =
                  result->background_color.ib = background->green;
               


               result->background_color.dr =
                  result->background_color.dg =
                  result->background_color.db = sRGB_to_d[background->green];
            }
         }
         else if ((out_format & PNG_FORMAT_FLAG_COLORMAP) == 0)
         {
            result->background_color.ir =
               result->background_color.ig =
               result->background_color.ib = BUFFER_INIT8;
            


            result->background_color.dr =
               result->background_color.dg =
               result->background_color.db = sRGB_to_d[BUFFER_INIT8];
         }
         



         else
            result->background = NULL;
      }
   }
   if (result->background == NULL)
   {
      result->background_color.ir =
         result->background_color.ig =
         result->background_color.ib = -1; 
      result->background_color.dr =
         result->background_color.dg =
         result->background_color.db = 1E30; 
   }
   
   result->error[0] = result->error_ptr[0];
   result->error[1] = result->error_ptr[1];
   result->error[2] = result->error_ptr[2];
   result->error[3] = result->error_ptr[3];
}
