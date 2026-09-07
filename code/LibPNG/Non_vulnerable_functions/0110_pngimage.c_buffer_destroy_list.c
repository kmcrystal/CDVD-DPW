static void
buffer_destroy_list(struct buffer_list *list)
{
   if (list != NULL)
   {
      struct buffer_list *next = list->next;
      DELETE(list);
      buffer_destroy_list(next);
   }
}
