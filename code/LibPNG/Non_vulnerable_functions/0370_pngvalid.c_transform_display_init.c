static void
transform_display_init(transform_display *dp, png_modifier *pm, png_uint_32 id,
    PNG_CONST image_transform *transform_list)
{
   memset(dp, 0, sizeof *dp);
   
   standard_display_init(&dp->this, &pm->this, id, 0,
      pm->use_update_info);
   
   dp->pm = pm;
   dp->transform_list = transform_list;
   
   dp->output_colour_type = 255; 
   dp->output_bit_depth = 255;  
}
