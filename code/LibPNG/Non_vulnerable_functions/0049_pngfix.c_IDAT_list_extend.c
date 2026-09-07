static struct IDAT_list *
IDAT_list_extend(struct IDAT_list *tail)
{
   
   struct IDAT_list *next = tail->next;
   if (next == NULL)
   {
      


      unsigned int length = 2 * tail->length;
      if (length < tail->length) 
         length = tail->length;
      next = png_voidcast(IDAT_list*, malloc(IDAT_list_size(NULL, length)));
      CLEAR(*next);
      
      if (next == NULL)
         return NULL;
      next->next = NULL;
      next->length = length;
      tail->next = next;
   }
   return next;
}
