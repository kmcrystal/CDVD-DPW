static size_t
store_message(png_store *ps, png_const_structp pp, char *buffer, size_t bufsize,
   size_t pos, PNG_CONST char *msg)
{
   if (pp != NULL && pp == ps->pread)
   {
      
      pos = safecat(buffer, bufsize, pos, "read: ");
      if (ps->current != NULL)
      {
         pos = safecat(buffer, bufsize, pos, ps->current->name);
         pos = safecat(buffer, bufsize, pos, sep);
      }
   }
   else if (pp != NULL && pp == ps->pwrite)
   {
      
      pos = safecat(buffer, bufsize, pos, "write: ");
      pos = safecat(buffer, bufsize, pos, ps->wname);
      pos = safecat(buffer, bufsize, pos, sep);
   }
   else
   {
      
      pos = safecat(buffer, bufsize, pos, "pngvalid: ");
   }
   if (ps->test[0] != 0)
   {
      pos = safecat(buffer, bufsize, pos, ps->test);
      pos = safecat(buffer, bufsize, pos, sep);
   }
   pos = safecat(buffer, bufsize, pos, msg);
   return pos;
}
