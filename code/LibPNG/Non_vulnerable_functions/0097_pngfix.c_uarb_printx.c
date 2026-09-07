static void
uarb_printx(uarb num, int digits, FILE *out)
   
{
   while (digits > 0)
      if (num[--digits] > 0)
      {
         fprintf(out, "0x%x", num[digits]);
         while (digits > 0)
            fprintf(out, "%.4x", num[--digits]);
      }
      else if (digits == 0) 
         fputs("0x0", out);
}
