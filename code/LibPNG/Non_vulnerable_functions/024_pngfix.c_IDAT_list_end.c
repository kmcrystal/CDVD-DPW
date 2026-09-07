static void
IDAT_list_end(struct IDAT_list *IDAT_list)
{
   struct IDAT_list *list = IDAT_list->next;
   CLEAR(*IDAT_list);
   while (list != NULL)
   {
      struct IDAT_list *next = list->next;
      clear(list, IDAT_list_size(list, 0));
      free(list);
      list = next;
   }
}
