static void
check_handling(display *d, int def, png_uint_32 chunks, png_uint_32 known,
   png_uint_32 unknown, const char *position, int set_callback)
{
   while (chunks)
   {
      png_uint_32 flag = chunks & -(png_int_32)chunks;
      int i = find_by_flag(flag);
      int keep = chunk_info[i].keep;
      const char *type;
      const char *errorx = NULL;
      if (chunk_info[i].unknown)
      {
         if (keep == PNG_HANDLE_CHUNK_AS_DEFAULT)
         {
            type = "UNKNOWN (default)";
            keep = def;
         }
         else
            type = "UNKNOWN (specified)";
         if (flag & known)
            errorx = "chunk processed";
         else switch (keep)
         {
            case PNG_HANDLE_CHUNK_AS_DEFAULT:
               if (flag & unknown)
                  errorx = "DEFAULT: unknown chunk saved";
               break;
            case PNG_HANDLE_CHUNK_NEVER:
               if (flag & unknown)
                  errorx = "DISCARD: unknown chunk saved";
               break;
            case PNG_HANDLE_CHUNK_IF_SAFE:
               if (ancillary(chunk_info[i].name))
               {
                  if (!(flag & unknown))
                     errorx = "IF-SAFE: unknown ancillary chunk lost";
               }
               else if (flag & unknown)
                  errorx = "IF-SAFE: unknown critical chunk saved";
               break;
            case PNG_HANDLE_CHUNK_ALWAYS:
               if (!(flag & unknown))
                  errorx = "SAVE: unknown chunk lost";
               break;
            default:
               errorx = "internal error: bad keep";
               break;
         }
      } 
      else 
      {
         type = "KNOWN";
         if (flag & known)
         {
            


            if (keep != PNG_HANDLE_CHUNK_AS_DEFAULT)
               errorx = "!DEFAULT: known chunk processed";
         }
         else  switch (keep)
         {
            case PNG_HANDLE_CHUNK_AS_DEFAULT:
               errorx = "DEFAULT: known chunk not processed";
               break;
            case PNG_HANDLE_CHUNK_NEVER:
               if (flag & unknown)
                  errorx = "DISCARD: known chunk saved";
               break;
            case PNG_HANDLE_CHUNK_IF_SAFE:
               if (ancillary(chunk_info[i].name))
               {
                  if (!(flag & unknown))
                     errorx = "IF-SAFE: known ancillary chunk lost";
               }
               else if (flag & unknown)
                  errorx = "IF-SAFE: known critical chunk saved";
               break;
            case PNG_HANDLE_CHUNK_ALWAYS:
               if (!(flag & unknown))
                  errorx = "SAVE: known chunk lost";
               break;
            default:
               errorx = "internal error: bad keep (2)";
               break;
         }
      }
      if (errorx != NULL)
      {
         ++(d->error_count);
         fprintf(stderr, "%s(%s%s): %s %s %s: %s\n", d->file, d->test,
            set_callback ? ",callback" : "",
            type, chunk_info[i].name, position, errorx);
      }
      chunks &= ~flag;
   }
}
