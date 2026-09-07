static void
IDAT_list_init(struct IDAT_list *list)
{
   CLEAR(*list);
   list->next = NULL;
   list->length = IDAT_INIT_LENGTH;
}
