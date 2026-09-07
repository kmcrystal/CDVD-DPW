static void
insert_tEXt(png_structp png_ptr, png_infop info_ptr, int nparams,
   png_charpp params)
{
   png_text text;
   check_param_count(nparams, 2);
   clear_text(&text, params[0]);
   set_text(png_ptr, info_ptr, &text, params[1]);
}
