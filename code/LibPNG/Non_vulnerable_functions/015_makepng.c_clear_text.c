static void
clear_text(png_text *text, png_charp keyword)
{
   text->compression = -1; 
   text->key = keyword;
   text->text = NULL;
   text->text_length = 0; 
   text->itxt_length = 0; 
   text->lang = NULL;
   text->lang_key = NULL;
}
