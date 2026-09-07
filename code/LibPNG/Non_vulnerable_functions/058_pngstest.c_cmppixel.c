static int
cmppixel(Transform *transform, png_const_voidp in, png_const_voidp out,
   png_uint_32 x, png_uint_32 y)
{
   int maxerr;
   png_const_charp errmsg;
   Pixel pixel_in, pixel_calc, pixel_out;
   transform->in_gp(&pixel_in, in);
   if (transform->from_linear == NULL)
      transform->transform(&pixel_calc, &pixel_in, transform->background);
   else
   {
      transform->transform(&pixel_out, &pixel_in, transform->background);
      transform->from_linear(&pixel_calc, &pixel_out, NULL);
   }
   transform->out_gp(&pixel_out, out);
   
   if (pixel_calc.a == pixel_out.a && pixel_calc.r == pixel_out.r &&
      pixel_calc.g == pixel_out.g && pixel_calc.b == pixel_out.b)
      return 1;
   



   if (transform->output_8bit && pixel_calc.a == 0 && pixel_out.a == 0)
      return 1;
   


   errmsg = NULL;
   {
      int err_a = abs(pixel_calc.a-pixel_out.a);
      if (err_a > transform->error[3])
      {
         
         if (transform->accumulate)
            transform->error[3] = (png_uint_16)err_a;
         else
            errmsg = "alpha";
      }
   }
   


   if (errmsg == NULL && transform->output_8bit &&
      (pixel_calc.a == 0 || pixel_out.a == 0))
      return 1;
   if (errmsg == NULL) 
   {
      int err_r = abs(pixel_calc.r - pixel_out.r);
      int err_g = abs(pixel_calc.g - pixel_out.g);
      int err_b = abs(pixel_calc.b - pixel_out.b);
      int limit;
      if ((err_r | err_g | err_b) == 0)
         return 1; 
      
      if (pixel_in.a >= transform->in_opaque)
      {
         errmsg = "opaque component";
         limit = 2; 
      }
      else if (pixel_in.a > 0)
      {
         errmsg = "alpha component";
         limit = 1; 
      }
      else
      {
         errmsg = "transparent component (background)";
         limit = 0; 
      }
      maxerr = err_r;
      if (maxerr < err_g) maxerr = err_g;
      if (maxerr < err_b) maxerr = err_b;
      if (maxerr <= transform->error[limit])
         return 1; 
      


      if (transform->accumulate)
      {
         transform->error[limit] = (png_uint_16)maxerr;
         return 1; 
      }
   }
   
   return logpixel(transform, x, y, &pixel_in, &pixel_calc, &pixel_out, errmsg);
}
