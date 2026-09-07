static void
image_transform_reset_count(void)
{
   image_transform *next = image_transform_first;
   int count = 0;
   while (next != &image_transform_end)
   {
      next->local_use = 0;
      next->next = 0;
      next = next->list;
      ++count;
   }
   


   if (count > 32) abort();
}
