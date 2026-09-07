static void (*
get_pixel(png_uint_32 format))(Pixel *p, png_const_voidp pb)
{
   



   if (format & PNG_FORMAT_FLAG_LINEAR)
   {
      if (format & PNG_FORMAT_FLAG_COLOR)
      {
#        ifdef PNG_FORMAT_BGR_SUPPORTED
            if (format & PNG_FORMAT_FLAG_BGR)
            {
               if (format & PNG_FORMAT_FLAG_ALPHA)
               {
#                 ifdef PNG_FORMAT_AFIRST_SUPPORTED
                     if (format & PNG_FORMAT_FLAG_AFIRST)
                        return gp_abgr16;
                     else
#                 endif
                     return gp_bgra16;
               }
               else
                  return gp_bgr16;
            }
            else
#        endif
         {
            if (format & PNG_FORMAT_FLAG_ALPHA)
            {
#              ifdef PNG_FORMAT_AFIRST_SUPPORTED
                  if (format & PNG_FORMAT_FLAG_AFIRST)
                     return gp_argb16;
                  else
#              endif
                  return gp_rgba16;
            }
            else
               return gp_rgb16;
         }
      }
      else
      {
         if (format & PNG_FORMAT_FLAG_ALPHA)
         {
#           ifdef PNG_FORMAT_AFIRST_SUPPORTED
               if (format & PNG_FORMAT_FLAG_AFIRST)
                  return gp_ag16;
               else
#           endif
               return gp_ga16;
         }
         else
            return gp_g16;
      }
   }
   else
   {
      if (format & PNG_FORMAT_FLAG_COLOR)
      {
#        ifdef PNG_FORMAT_BGR_SUPPORTED
            if (format & PNG_FORMAT_FLAG_BGR)
            {
               if (format & PNG_FORMAT_FLAG_ALPHA)
               {
#                 ifdef PNG_FORMAT_AFIRST_SUPPORTED
                     if (format & PNG_FORMAT_FLAG_AFIRST)
                        return gp_abgr8;
                     else
#                 endif
                     return gp_bgra8;
               }
               else
                  return gp_bgr8;
            }
            else
#        endif
         {
            if (format & PNG_FORMAT_FLAG_ALPHA)
            {
#              ifdef PNG_FORMAT_AFIRST_SUPPORTED
                  if (format & PNG_FORMAT_FLAG_AFIRST)
                     return gp_argb8;
                  else
#              endif
                  return gp_rgba8;
            }
            else
               return gp_rgb8;
         }
      }
      else
      {
         if (format & PNG_FORMAT_FLAG_ALPHA)
         {
#           ifdef PNG_FORMAT_AFIRST_SUPPORTED
               if (format & PNG_FORMAT_FLAG_AFIRST)
                  return gp_ag8;
               else
#           endif
               return gp_ga8;
         }
         else
            return gp_g8;
      }
   }
}
