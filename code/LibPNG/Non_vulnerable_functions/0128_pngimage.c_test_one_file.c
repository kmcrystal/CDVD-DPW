static void
test_one_file(struct display *dp, const char *filename)
{
   


   dp->operation = "cache file";
   dp->transforms = 0;
   display_cache_file(dp, filename);
   update_display(dp);
   


   if (dp->ignored_transforms != 0)
   {
      read_png(dp, &dp->original_file, "ignored transforms",
         dp->ignored_transforms);
      
      if (!compare_read(dp, 0))
         return; 
   }
#ifdef PNG_WRITE_SUPPORTED
   



   dp->operation = "write";
   write_png(dp, dp->original_ip, 0);
   read_png(dp, &dp->written_file, NULL, 0);
   if (!compare_read(dp, 0))
      return;
#endif
   


   {
      



      unsigned int active = dp->active_transforms;
      const int exhaustive = (dp->options & EXHAUSTIVE) != 0;
      unsigned int current = first_transform(active);
      unsigned int bad_transforms = 0;
      unsigned int bad_combo = ~0U;    
      unsigned int bad_combo_list = 0; 
      for (;;)
      {
         read_png(dp, &dp->original_file, "active transforms", current);
         









#ifdef PNG_WRITE_SUPPORTED
         if ((current & write_transforms) == current)
         {
            




            dp->operation = "reversible transforms";
            write_png(dp, dp->read_ip, current);
            


            read_png(dp, &dp->written_file, NULL, 0);
            if (!compare_read(dp, current))
            {
               




               if (is_combo(current))
               {
                  bad_combo &= current;
                  bad_combo_list |= current;
               }
               else
                  bad_transforms |= current;
            }
         }
#endif
         
         if (exhaustive) 
         {
            unsigned int next = current;
            do
            {
               if (next == read_transforms) 
                  goto combo;
               ++next;
            }  


            while (  (next & read_transforms) <= current
                  || (next & active) == 0 
                  || (next & bad_transforms) != 0
                  || skip_transform(dp, next));
            assert((next & read_transforms) == next);
            current = next;
         }
         else 
         {
            active &= ~current;
            if (active == 0)
               goto combo;
            current = first_transform(active);
         }
      }
combo:
      if (dp->options & FIND_BAD_COMBOS)
      {
         



         if (bad_combo != ~0U)
            printf("%s[0x%x]: PROBLEM: 0x%x[0x%x] ANTIDOTE: 0x%x\n",
               dp->filename, active, bad_combo, bad_combo_list,
               rw_transforms & ~bad_combo_list);
         else
            printf("%s: no %sbad combos found\n", dp->filename,
               (dp->options & SKIP_BUGS) ? "additional " : "");
      }
   }
}
