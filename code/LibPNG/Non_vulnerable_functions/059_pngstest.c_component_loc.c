static png_byte
component_loc(png_byte loc[4], png_uint_32 format)
{
   











   png_byte channels;
   if (format & PNG_FORMAT_FLAG_COLOR)
   {
      channels = 3;
      loc[2] = 1;
#     ifdef PNG_FORMAT_BGR_SUPPORTED
         if (format & PNG_FORMAT_FLAG_BGR)
         {
            loc[1] = 2;
            loc[3] = 0;
         }
         else
#     endif
      {
         loc[1] = 0;
         loc[3] = 2;
      }
   }
   else
   {
      channels = 1;
      loc[1] = loc[2] = loc[3] = 0;
   }
   if (format & PNG_FORMAT_FLAG_ALPHA)
   {
#     ifdef PNG_FORMAT_AFIRST_SUPPORTED
         if (format & PNG_FORMAT_FLAG_AFIRST)
         {
            loc[0] = 0;
            ++loc[1];
            ++loc[2];
            ++loc[3];
         }
         else
#     endif
         loc[0] = channels;
      ++channels;
   }
   else
      loc[0] = 4; 
   return channels;
}
