static void
insert_iTXt(png_structp png_ptr, png_infop info_ptr, int nparams,
   png_charpp params)
{
   png_text text;
   check_param_count(nparams, 4);
   clear_text(&text, params[0]);
   text.compression = 2; 
   text.lang = params[1];
   text.lang_key = params[2]; 
   set_text(png_ptr, info_ptr, &text, params[3]);
}
