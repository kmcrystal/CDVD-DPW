static void
buffer_read(struct display *dp, struct buffer *bp, png_bytep data,
   png_size_t size)
{
   struct buffer_list *last = bp->current;
   size_t read_count = bp->read_count;
   while (size > 0)
   {
      size_t avail;
      if (last == NULL ||
         (last == bp->last && read_count >= bp->end_count))
      {
         display_log(dp, USER_ERROR, "file truncated (%lu bytes)",
            (unsigned long)size);
         
         break;
      }
      else if (read_count >= sizeof last->buffer)
      {
         
         last = last->next;
         read_count = 0;
         bp->current = last; 
         
         if (last == NULL)
         {
            display_log(dp, INTERNAL_ERROR, "damaged buffer list");
            
            break;
         }
      }
      avail = (sizeof last->buffer) - read_count;
      if (avail > size)
         avail = size;
      memcpy(data, last->buffer + read_count, avail);
      read_count += avail;
      size -= avail;
      data += avail;
   }
   bp->read_count = read_count;
}
