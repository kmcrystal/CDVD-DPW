static int
buffer_from_file(struct buffer *buffer, FILE *fp)
{
   struct buffer_list *last = &buffer->first;
   size_t count = 0;
   for (;;)
   {
      size_t r = fread(last->buffer+count, 1,
         (sizeof last->buffer)-count, fp);
      if (r > 0)
      {
         count += r;
         if (count >= sizeof last->buffer)
         {
            assert(count == sizeof last->buffer);
            count = 0;
            if (last->next == NULL)
            {
               last = buffer_extend(last);
               if (last == NULL)
                  return MEMORY;
            }
            else
               last = last->next;
         }
      }
      else 
      {
         if (feof(fp))
         {
            buffer->last = last;
            buffer->end_count = count;
            return 0; 
         }
         
         return errno == 0 ? ERANGE : errno;
      }
   }
}
