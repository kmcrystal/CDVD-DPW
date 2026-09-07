static void
zlib_end(struct zlib *zlib)
{
   


   if (!zlib->global->quiet)
   {
      if (zlib->ok_bits < 16) 
      {
         const char *reason;
         if (zlib->cksum)
            reason = "CHK"; 
         else if (zlib->ok_bits > zlib->file_bits)
            reason = "TFB"; 
         else if (zlib->ok_bits == zlib->file_bits)
            reason = "OK ";
         else
            reason = "OPT"; 
         



         type_name(zlib->chunk->chunk_type, stdout);
         printf(" %s %s %d %d ", reason, zlib_flevel(zlib), zlib->file_bits,
            zlib->ok_bits);
         uarb_print(zlib->compressed_bytes, zlib->compressed_digits, stdout);
         putc(' ', stdout);
         uarb_print(zlib->uncompressed_bytes, zlib->uncompressed_digits,
            stdout);
         putc(' ', stdout);
         fputs(zlib->file->file_name, stdout);
         putc('\n', stdout);
      }
      else
      {
         










         type_name(zlib->chunk->chunk_type, stdout);
         printf(" SKP %s %d %s ", zlib_flevel(zlib), zlib->file_bits,
            zlib_rc(zlib));
         uarb_print(zlib->compressed_bytes, zlib->compressed_digits, stdout);
         putc(' ', stdout);
         emit_string(zlib->z.msg ? zlib->z.msg : "[no_message]", stdout);
         putc(' ', stdout);
         fputs(zlib->file->file_name, stdout);
         putc('\n', stdout);
      }
   }
   if (zlib->state >= 0)
   {
      zlib->rc = inflateEnd(&zlib->z);
      if (zlib->rc != Z_OK)
         zlib_message(zlib, 1);
   }
   CLEAR(*zlib);
}
