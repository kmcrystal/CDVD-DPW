static void
IDAT_end(struct IDAT **idat_var)
{
   struct IDAT *idat = *idat_var;
   struct file *file = idat->file;
   *idat_var = NULL;
   CLEAR(*idat);
   assert(file->chunk != NULL);
   chunk_end(&file->chunk);
   



   file->state = STATE_CHUNKS;
}
