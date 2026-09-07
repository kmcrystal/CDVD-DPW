static void
transform_test(png_modifier *pmIn, PNG_CONST png_uint_32 idIn,
    PNG_CONST image_transform* transform_listIn, PNG_CONST char * volatile name)
{
   transform_display d;
   context(&pmIn->this, fault);
   transform_display_init(&d, pmIn, idIn, transform_listIn);
   Try
   {
      size_t pos = 0;
      png_structp pp;
      png_infop pi;
      char full_name[256];
      


      transform_set_encoding(&d);
      
      d.transform_list->ini(d.transform_list, &d);
      
      pos = safecat(full_name, sizeof full_name, pos, name);
      pos = safecat_current_encoding(full_name, sizeof full_name, pos, d.pm);
      
      pp = set_modifier_for_read(d.pm, &pi, d.this.id, full_name);
      standard_palette_init(&d.this);
#     if 0
         
         {
            char buffer[256];
            (void)store_message(&d.pm->this, pp, buffer, sizeof buffer, 0,
               "running test");
            fprintf(stderr, "%s\n", buffer);
         }
#     endif
      
      if (d.pm->this.progressive)
      {
         
         png_set_progressive_read_fn(pp, &d, transform_info, progressive_row,
            transform_end);
         
         modifier_progressive_read(d.pm, pp, pi);
      }
      else
      {
         
         png_set_read_fn(pp, d.pm, modifier_read);
         
         png_read_info(pp, pi);
         
         transform_info_imp(&d, pp, pi);
         sequential_row(&d.this, pp, pi, -1, 0);
         if (!d.this.speed)
            transform_image_validate(&d, pp, pi);
         else
            d.this.ps->validated = 1;
      }
      modifier_reset(d.pm);
   }
   Catch(fault)
   {
      modifier_reset(voidcast(png_modifier*,(void*)fault));
   }
}
