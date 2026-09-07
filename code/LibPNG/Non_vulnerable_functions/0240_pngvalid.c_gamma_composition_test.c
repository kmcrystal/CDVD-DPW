   defined(PNG_READ_ALPHA_MODE_SUPPORTED)
static void gamma_composition_test(png_modifier *pm,
   PNG_CONST png_byte colour_type, PNG_CONST png_byte bit_depth,
   PNG_CONST int palette_number,
   PNG_CONST int interlace_type, PNG_CONST double file_gamma,
   PNG_CONST double screen_gamma,
   PNG_CONST int use_input_precision, PNG_CONST int do_background,
   PNG_CONST int expand_16)
{
   size_t pos = 0;
   png_const_charp base;
   double bg;
   char name[128];
   png_color_16 background;
   
   switch (do_background)
   {
      default:
         base = "";
         bg = 4; 
         break;
      case PNG_BACKGROUND_GAMMA_SCREEN:
         base = " bckg(Screen):";
         bg = 1/screen_gamma;
         break;
      case PNG_BACKGROUND_GAMMA_FILE:
         base = " bckg(File):";
         bg = file_gamma;
         break;
      case PNG_BACKGROUND_GAMMA_UNIQUE:
         base = " bckg(Unique):";
         



         bg = (file_gamma + screen_gamma) / 3;
         break;
#ifdef PNG_READ_ALPHA_MODE_SUPPORTED
      case ALPHA_MODE_OFFSET + PNG_ALPHA_PNG:
         base = " alpha(PNG)";
         bg = 4; 
         break;
      case ALPHA_MODE_OFFSET + PNG_ALPHA_STANDARD:
         base = " alpha(Porter-Duff)";
         bg = 4; 
         break;
      case ALPHA_MODE_OFFSET + PNG_ALPHA_OPTIMIZED:
         base = " alpha(Optimized)";
         bg = 4; 
         break;
      case ALPHA_MODE_OFFSET + PNG_ALPHA_BROKEN:
         base = " alpha(Broken)";
         bg = 4; 
         break;
#endif
   }
   


   if (expand_16 || bit_depth == 16)
   {
      png_uint_32 r = random_32();
      background.red = (png_uint_16)r;
      background.green = (png_uint_16)(r >> 16);
      r = random_32();
      background.blue = (png_uint_16)r;
      background.gray = (png_uint_16)(r >> 16);
      





#     if DIGITIZE
         if (expand_16 && (do_background == PNG_BACKGROUND_GAMMA_UNIQUE ||
                           do_background == PNG_BACKGROUND_GAMMA_FILE) &&
            fabs(bg*screen_gamma-1) > PNG_GAMMA_THRESHOLD)
         {
            



            background.red = (png_uint_16)((background.red >> 8) * 257);
            background.green = (png_uint_16)((background.green >> 8) * 257);
            background.blue = (png_uint_16)((background.blue >> 8) * 257);
            background.gray = (png_uint_16)((background.gray >> 8) * 257);
         }
#     endif
   }
   else 
   {
      png_uint_32 r = random_32();
      background.red = (png_byte)r;
      background.green = (png_byte)(r >> 8);
      background.blue = (png_byte)(r >> 16);
      background.gray = (png_byte)(r >> 24);
   }
   background.index = 193; 
   if (!(colour_type & PNG_COLOR_MASK_COLOR))
   {
      


      background.red = background.green = background.blue = background.gray;
   }
   pos = safecat(name, sizeof name, pos, "gamma ");
   pos = safecatd(name, sizeof name, pos, file_gamma, 3);
   pos = safecat(name, sizeof name, pos, "->");
   pos = safecatd(name, sizeof name, pos, screen_gamma, 3);
   pos = safecat(name, sizeof name, pos, base);
   if (do_background < ALPHA_MODE_OFFSET)
   {
      
      pos = safecat(name, sizeof name, pos, "(");
      

      if (colour_type & PNG_COLOR_MASK_COLOR)
      {
         pos = safecatn(name, sizeof name, pos, background.red);
         pos = safecat(name, sizeof name, pos, ",");
         pos = safecatn(name, sizeof name, pos, background.green);
         pos = safecat(name, sizeof name, pos, ",");
         pos = safecatn(name, sizeof name, pos, background.blue);
      }
      else
         pos = safecatn(name, sizeof name, pos, background.gray);
      pos = safecat(name, sizeof name, pos, ")^");
      pos = safecatd(name, sizeof name, pos, bg, 3);
   }
   gamma_test(pm, colour_type, bit_depth, palette_number, interlace_type,
      file_gamma, screen_gamma, 0, 0, name, use_input_precision,
      0, expand_16, do_background, &background, bg);
}
