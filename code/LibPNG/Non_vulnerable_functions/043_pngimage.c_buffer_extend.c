static struct buffer_list *
buffer_extend(struct buffer_list *current)
{
   struct buffer_list *add;
   assert(current->next == NULL);
   add = NEW(struct buffer_list);
   if (add == NULL)
      return NULL;
   add->next = NULL;
   current->next = add;
   return add;
}
