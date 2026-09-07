   const char *tests);
static int check_one_character(checkfp_command *co, checkfp_control c, int ch)
{
   

   png_size_t index = 0;
   const char test = (char)ch;
   const int number_is_valid = png_check_fp_number(&test, 1, &c.state, &index);
   const int character_accepted = (index == 1);
   if (c.check_state != exponent && isdigit(ch) && ch != '0')
      c.is_zero = 0;
   if (c.check_state == start && c.at_start && ch == '-')
      c.is_negative = 1;
   if (isprint(ch))
      co->number[c.cnumber++] = (char)ch;
   else
   {
      co->number[c.cnumber++] = '<';
      co->number[c.cnumber++] = hexdigits[(ch >> 4) & 0xf];
      co->number[c.cnumber++] = hexdigits[ch & 0xf];
      co->number[c.cnumber++] = '>';
   }
   co->number[c.cnumber] = 0;
   if (co->verbose > 1)
      fprintf(stderr, "%s\n", co->number);
   if (++(co->ctimes) == 1000000)
   {
      if (co->verbose == 1)
         fputc('.', stderr);
      co->ctimes = 0;
      ++(co->cmillions);
   }
   if (!number_is_valid)
      ++(co->cinvalid);
   if (!character_accepted)
      ++(co->cnoaccept);
   
   if (index != 0 && index != 1)
   {
      fprintf(stderr, "%s: read beyond end of string (%lu)\n", co->number,
         (unsigned long)index);
      return 0;
   }
   


   if (PNG_FP_IS_NEGATIVE(c.state) !=
      (number_is_valid && !c.is_zero && c.is_negative))
   {
      fprintf(stderr, "%s: negative when it is not\n", co->number);
      return 0;
   }
   if (PNG_FP_IS_ZERO(c.state) != (number_is_valid && c.is_zero))
   {
      fprintf(stderr, "%s: zero when it is not\n", co->number);
      return 0;
   }
   if (PNG_FP_IS_POSITIVE(c.state) !=
      (number_is_valid && !c.is_zero && !c.is_negative))
   {
      fprintf(stderr, "%s: positive when it is not\n", co->number);
      return 0;
   }
   
   if (isdigit(ch))
   {
      if (!character_accepted)
      {
         fprintf(stderr, "%s: digit '%c' not accepted\n", co->number, ch);
         return 0;
      }
      if (!number_is_valid)
      {
         fprintf(stderr, "%s: saw a digit (%c) but number not valid\n",
            co->number, ch);
         return 0;
      }
      ++c.cdigits_in_state;
      c.at_start = 0;
      c.number_was_valid = 1;
      



      if (c.cdigits_in_state < 1)
         return check_all_characters(co, c);
      else
         return check_some_characters(co, c,
            state_characters[c.check_state].tests);
   }
   
   else if (((ch == '+' || ch == '-') && c.check_state != fraction &&
               c.at_start) ||
            (ch == '.' && c.check_state == start) ||
            ((ch == 'e' || ch == 'E') && c.number_was_valid &&
               c.check_state != exponent))
   {
      if (!character_accepted)
      {
         fprintf(stderr, "%s: character '%c' not accepted\n", co->number, ch);
         return 0;
      }
      
      if (number_is_valid && (c.check_state != start || ch != '.'))
      {
         fprintf(stderr, "%s: saw a non-digit (%c) but number valid\n",
            co->number, ch);
         return 0;
      }
      c.number_was_valid = number_is_valid;
      



      if (c.check_state == start && ch == '.')
      {
         c.check_state = fraction;
         c.at_start = !number_is_valid;
         c.cdigits_in_state = 0;
         c.limit = co->limit;
         return check_all_characters(co, c);
      }
      else if (c.check_state < exponent && (ch == 'e' || ch == 'E'))
      {
         c.check_state = exponent;
         c.at_start = 1;
         c.cdigits_in_state = 0;
         c.limit = co->limit;
         return check_all_characters(co, c);
      }
      
      else
      {
         if (ch != '-' && ch != '+')
         {
            fprintf(stderr, "checkfp: internal error (1)\n");
            return 0;
         }
         c.at_start = 0;
         return check_all_characters(co, c);
      }
   }
   
   else
   {
      if (character_accepted)
      {
         fprintf(stderr, "%s: character '%c' [0x%.2x] accepted\n", co->number,
            ch, ch);
         return 0;
      }
      if (number_is_valid != c.number_was_valid)
      {
         fprintf(stderr,
            "%s: character '%c' [0x%.2x] changed number validity\n", co->number,
            ch, ch);
         return 0;
      }
      


   }
   
   return 1;
}
