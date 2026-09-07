static void
standard_test(png_store* PNG_CONST psIn, png_uint_32 PNG_CONST id,
   int do_interlace, int use_update_info)
{
   standard_display d;
   context(psIn, fault);
   


   standard_display_init(&d, psIn, id, do_interlace, use_update_info);
   


   Try
   {
      png_structp pp;
      png_infop pi;
      


      pp = set_store_for_read(d.ps, &pi, d.id,
         d.do_interlace ?  (d.ps->progressive ?
            "pngvalid progressive deinterlacer" :
            "pngvalid sequential deinterlacer") : (d.ps->progressive ?
               "progressive reader" : "sequential reader"));
      
      standard_palette_init(&d);
      
      if (d.ps->progressive)
      {
         png_set_progressive_read_fn(pp, &d, standard_info, progressive_row,
            standard_end);
         
         store_progressive_read(d.ps, pp, pi);
      }
      else
      {
         
         png_set_read_fn(pp, d.ps, store_read);
         
         png_read_info(pp, pi);
         


         standard_info_imp(&d, pp, pi, 2 );
         



         {
            sequential_row(&d, pp, pi, 0, 1);
            


            if (!d.speed)
            {
               standard_text_validate(&d, pp, pi, 1);
               standard_image_validate(&d, pp, 0, 1);
            }
            else
               d.ps->validated = 1;
         }
      }
      
      if (!d.ps->validated)
         png_error(pp, "image read failed silently");
      
   }
   Catch(fault)
      d.ps = fault; 
   
   store_read_reset(d.ps);
}
