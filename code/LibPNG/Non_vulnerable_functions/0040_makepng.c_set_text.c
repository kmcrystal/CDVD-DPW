static void
set_text(png_structp png_ptr, png_infop info_ptr, png_textp text,
   png_charp param)
{
   switch (param[0])
   {
      case '<':
         {
            png_bytep file = NULL;
            text->text_length = load_file(param+1, &file);
            text->text = (png_charp)file;
         }
         break;
      case '0': case '1': case '2': case '3': case '4':
      case '5': case '6': case '7': case '8': case '9':
         {
            png_bytep data = NULL;
            png_size_t fake_len = load_fake(param, &data);
            if (fake_len > 0) 
            {
               text->text_length = fake_len;
               text->text = (png_charp)data;
               break;
            }
         }
      default:
         text->text = param;
         break;
   }
   png_set_text(png_ptr, info_ptr, text, 1);
   if (text->text != param)
      free(text->text);
}
